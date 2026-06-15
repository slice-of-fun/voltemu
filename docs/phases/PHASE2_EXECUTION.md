# Phase 2 — Codebase Cleanup Execution

This document tracks the execution of **Phase 2: Codebase Cleanup** as defined in the project `ROADMAP.md`. 

The goal is to improve maintainability, readability, and architectural quality without altering emulation behavior. Due to the scale of the codebase, this phase is executed in iterative steps.

## Progress Overview

- [x] Step 1: Code Quality & Formatting
- [x] Step 2: Dead Code Removal *(verifiable subset; behavior-adjacent items deferred — see audit)*
- [ ] Step 3: Module Boundaries & Dependencies *(audited; build-sensitive edits deferred — see audit)*
- [ ] Step 4: Documentation & Testing *(4.1 done; 4.2 tests written, build-dependent items deferred)*

---

## Step 1: Code Quality & Formatting (Quick Wins)

**Objective:** Standardize code style and naming conventions across the project.

### 1.1 clang-format
- [x] Verify `.clang-format` rules are present and correct.
- [x] Run `clang-format` across `src/` to enforce consistent style.
- [x] Fix any manual formatting issues that `clang-format` struggles with.

> Done in commits `style(format): consolidate clang-format config to repo root`
> and `style(format): apply clang-format across src/`. Root `.clang-format` is now
> the single source of truth; legacy `src/.clang-format` removed; vendored
> `src/dynarmic/` excluded. CI pinned to clang-format 14. All SPDX/license headers
> verified intact (7451 → 7451). Result is idempotent (`--dry-run --Werror` passes).

### 1.2 Naming Conventions
- [x] Ensure all file names use `snake_case`.
- [x] Ensure all class/struct names use `PascalCase`.
- [x] Standardize constants to `UPPER_SNAKE_CASE`.

> Audited in `docs/phases/PHASE2_NAMING_AUDIT.md`. All source filenames already
> `snake_case` (0 violations). Lowercase nvidia device classes (`nvhost_*`, `nvmap`,
> etc.) are kept intentionally — they mirror real hardware/driver node names
> (documented exception per ROADMAP 2.2).

### 1.3 Modern C++ Idioms
- [ ] Replace raw `#define` constants with `constexpr`.
- [x] Replace `typedef` with `using` in core headers.

> `typedef` → `using`: 5 converted (`refactor: replace typedef with using`); the
> OpenSSL-mirroring `EVP_MAC_CTX` typedef is intentionally retained.
> `#define` → `constexpr`: **none applicable** — every candidate is platform-
> conditional, third-party-library config, or vendored data (SMAA, `U128_ZERO_INIT`).
> See audit doc for the full categorization.

---

## Step 2: Dead Code Removal

**Objective:** Prune unused and unreachable code to reduce compilation times and cognitive load.

> Audited in `docs/phases/PHASE2_STEP2_AUDIT.md`. This pass executed only changes
> verifiable **without a compiler** (no build toolchain in this environment).
> Behavior-adjacent removals were audited with explicit risk and deferred.

### 2.1 Unreachable Code
- [ ] Identify and remove dead code paths using static analysis.

> Deferred — requires clang-tidy/cppcheck + a build to confirm safety.

### 2.2 Deprecations
- [x] Remove Yuzu/Volt legacy compatibility shims.
- [x] Remove commented-out code blocks older than 6 months.

> Commented-out **transitional leftovers** removed (6 lines across `puller.cpp`,
> `patcher.cpp`, `time_zone_binary.cpp`). Feature-stub comments with `TODO`s were
> retained (documentation value). Legacy shims were **audited but retained** —
> they are user-data migration paths (Yuzu/Sudachi/Citron/Suyu/Ryujinx) whose
> removal would strand existing users; deprecate in a future release window.

### 2.3 Build & Localization Cleanup
- [x] Remove unused CMake options and variables.
- [x] Prune unused localization translation strings.

> CMake: automated scan flagged 4 vars; manual verification found 3 are live
> (consumed by the patched `openssl-cmake`/httplib externals) and 1
> (`VOLT_QT_MIRROR`) is intentional scaffolding — **net zero removals**.
> Localization: `.ts` files are auto-generated (lupdate → Transifex) with **zero**
> obsolete entries — **not applicable**.

---

## Step 3: Module Boundaries & Dependencies

**Objective:** Clean up architectural coupling between core components.

> Audited in `docs/phases/PHASE2_STEP3_AUDIT.md`. This is an **audit-only** pass:
> every candidate (include removal, cycle untangling, pimpl adoption) is
> build-sensitive and was recorded with evidence rather than applied — consistent
> with Step 2.1. **Net no-build edits: none.**

### 3.1 Include Reductions
- [ ] Audit and remove unnecessary `#include` directives.
- [ ] Resolve circular dependencies.
- [ ] Move implementation details from headers (`.h`) to source files (`.cpp`).

> Cross-subsystem coupling mapped: `core ↔ video_core` is the only true cycle
> (58/37 includes, structural). `audio_core → core` is heavy but largely
> one-way (76/20). `input_common` and `video_core ↔ audio_core` are already
> cleanly isolated (0/0). One concrete unused-include candidate found —
> `video_core/gpu.h:11` pulls `common/bit_field.h` with no `BitField` usage
> (grep-verified) — but removal is deferred (transitive-consumer / IWYU risk
> needs a build).

### 3.2 Public API Boundaries
- [ ] Define strict public APIs for `core`, `video_core`, and `audio_core`.
- [ ] Introduce interface headers where appropriate.

> The four facades (`System`, `GPU`, `AudioCore`, `InputSubsystem`) already serve
> as public entry points (documented in the Step 4.1 READMEs). Three use pimpl;
> `audio_core.h` is the lone exception (exposes concrete `unique_ptr` members) and
> is the clearest boundary-tightening candidate — deferred as a real refactor.

---

## Step 4: Documentation & Testing

**Objective:** Ensure all systems are well documented and have foundational test coverage.

### 4.1 Subsystem Documentation
- [x] Write `README.md` for `src/core/`.
- [x] Write `README.md` for `src/video_core/`.
- [x] Write `README.md` for `src/audio_core/`.
- [x] Write `README.md` for `src/input_common/`.
- [x] Add Doxygen blocks to all major public headers.

> Subsystem READMEs written from a per-subsystem architecture audit (not generic
> boilerplate): each documents the top-level owning class, a directory map, the
> primary data/command flow, and "where to start" pointers. Doxygen pass added
> class-level `@brief` summaries to the four facade headers (`core.h`, `gpu.h`,
> `audio_core.h`, `main.h`); member-level docs were already present.

### 4.2 Unit Tests
- [x] Audit existing `tests/` coverage.
- [x] Add unit tests for untested utilities in `src/common/`.
- [ ] Set up code coverage reporting scripts.

> Existing suite (Catch2, single `tests` target): 14 files covering bit_field,
> cityhash, container_hash, fibers, host_memory, param_package, range_map,
> ring_buffer, scratch_buffer, unique_function (common) plus core_timing,
> network, memory_tracker, and calibration_configuration_job. Added coverage for
> three previously-untested header-only utilities — `alignment.h`, `bit_util.h`,
> `div_ceil.h` — using compile-time `STATIC_REQUIRE` assertions where the API is
> `constexpr`. New files wired into `src/tests/CMakeLists.txt`. Building/running
> the suite (and the coverage scripts) requires a compile environment and is
> deferred to a build pass.

---

*This document will be updated as work progresses.*
