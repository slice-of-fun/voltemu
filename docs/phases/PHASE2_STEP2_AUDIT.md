# Phase 2 — Step 2 Dead Code Removal: Audit

This document records the dead-code audit for **Phase 2, Step 2** and the
rationale for what was removed vs. deliberately retained. The guiding rule for
this pass: **only changes verifiable without a compiler were executed.** Because
no build toolchain is available in this environment, behavior-adjacent removals
(unreachable runtime paths, deprecation of live features) were audited and
documented but **not** executed.

Vendored trees (`src/dynarmic/`, SMAA texture data, third-party externals) are
excluded throughout.

---

## 2.1 / 2.2 Commented-out code — EXECUTED (zero-risk subset)

A commented-out line cannot execute, so deleting a *transitional leftover*
(old approach commented directly above its active replacement) cannot change
behavior. Only that category was removed.

| File | Removed | Why safe |
|------|---------|----------|
| `src/video_core/engines/puller.cpp` | 2 lines: `UNIMPLEMENTED_MSG` + `WaitFence` in `FenceOperation::Acquire` | Superseded by the active `rasterizer->ReleaseFences()` on the next line. |
| `src/core/arm/nce/patcher.cpp` | 1 line: old `m_branch_to_module_relocations.push_back` in the `use_split` branch | Replaced by the active `..._pre.push_back` immediately below. |
| `src/core/hle/service/glue/time/time_zone_binary.cpp` | 3 lines: old `fmt::format("{}:/...", "TimeZoneBinary")` path constructions | Replaced by the active hardcoded `/...` paths on the following lines. |

**Total: 6 lines across 3 files.** `clang-format --dry-run --Werror` passes on all
three.

### Commented-out code RETAINED (intentional — not dead)

These look like dead code but carry documentation value (a disabled feature with
a `TODO`/marker explaining *why* it's off). Removing them loses intent, and the
"right" fix is to *implement* the feature, not delete the breadcrumb — out of
scope for a cleanup pass.

| File | Lines | Reason retained |
|------|-------|-----------------|
| `src/core/hle/kernel/init/init_slab_setup.cpp` | ~212 | `memset` blocked on a documented unmet dependency ("implement access to kernel VAs"). |
| `src/core/hle/service/am/button_poller.cpp` | ~89, ~132 | Power-button handling stubbed with `TODO`; signals planned work. |
| `src/core/hle/service/ro/ro.cpp` | ~261 | IPS patch application — known disabled feature. |
| `src/core/hle/service/hid/hid_server.cpp` | ~242 | Handler table entry marked uncertain ("What?"). |
| `src/video_core/renderer_opengl/present/layer.cpp` | ~201 | Should be *replaced* with active logging, not deleted — a missing runtime case. |

---

## 2.2 Legacy compatibility shims — AUDITED, NOT REMOVED (high user-data risk)

The legacy shims in this fork are almost entirely **user-data migration** paths.
Removing them strands existing Yuzu / Sudachi / Citron / Suyu / Ryujinx users on
their next launch. These are **deferred**, per the ROADMAP's intent, to a future
deprecation window (1–2 release cycles with release-note warning) — not a
mechanical cleanup pass.

| Component | Files | Risk | Disposition |
|-----------|-------|------|-------------|
| Legacy emulator path system (`EmuPath` enum, `GetLegacyPath*`) | `src/common/fs/fs_paths.h`, `src/common/fs/path_util.{h,cpp}` | **HIGH** | Keep — actively queried by migration. |
| User-data migration UI + worker | `src/yuzu/user_data_migration.*`, `src/yuzu/migration_worker.*` | **MEDIUM** | Keep — first-launch migration for existing users. |
| Ryujinx save linking + KVDB parse | `src/common/fs/ryujinx_compat.*`, `src/qt_common/util/fs.*` | **MEDIUM** | Keep — breaks users with linked saves. |
| QSettings backwards-compat escaping | `src/frontend_common/config.cpp` (~1086) | **LOW** | Keep — config deserialization safety. |

> Cross-phase note: `src/common/fs/fs_paths.h` still defines `EDEN_DIR "eden"` and
> related fork-name macros. These belong to **Phase 1 rebranding**, not Phase 2,
> and are tracked there.

---

## 2.3 Build (CMake) cleanup — AUDITED, minimal action

An automated sweep flagged four variables; manual verification rejected three of
them as **false positives**:

| Candidate | Verdict | Evidence |
|-----------|---------|----------|
| `OPENSSL_BUILD_VERBOSE` (`CMakeLists.txt:390`) | **NOT dead** | Consumed by the patched `openssl-cmake` external — `option(...)` at `.patch/openssl-cmake/0001-cpmutil-compat.patch:27`, read at `:152`. |
| `OPENSSL_CONFIGURE_VERBOSE` (`CMakeLists.txt:391`) | **NOT dead** | Same patch, read at `:152`. |
| `HTTPLIB_USE_BROTLI_IF_AVAILABLE` | **NOT dead** | Passed through to the httplib CPM package config. |
| `VOLT_QT_MIRROR` (`CMakeLists.txt:76`) | **Orphaned but intentional** | Zero consumers, but it is a `VOLT_`-prefixed cache option — scaffolding for a not-yet-implemented bundled-Qt mirror selector. Left in place; removing it would delete a deliberate extension point, not dead code. |

**Net CMake action: none.** The lesson reinforced: automated "unused variable"
scans that exclude `.patch/` files produce false positives in a CPM/patch-based
build.

---

## 2.3 Localization cleanup — NOT APPLICABLE

The `.ts` files in `dist/languages/` are **auto-generated** (lupdate → Transifex →
CI). Across 27 languages + English plurals there are **zero** `type="obsolete"`
or `type="vanished"` entries. Manual pruning is both unnecessary (nothing stale)
and pointless (the next lupdate/Transifex sync regenerates the files). No action.

---

## Summary

| Sub-step | Action taken |
|----------|--------------|
| 2.1 Unreachable code | Deferred — needs static analysis + build. |
| 2.2 Commented-out code | **6 lines removed** (transitional leftovers); feature-stub comments retained. |
| 2.2 Legacy shims | Audited; **retained** (user-data migration risk; deprecate later). |
| 2.3 CMake | Audited; **no removal** (3 false positives, 1 intentional scaffold). |
| 2.3 Localization | **N/A** — auto-generated, zero obsolete strings. |

The high-value targets in Step 2 are all **behavior-adjacent** and require a build
environment to land safely. This pass took the verifiable wins and documented the
rest with explicit risk and disposition.
