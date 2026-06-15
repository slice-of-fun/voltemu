<!-- SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project -->
<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Phase 1 — Step 1.2 / 1.3 Audit: Rebrand (Application Identity & Build System)

A **no-build inspection** of remaining "Eden" references for the Volt rebrand,
covering ROADMAP Phase 1.2 (Application Identity) and 1.3 (Build System). Edits
are **not applied** here — string/identity changes need a build (and, for some,
a user-data migration strategy) to verify safely. Findings are evidence-backed
(file:line, grep-confirmed). Historical/legal references (SPDX headers,
copyright, ATTRIBUTION, lineage mentions in `docs/`) are **out of scope** and
intentionally preserved.

## 1.3 Build System — status: largely DONE, two leftovers

| Item | State | Evidence |
|------|-------|----------|
| CMake project renamed to `volt` | ✅ done | `CMakeLists.txt:6` → `project(volt)` |
| `EDEN_` CMake variables removed | ✅ done | 0 `EDEN_` vars in non-vendored `*.cmake`/`CMakeLists.txt` |
| Main target renamed to `volt` | ✅ done | `src/yuzu/CMakeLists.txt:17` `add_executable(volt`, `:244` `OUTPUT_NAME "volt"` |
| vcpkg manifest name | N/A | no root `vcpkg.json` in tree |

### 🔴 Build-System leftover (real bug, currently latent)
- **`src/yuzu/CMakeLists.txt:366`** — `set_target_properties(eden PROPERTIES ...)`
  references a target named `eden` that **no longer exists** (renamed to `volt`).
  This is inside an `if(APPLE)` + `CMAKE_GENERATOR MATCHES "Xcode"` block, so it
  only fires on macOS/Xcode — which is why it hasn't surfaced. On that path it is
  a **configure-time error**. Adjacent stale asset refs: `:364` `eden.icon`,
  `:374` `eden.icns`.
  > Fix is a one-line target rename + asset path update, but **deferred** —
  > needs a macOS/Xcode configure to verify, and the asset files themselves
  > (`dist/eden.icns`, `dist/eden.icon`) are part of the 1.6 asset rework.

### `src/` source leftover
- **`EDEN_DIR "eden"`** (5 refs in `fs_paths.h` + consumers) — the on-disk user
  data directory name. **Cross-cutting with user-data migration** (same class as
  the Phase 2 legacy shims): renaming it strands existing installs' data. Belongs
  to a deliberate migration window, not a blind rename. *(Already noted in
  `PHASE2_STEP2_AUDIT.md` as a Phase 1 item.)*

## 1.2 Application Identity — substantial work remaining

52 user-facing "Eden" references found across desktop (Qt), macOS, and Android.
Grouped below by **migration risk**, because not all renames are equal: some are
cosmetic, but a few silently relocate user config or break OS-level associations.

### Tier A — ⚠️ Migration-sensitive (do NOT blind-replace)
Changing these moves or orphans existing users' data/associations. Each needs a
migration plan (read-old-then-write-new, or a one-time import), verified on a
build.

| Ref | File:line | Risk |
|-----|-----------|------|
| `setOrganizationName("eden")` | `src/yuzu/main.cpp:115` | Relocates **all** stored config (Windows registry / macOS prefs / Linux `~/.config`) |
| `setApplicationName("eden")` | `src/yuzu/main.cpp:116` | Same — config/data path key |
| `setDesktopFileName("dev.eden_emu.eden")` | `src/yuzu/main.cpp:158` | Must match the installed `.desktop` id (`dist/dev.volt_emu.volt.desktop`) — **currently mismatched** |
| `EDEN_DIR "eden"` | `src/common/fs/fs_paths.h` (+consumers) | On-disk data dir (see 1.3) |
| UTType ids `com.eden-emu.{xci,nsp,nca,nro,nso,xaml}` | `src/yuzu/Info.plist` (6) | macOS file-type associations; changing breaks existing "open with" links |

> Note: `setDesktopFileName` (`dev.eden_emu.eden`) already disagrees with the
> shipped desktop file (`dev.volt_emu.volt`) — a pre-existing inconsistency worth
> flagging regardless of rebrand timing.

### Tier B — Cosmetic UI strings (safe rename, build-verify only)
Pure display text; no migration impact. Deferred only because Qt `.ui`/`tr()`
changes want a build + ideally a translation (`lupdate`) refresh.

**Qt `.ui` (desktop):**
- `aboutdialog.ui:14` "About Eden"; `:72` 28pt "Eden" heading; `:105` description; `:147` Nintendo disclaimer
- `configuration/configure.ui:20` "Eden Configuration"
- `configuration/configure_web.ui:25` "Eden Web Service"
- `deps_dialog.ui:14/:72/:85` "Eden Dependencies" + body; widget `labelEden`
- `main.ui:62` "Open &Eden Folders"; `:294` "&About Eden"; `:573` "&Eden Dependencies"
- `ryujinx_dialog.ui:26/:28/:40` migration copy + "From Eden" button
- `configuration/system/new_user_dialog.ui:60` default "Eden"

**Qt `tr()` message-box titles (`tr("Eden")`):**
- `configure_general.cpp:108`; `configure_motion_touch.cpp:175,179,183,189,195,296` (6); `main_window.cpp:4589,4687,4700` (3)

**Other code strings:**
- `main_window.cpp:1937` `LOG_INFO(Frontend, "Eden starting...")`
- `main_window.cpp:1881` video-core error text "Eden has encountered…"
- `new_user_dialog.cpp:68` default username `"Eden"`
- `multiplayer/lobby.cpp:71,78` default nickname `"Eden"` *(moderate — also a stored value)*

**Generated artifact filenames:**
- `video_core/gpu_logging/gpu_logging.cpp:61,64` `eden_gpu.log` / `eden_gpu.log.old.txt`

### Tier C — Internal identifiers (optional; not user-visible)
Cosmetic only; may appear in crash/debug symbols. Lowest priority.
- `main_window.h:396` `OnEdenDependencies()`; `:590` `GetEdenCommand()`
- `main_window.cpp:1654` `ui->action_Eden_Dependencies`

### macOS / Android identity (platform 1.4 / 1.5 overlap)
- `Info.plist:30` `CFBundleName "eden"`; `:20` `eden.icns`; `:22` `eden_liquidglass`; `:79` copyright string (user-visible in About)
- Android `strings.xml` user-facing: `:13,:14,:211,:306,:327,:330,:818` (notifications, welcome, file/log descriptions, shader-wipe dialog)

## Summary

| Area | Disposition |
|------|-------------|
| 1.3 CMake project/vars/target | ✅ mostly done; **1 latent macOS/Xcode bug** (`CMakeLists.txt:366`) + `EDEN_DIR` |
| 1.2 Tier A (migration-sensitive) | Documented; **needs migration plan + build** — do not blind-replace |
| 1.2 Tier B (cosmetic UI) | Documented; safe rename, deferred to build + `lupdate` pass |
| 1.2 Tier C (internal) | Optional, lowest priority |
| macOS / Android identity | Overlaps Phase 1.4/1.5; documented for that workstream |

**Net no-build edits this pass: none.** Every change is either migration-risky
(Tier A) or build-/translation-verified (Tier B/C), consistent with the audit
discipline used throughout Phase 2. The cleanest *first* execution targets, once
a build is available, are: (1) the `CMakeLists.txt:366` `eden`→`volt` target fix
(real bug), and (2) Tier B cosmetic strings (zero migration risk).
