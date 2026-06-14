# Phase 2 — Codebase Cleanup Execution

This document tracks the execution of **Phase 2: Codebase Cleanup** as defined in the project `ROADMAP.md`. 

The goal is to improve maintainability, readability, and architectural quality without altering emulation behavior. Due to the scale of the codebase, this phase is executed in iterative steps.

## Progress Overview

- [x] Step 1: Code Quality & Formatting
- [ ] Step 2: Dead Code Removal
- [ ] Step 3: Module Boundaries & Dependencies
- [ ] Step 4: Documentation & Testing

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

### 2.1 Unreachable Code
- [ ] Identify and remove dead code paths using static analysis.

### 2.2 Deprecations
- [ ] Remove Yuzu/Eden legacy compatibility shims.
- [ ] Remove commented-out code blocks older than 6 months.

### 2.3 Build & Localization Cleanup
- [ ] Remove unused CMake options and variables.
- [ ] Prune unused localization translation strings.

---

## Step 3: Module Boundaries & Dependencies

**Objective:** Clean up architectural coupling between core components.

### 3.1 Include Reductions
- [ ] Audit and remove unnecessary `#include` directives.
- [ ] Resolve circular dependencies.
- [ ] Move implementation details from headers (`.h`) to source files (`.cpp`).

### 3.2 Public API Boundaries
- [ ] Define strict public APIs for `core`, `video_core`, and `audio_core`.
- [ ] Introduce interface headers where appropriate.

---

## Step 4: Documentation & Testing

**Objective:** Ensure all systems are well documented and have foundational test coverage.

### 4.1 Subsystem Documentation
- [ ] Write `README.md` for `src/core/`.
- [ ] Write `README.md` for `src/video_core/`.
- [ ] Write `README.md` for `src/audio_core/`.
- [ ] Write `README.md` for `src/input_common/`.
- [ ] Add Doxygen blocks to all major public headers.

### 4.2 Unit Tests
- [ ] Audit existing `tests/` coverage.
- [ ] Add unit tests for untested utilities in `src/common/`.
- [ ] Set up code coverage reporting scripts.

---

*This document will be updated as work progresses.*
