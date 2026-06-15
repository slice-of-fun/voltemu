<!-- SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project -->
<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Phase 1 — Step 1 Legal Audit (Baseline)

A **no-build inspection** establishing the legal-compliance baseline for the Volt
rebrand, covering ROADMAP Phase 1 Step 1 (1.1–1.5). This baseline is what Step 8
(post-rebrand legal verification) compares against: the SPDX/copyright/license
counts recorded here **must survive** every subsequent rebrand edit unchanged.

Scope excludes the vendored `src/dynarmic/` subtree (third-party, own licensing).

## 1.1 / 1.2 — SPDX & license-identifier baseline

| Metric | Count |
|--------|-------|
| Tracked C/C++ source files in `src/` (excl. dynarmic) | 2619 |
| `SPDX-License-Identifier` lines (excl. dynarmic) | 4103 |
| `SPDX-FileCopyrightText` lines (excl. dynarmic) | 4169 |
| `Copyright` lines total (excl. dynarmic) — Step 8 comparator | 4201 |

### Distinct SPDX license identifiers in use

| Identifier | Lines | Notes |
|-----------|-------|-------|
| `GPL-2.0-or-later` | 2396 | yuzu/Citra-era files (project default historically) |
| `GPL-3.0-or-later` | 1976 | Volt-era headers + project relicensing |
| `0BSD` | 385 | permissive, mostly generated/trivial |
| `MIT` | 18 | |
| `MPL-2.0` | 2 | |
| `BSD-3-Clause [AND GPL-2.0-or-later]` | 4 | dual-tagged |
| `WTFPL` / `CC0-1.0` / `BSL-1.0` / `BSD-2-Clause` / `Apache-2.0` / `GPL-2.0-or-later AND MIT` | 1 each | isolated third-party-derived files |

> Every identifier above has a corresponding license text in `LICENSES/`
> (16 files), satisfying REUSE expectations. (See 1.4.)

## 1.3 — Copyright-holder inventory

Top holders (by `SPDX-FileCopyrightText` line count, excl. dynarmic):

| Holder (with year) | Lines |
|--------------------|-------|
| Volt Emulator Project (2024–2026) | ~1680 |
| yuzu Emulator Project (2017–2025) | ~2980 |
| Citra Emulator Project (2014–2020) | ~130 |
| Skyline / Torzu / Dolphin / Ryujinx / Android OSS / merryhime | ~70 |

**Lineage chain (Volt ← Volt ← Sudachi ← Yuzu ← Citra) is intact** in the
attribution record. No holder was dropped; new Volt/Volt headers were *prepended*,
preserving the historical block beneath — **except where corrupted (see Defects).**

## 1.4 — `LICENSES/` directory

16 license texts present and intact (none modified): `Apache-2.0`, `BSD-2-Clause`,
`BSD-3-Clause`, `BSL-1.0`, `CC-BY-4.0`, `CC-BY-SA-3.0`, `CC0-1.0`,
`GPL-2.0-or-later`, `GPL-3.0-or-later`, `LGPL-3.0-or-later`, `LLVM-exception`,
`MIT`, `MPL-2.0`, `Unlicense`, `WTFPL`, `Zlib`. Root `LICENSE.txt` = GNU GPL.
Every identifier observed in 1.1 maps to a file here. ✅

## 1.5 — Upstream attribution blocks

The dual-header convention (new Volt/Volt block on top, original upstream block
beneath) is the project's attribution mechanism and is preserved across the tree.
The About-dialog attribution (Volt/Sudachi/Yuzu) is tracked separately under
Phase 1 Step 8.4 and the rebrand audit (Tier B `.ui` strings).

---

## 🔴 Defects found (header corruption)

The audit surfaced three classes of damaged legal headers. These are the audit's
most important output: two were inherited from upstream, **one is a regression
introduced by Volt's own rebrand** and contradicts that commit's stated guarantee
("All license headers and copyright notices unchanged").

### D1 — Volt-introduced: falsified copyright holder (15 files) ⚠️ REGRESSION
Commit `4df3a6d7ea rebrand(cmake): rename project and variables` find-replaced
`yuzu` → `volt` **inside historical copyright lines**, leaving the original year
intact (e.g. `2018 volt Emulator Project`). Volt did not exist in 2018; this is
false attribution and reverses the lineage-preservation principle. Git confirms
each line read `yuzu Emulator Project` immediately before that commit.

Affected (line 4 of each, except where noted):
```
src/CMakeLists.txt                                   2018
src/audio_core/CMakeLists.txt                        2018
src/common/CMakeLists.txt                            2018  (also D2)
src/core/CMakeLists.txt                              2018
src/hid_core/CMakeLists.txt                          2018
src/input_common/CMakeLists.txt                      2018
src/shader_recompiler/CMakeLists.txt                 2018
src/tests/CMakeLists.txt                             2018
src/video_core/CMakeLists.txt                        2018
src/web_service/CMakeLists.txt                        2018
src/yuzu_cmd/CMakeLists.txt                          2018
src/network/CMakeLists.txt                           2022
src/frontend_common/CMakeLists.txt                   2023
src/android/app/src/main/jni/CMakeLists.txt          2023
src/video_core/host_shaders/StringShaderHeader.cmake 2020  (line 1)
```
A follow-up **repo-wide** sweep (beyond the initial `src/`-only scope) found the
same regression in 6 more files under `CMakeModules/`, all git-confirmed
originally `yuzu`:
```
CMakeModules/FindOpus.cmake          2022
CMakeModules/Findgamemode.cmake      2023  (line 1)
CMakeModules/Findlz4.cmake           2022
CMakeModules/Findzstd.cmake          2022
CMakeModules/GenerateSCMRev.cmake    2019
CMakeModules/WindowsCopyFiles.cmake  2018
```
**Remediation (no-build, git-verified exact restoration):** revert holder
`volt Emulator Project` → `yuzu Emulator Project` in all 21 files. Years
unchanged.

### D2 — Pre-existing upstream: merged / missing license line
`src/common/CMakeLists.txt:4` — the copyright text and the
`SPDX-License-Identifier:` key are on one line and the **value is on the next
line**, split as a comment:
```
# SPDX-FileCopyrightText: 2018 volt Emulator Project SPDX-License-Identifier:
# GPL-2.0-or-later
```
Present (with `yuzu`) before any Volt commit — an upstream REUSE defect.
**Remediation:** reconstruct two well-formed lines:
```
# SPDX-FileCopyrightText: 2018 yuzu Emulator Project
# SPDX-License-Identifier: GPL-2.0-or-later
```

### D3 — Pre-existing upstream: truncated SPDX identifier (2 files)
`src/core/hle/kernel/svc.cpp:4` and `src/core/hle/kernel/svc.h:4` end the
identifier as `GPL-2.0-or-late` (missing `r`) — an **invalid SPDX id**. Present
before Volt (`f761c3cfc3^`). These two files are **generated** ("DO NOT MODIFY
MANUALLY"); the truncation originates in the generator template at
`tools/svc_generator.py:473`, so fixing only the outputs would regress on the
next regen.
**Remediation:** `GPL-2.0-or-late` → `GPL-2.0-or-later` in both generated files
**and** in `tools/svc_generator.py:473` (root cause).

### D4 — Volt-introduced: falsified holder, Volt-origin (1 file) ⚠️ REGRESSION
`dist/dev.volt_emu.volt.xml:4` — `2025 volt Emulator Project`. Unlike D1 (a
`yuzu`→`volt` replacement from commit `4df3a6d7ea`), this came from a *separate*
rebrand commit `0ad45e6ce0 rebrand(platform)` that replaced `volt`→`volt`. Git
confirms the file's original holder (under its pre-rename name
`dist/dev.volt_emu.volt.xml`) was `2025 Volt Emulator Project`. The sibling
`dist/dev.volt_emu.volt.metainfo.xml` was checked and is **intact**
(`2025 Volt Emulator Project`).
**Remediation:** restore holder `volt Emulator Project` → `Volt Emulator Project`
(Volt, not yuzu — this is a genuine Volt-era 2025 file).

### D5 — Volt-introduced: mangled URLs (collateral, not legal) ⚠️
While verifying D4, a related defect surfaced in the **same file's content**: the
rebrand find-replace in `0ad45e6ce0` rewrote `https://volt-emu.dev/` →
`https://github.com/pushkarverse/volt-emu` **without the trailing slash**, gluing
the following path segment onto the host:
```
faq/help   https://volt-emu.dev/docs       -> ...volt-emudocs        (broken)
donation   https://volt-emu.dev/donations  -> ...volt-emudonations   (broken)
contribute https://git.volt-emu.dev/volt-emu/volt -> ...volt-emu/volt-emu/volt (dangling)
```
in `dist/dev.volt_emu.volt.metainfo.xml` (AppStream metadata, user-visible in
software stores). Unlike D1/D4 these have **no git-verifiable correct value** (the
originals were Volt URLs that should become Volt URLs, but no Volt docs/donation
pages exist). **Remediation (conservative, points only at content that exists in
this repo):** faq/help → `…/volt-emu/tree/dev/docs`; contribute →
`…/volt-emu/blob/dev/CONTRIBUTING.md`; donation → repo base. Other url entries
(homepage, bugtracker, translate, contact, vcs-browser) were already valid and
left unchanged. A repo-wide sweep confirms no other glue artifacts remain.

### D6 — Incomplete rebrand: stale `volt.exe` in Windows installer ⚠️
`dist/installer.nsi` still referenced `volt.exe` (8×) and `volt-cli.exe` (1×)
even though the build produces `volt.exe` (CMake `OUTPUT_NAME "volt"`, CI
`build.yml:48`) and `volt-cmd.exe` (target `volt-cmd`). The platform-rebrand
commit `0ad45e6ce0` updated `PRODUCT_NAME` but missed the executable references,
so the Start-Menu/Desktop shortcuts, "run after install", file-association
handler, registry App-Path/DisplayIcon, and uninstall cleanup all pointed at a
binary that is never installed — broken on every install. (ROADMAP Step 4.5.)
**Remediation:** `volt.exe`→`volt.exe` (8×), `volt-cli.exe`→`volt-cmd.exe` (1×).
The line-1 SPDX copyright (`Eden Emulator Project`) is a legal attribution and
was **left untouched**.

| Item | Origin | Risk | Action |
|------|--------|------|--------|
| 1.1–1.5 baseline | — | — | ✅ recorded (this doc) |
| D1 (21 falsified holders: 15 `src/` + 6 `CMakeModules/`) | Volt rebrand `4df3a6d7ea` | legal/attribution | ✅ **Repaired** — holder restored `volt`→`yuzu` |
| D2 (merged license line) | upstream | REUSE validity | ✅ **Repaired** — split into two well-formed lines |
| D3 (truncated id ×2 + generator) | upstream | invalid SPDX id | ✅ **Repaired** — `or-late`→`or-later` in both files + `svc_generator.py` |
| D4 (1 falsified holder, Volt-origin) | Volt rebrand `0ad45e6ce0` | legal/attribution | ✅ **Repaired** — holder restored `volt`→`Volt` |
| D5 (4 mangled URLs, collateral) | Volt rebrand `0ad45e6ce0` | content/UX | ✅ **Repaired** — point at existing repo content |
| D6 (9 stale `volt.exe` installer refs) | Volt rebrand `0ad45e6ce0` (incomplete) | broken installer | ✅ **Repaired** — `volt.exe`/`volt-cmd.exe` |

All three repairs are **no-build-safe** (comment/header lines only) and are
exact, git-verified restorations rather than new edits — they *restore* rather
than change compliance. Applied after explicit authorization, in the
accompanying `fix(legal):` commit. Step 8 comparator counts are unaffected
(holder text and identifier spelling change; line *counts* do not — D2 restores
one properly-split license line, which the count already credited).
