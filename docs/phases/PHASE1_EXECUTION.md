# Phase 1 — Rebranding & Foundation: Execution Plan

This document is the authoritative task list for Phase 1. Every item must be completed and verified before Phase 1 is considered done.

---

## Overview

**Goal:** Establish Volt Emulator as a distinct project with its own identity.  
**Scope:** Identity, metadata, branding, infrastructure — no functional emulation changes.  
**Risk level:** Low (no behavior change) but high attention to legal compliance.

---

## Step-by-Step Execution

### Step 0 — Repository Initialization

| # | Task | Status | Notes |
|---|------|--------|-------|
| 0.1 | Clone Eden repository | ⬜ | `git clone https://git.eden-emu.dev/eden-emu/eden.git volt-emulator` |
| 0.2 | Initialize submodules | ⬜ | `git submodule update --init --recursive` |
| 0.3 | Create `volt-emu` GitHub organization | ⬜ | |
| 0.4 | Push to new remote | ⬜ | `git remote set-url origin https://github.com/volt-emu/volt.git` |
| 0.5 | Set up branch protection on `main` | ⬜ | Require PR + 1 review |
| 0.6 | Create `dev` branch | ⬜ | Default development branch |
| 0.7 | Verify CI pipeline runs | ⬜ | GitHub Actions |
| 0.8 | Create issue and PR templates | ⬜ | `.github/ISSUE_TEMPLATE/`, `.github/PULL_REQUEST_TEMPLATE.md` |

---

### Step 1 — Legal Audit (Before Any Changes)

> **Critical:** Do this FIRST. You need a baseline before changing anything.

| # | Task | Status | Notes |
|---|------|--------|-------|
| 1.1 | Run `scripts/rebranding/02_audit.sh` on clean clone | ⬜ | Establish baseline |
| 1.2 | Review all SPDX headers — document count | ⬜ | Must all survive rebrand |
| 1.3 | Review all copyright notices — document count | ⬜ | Must all survive rebrand |
| 1.4 | Review `LICENSES/` directory contents | ⬜ | Keep all files intact |
| 1.5 | Document all upstream attribution blocks | ⬜ | Cross-check post-rebrand |

---

### Step 2 — CMake & Build System

| # | Task | Status | Notes |
|---|------|--------|-------|
| 2.1 | Rename CMake project: `eden` → `volt` | ✅ | `CMakeLists.txt:6` `project(volt)` (audit-confirmed) |
| 2.2 | Rename all `EDEN_` variables to `VOLT_` | ✅ | 0 `EDEN_` vars remain in non-vendored cmake (audit-confirmed) |
| 2.3 | Rename CMake targets: `eden-*` → `volt-*` | ⚠️ | Main target is `volt` (`src/yuzu/CMakeLists.txt:17,244`); **leftover `eden` target at `:366`** in APPLE+Xcode block — latent configure error, see rebrand audit |
| 2.4 | Update version variables | ⬜ | `VOLT_VERSION_MAJOR/MINOR/PATCH` — not yet located in audit |
| 2.5 | Update vcpkg manifest name | N/A | No root `vcpkg.json` in tree (audit-confirmed) |
| 2.6 | Verify build compiles successfully | ⬜ | Windows + Linux |
| 2.7 | Commit step | ⬜ | `rebrand(cmake): rename project and variables` |

---

### Step 3 — Source Code — User-Facing Strings

> **Audited (no-build):** see `docs/phases/PHASE1_REBRAND_AUDIT.md` for the full
> 52-reference inventory grouped by migration risk. Tier A (org/app name, desktop
> id, `EDEN_DIR`, macOS UTType ids) must NOT be blind-replaced — they relocate
> user config / break OS associations and need a migration plan. Tier B (cosmetic
> UI strings) is safe to rename but wants a build + `lupdate` pass.

| # | Task | Status | Notes |
|---|------|--------|-------|
| 3.1 | Window title: "Eden" → "Volt Emulator" | ⬜ | `aboutdialog.ui`, `configure.ui:20`, `deps_dialog.ui:14` (audit Tier B) |
| 3.2 | About dialog: all Eden references | ⬜ | `aboutdialog.ui:14,72,105,147` — keep upstream attribution |
| 3.3 | Log output prefixes: "[eden]" → "[volt]" | ⬜ | `main_window.cpp:1937` "Eden starting…" |
| 3.4 | Error messages: "Eden" → "Volt Emulator" | ⬜ | `main_window.cpp:1881`; 9× `tr("Eden")` msgbox titles (audit Tier B) |
| 3.5 | Crash reporter: "Eden" → "Volt Emulator" | ⬜ | Also `eden_gpu.log` filenames (`gpu_logging.cpp:61,64`) |
| 3.6 | `namespace Eden` → `namespace Volt` | ⬜ | Forward-compat only; use with care |
| 3.7 | String literal audit: remaining "Eden" in `.cpp`/`.h` | ✅ | Inventoried in rebrand audit (Tier A/B/C) |
| 3.8 | Verify no license lines were touched | ⬜ | Run audit script, compare counts |
| 3.9 | Commit step | ⬜ | `rebrand(src): update user-facing strings` |

---

### Step 4 — Platform Metadata

#### Windows
| # | Task | Status | Notes |
|---|------|--------|-------|
| 4.1 | Update `.rc` file: product name | ⬜ | "Volt Emulator" |
| 4.2 | Update `.rc` file: company name | ⬜ | "Volt Emulator Team" |
| 4.3 | Update `.rc` file: description | ⬜ | Project description |
| 4.4 | Update `.rc` file: executable name | ⬜ | `volt.exe` |
| 4.5 | Update NSIS/WiX installer: all branding | ⬜ | Name, icon, exe name |
| 4.6 | Update installer output filename | ⬜ | `Volt-Emulator-Setup-vX.X.X.exe` |

#### Linux
| # | Task | Status | Notes |
|---|------|--------|-------|
| 4.7 | Update `.desktop` file: `Name=Volt Emulator` | ⬜ | |
| 4.8 | Update `.desktop` file: `Exec=volt` | ⬜ | |
| 4.9 | Update `.desktop` file: `Icon=volt` | ⬜ | |
| 4.10 | Update AppStream/metainfo XML: app ID | ⬜ | `dev.volt_emu.volt` |
| 4.11 | Update AppStream XML: name, description | ⬜ | |
| 4.12 | Update Flatpak manifest: app ID | ⬜ | |
| 4.13 | Update Snap manifest: name | ⬜ | |
| 4.14 | Commit step | ⬜ | `rebrand(platform): update desktop and installer metadata` |

---

### Step 5 — Android

| # | Task | Status | Notes |
|---|------|--------|-------|
| 5.1 | Update `applicationId`: `dev.volt_emu.volt` | ⬜ | `build.gradle` |
| 5.2 | Update app label: "Volt Emulator" | ⬜ | `strings.xml` |
| 5.3 | Rename Java/Kotlin package directories | ⬜ | `dev/eden_emu/` → `dev/volt_emu/` |
| 5.4 | Update package declarations in all `.kt` / `.java` files | ⬜ | Match new package |
| 5.5 | Update `AndroidManifest.xml` package | ⬜ | |
| 5.6 | Update all Gradle files | ⬜ | App ID, build variants |
| 5.7 | Update JNI bridge class paths | ⬜ | Native method resolution |
| 5.8 | Replace launcher icon (placeholder Volt icon) | ⬜ | All densities + adaptive |
| 5.9 | Replace splash screen | ⬜ | |
| 5.10 | Build Android APK successfully | ⬜ | Debug build minimum |
| 5.11 | Commit step | ⬜ | `rebrand(android): update package name and branding` |

---

### Step 6 — Assets

| # | Task | Status | Notes |
|---|------|--------|-------|
| 6.1 | Design Volt logo (SVG) | ⬜ | Lightning bolt / volt theme |
| 6.2 | Export application icon set: 16, 32, 48, 64, 128, 256px PNG | ⬜ | |
| 6.3 | Create Windows `.ico` bundle | ⬜ | All sizes in one file |
| 6.4 | Create macOS `.icns` (if macOS support planned) | ⬜ | |
| 6.5 | Create Android adaptive icon (foreground + background) | ⬜ | `ic_launcher_foreground.xml` |
| 6.6 | Create Linux hicolor icon set | ⬜ | `/usr/share/icons/hicolor/` structure |
| 6.7 | Create splash screen / loading screen | ⬜ | |
| 6.8 | Place all assets in correct source locations | ⬜ | |
| 6.9 | Commit step | ⬜ | `rebrand(assets): add Volt Emulator branding assets` |

---

### Step 7 — Documentation

| # | Task | Status | Notes |
|---|------|--------|-------|
| 7.1 | README.md | ✅ | Complete |
| 7.2 | CONTRIBUTING.md | ✅ | Complete |
| 7.3 | ATTRIBUTION.md | ✅ | Complete |
| 7.4 | CHANGELOG.md | ✅ | Complete |
| 7.5 | SECURITY.md | ✅ | Complete |
| 7.6 | CODE_OF_CONDUCT.md | ✅ | Complete |
| 7.7 | CONTRIBUTORS.md | ✅ | Complete |
| 7.8 | docs/BUILDING.md | ✅ | Complete |
| 7.9 | docs/SETUP.md | ✅ | Complete |
| 7.10 | docs/CONFIGURATION.md | ✅ | Complete |
| 7.11 | docs/phases/ROADMAP.md | ✅ | Complete |
| 7.12 | docs/architecture/ARCHITECTURE.md | ✅ | Complete |
| 7.13 | docs/standards/CODING_STANDARDS.md | ✅ | Complete |
| 7.14 | docs/legal/THIRD_PARTY_LICENSES.md | ✅ | Complete |
| 7.15 | Update Eden's original README → archive it | ⬜ | Move to `docs/upstream/EDEN_README.md` |
| 7.16 | Commit step | ⬜ | `docs: add Phase 1 documentation suite` |

---

### Step 8 — Legal Verification (Post-Rebrand)

| # | Task | Status | Notes |
|---|------|--------|-------|
| 8.1 | Run `02_audit.sh` again — compare to baseline | ⬜ | License count must be unchanged |
| 8.2 | Manually verify 10 random source files — license headers intact | ⬜ | Spot check |
| 8.3 | Verify `ATTRIBUTION.md` is accurate | ⬜ | |
| 8.4 | Verify Eden/Sudachi/Yuzu attribution blocks still exist in `about` dialog | ⬜ | |
| 8.5 | Run `grep -r "Copyright" src/ | wc -l` — compare to pre-rebrand count | ⬜ | Should be equal |
| 8.6 | Generate third-party license report | ⬜ | `cmake --build build --target volt_licenses` (if target exists) |

---

### Step 9 — CI/CD Setup

| # | Task | Status | Notes |
|---|------|--------|-------|
| 9.1 | Update GitHub Actions workflows: project name | ⬜ | |
| 9.2 | Configure build matrix: Windows, Linux, Android | ⬜ | |
| 9.3 | Add format check step (`clang-format --dry-run`) | ⬜ | |
| 9.4 | Add tidy check step (`clang-tidy`) | ⬜ | |
| 9.5 | Configure release artifact naming: `volt-*` | ⬜ | |
| 9.6 | Set up release pipeline draft | ⬜ | |

---

### Step 10 — Phase 1 Exit Verification

| # | Criterion | Status |
|---|-----------|--------|
| 10.1 | Application builds on Windows (MSVC) | ⬜ |
| 10.2 | Application builds on Linux (Clang) | ⬜ |
| 10.3 | Android APK builds successfully | ⬜ |
| 10.4 | Window title shows "Volt Emulator" | ⬜ |
| 10.5 | About dialog shows "Volt Emulator" | ⬜ |
| 10.6 | About dialog still shows Eden/Sudachi/Yuzu attribution | ⬜ |
| 10.7 | Android app name shows "Volt Emulator" | ⬜ |
| 10.8 | Android package is `dev.volt_emu.volt` | ⬜ |
| 10.9 | Log output shows `[Volt]` prefix | ⬜ |
| 10.10 | No user-visible "Eden" branding remaining | ⬜ |
| 10.11 | All SPDX headers intact (count matches pre-rebrand) | ⬜ |
| 10.12 | All copyright notices intact | ⬜ |
| 10.13 | Documentation baseline complete (all 7.x items ✅) | ✅ |
| 10.14 | CI passes on `dev` branch | ⬜ |
| 10.15 | Phase 1 release tagged: `v0.1.0` | ⬜ |

---

## Commit Message Convention for Phase 1

All Phase 1 commits use the `rebrand` type:

```
rebrand(scope): short description

Detailed explanation if needed.

Part of Volt Emulator Phase 1 rebranding.
Upstream lineage preserved: Volt ← Eden ← Sudachi ← Yuzu
All license headers and copyright notices unchanged.
```

Scopes: `cmake`, `src`, `android`, `windows`, `linux`, `assets`, `docs`, `ci`

---

## Time Estimate

| Step | Estimated Effort |
|------|----------------|
| 0 — Repository | 2–4 hours |
| 1 — Legal audit | 1–2 hours |
| 2 — CMake | 2–4 hours |
| 3 — Source strings | 4–8 hours |
| 4 — Platform metadata | 2–4 hours |
| 5 — Android | 4–8 hours |
| 6 — Assets (design) | 8–16 hours |
| 7 — Documentation | ✅ Done |
| 8 — Legal verify | 1–2 hours |
| 9 — CI/CD | 2–4 hours |
| **Total** | **~3–6 days** |
