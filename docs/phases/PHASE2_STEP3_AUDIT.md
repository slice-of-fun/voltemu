<!-- SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project -->
<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Phase 2 — Step 3 Audit: Module Boundaries & Dependencies

This document records a **no-build inspection** of architectural coupling between
the four major subsystems (`src/core`, `src/video_core`, `src/audio_core`,
`src/input_common`) and shared `src/common`. Mirroring Steps 1–2, edits that
require a compiler to verify safely (include removal can break transitive
consumers; circular-dependency surgery changes link/compile order) are **audited
and deferred** to a build environment. Findings below are evidence, not applied
changes.

## 3.1 Include Reductions

### Cross-subsystem include counts (grep of `#include "<subsystem>/`)

| Direction | Count | Files | Notes |
|-----------|-------|-------|-------|
| `video_core` → `core` | 58 | 40 | Mostly `core/core.h` (16), `frontend/emu_window.h` (12), `frontend/graphics_context.h` (8) |
| `core` → `video_core` | 37 | 20 | Mostly `host1x/host1x.h` (12), `gpu.h` (8), `renderer_base.h` (6) |
| `audio_core` → `core` | 76 | 41 | Mostly `core/core.h` (25), `hle/service/audio/errors.h` (16), `core_timing.h` (11) |
| `core` → `audio_core` | 20 | 15 | Mostly `audio_core/audio_core.h` (4) + audio service managers |
| `input_common` ↔ `core` | 0 / 0 | — | **Clean** — no coupling either direction |
| `video_core` ↔ `audio_core` | 0 / 0 | — | **Clean** — no coupling either direction |

**Circular coupling:** `core ↔ video_core` is genuinely bidirectional (58/37).
This is structural — the GPU memory manager and the `System` facade coordinate
directly — and is a common pattern in GPU-heavy emulators. `audio_core → core`
is heavy (76) but largely one-directional (only 20 back), so it is asymmetric
coupling rather than a tight cycle. `input_common` and `audio_core↔video_core`
are already cleanly isolated.

> **Deferred:** Untangling the `core ↔ video_core` cycle is a behavior-adjacent
> refactor (changes compile/link order, risks ODR and build breakage). It
> requires a build + link to validate and is out of scope for a no-build pass.

### Facade header hygiene (forward-decl vs. concrete includes, pimpl)

| Facade | Pimpl? | Forward decls | Concrete includes | Verdict |
|--------|--------|---------------|-------------------|---------|
| `core/core.h` | ✅ | Extensive (Kernel, FileSys, Service, Tegra, VideoCore, AudioCore…) | `vfs_types.h`, `kernel_helpers.h`, `os/event.h`, `common_types.h` | Well-designed; includes are for return/param types |
| `video_core/gpu.h` | ✅ (`mutable`) | DmaPusher, Maxwell3D, KeplerCompute, ChannelState, Host1x, MemoryManager | `nvdrv/nvdata.h`, `cdma_pusher.h`, `framebuffer_config.h`, `rasterizer_download_area.h` | Good; **one suspected unused include** (below) |
| `audio_core/audio_core.h` | ❌ | `Core::System` only | `adsp/adsp.h`, `audio_manager.h`, `sink/sink.h` | Concrete `unique_ptr` members force full types; **pimpl candidate** |
| `input_common/main.h` | ✅ | ParamPackage, ButtonNames, Settings enums, all driver classes | none (only `<ankerl/unordered_dense.h>` + STL) | Excellent isolation |

### Candidate: suspected unused include (DEFERRED — needs build)

- **`src/video_core/gpu.h:11` → `#include "common/bit_field.h"`**
  Verified by grep: the tokens `BitField` / `bit_field` appear **only** on the
  include line itself; no `BitField<>` member or type is used anywhere in the
  header.
  **Why deferred, not removed:** `gpu.h` is included widely; some `.cpp` may
  transitively rely on `bit_field.h` being pulled in via `gpu.h`. Removing it
  without a compile risks breaking those consumers (IWYU breakage). Safe to drop
  only with a build to confirm.

### 3.1 checklist status
- [ ] Audit and remove unnecessary `#include` directives — **audited**; one
  concrete candidate found (`gpu.h:11`), removal deferred to a build pass.
- [ ] Resolve circular dependencies — **audited**; `core ↔ video_core` is the
  only true cycle, structural, deferred (build/link-risk).
- [ ] Move implementation details from headers to `.cpp` — three of four facades
  already use pimpl; `audio_core.h` is the lone candidate (see 3.2).

## 3.2 Public API Boundaries

- The four subsystems already expose **facade classes** (`System`, `GPU`,
  `AudioCore`, `InputSubsystem`) that act as their public entry points; the
  Step 4.1 READMEs document each as the intended boundary.
- **Pimpl** already enforces an opaque boundary for `core`, `video_core`, and
  `input_common`. `audio_core` is the exception — `AudioCore` exposes concrete
  `unique_ptr<AudioManager/Sink/ADSP>` members, coupling callers to those types.
  Introducing a pimpl there is the clearest API-boundary improvement, but it is
  a real code change requiring a build to verify.

### 3.2 checklist status
- [ ] Define strict public APIs — facades already serve this role (documented in
  Step 4.1 READMEs); formalizing is a design task, not a no-build edit.
- [ ] Introduce interface headers — deferred; would need build verification.

## Summary

| Item | Risk | Disposition |
|------|------|-------------|
| `gpu.h` unused `bit_field.h` include | Low-med (transitive) | Documented; **deferred to build** |
| `core ↔ video_core` cycle | High (structural) | Documented; deferred |
| `audio_core.h` pimpl adoption | Med (real refactor) | Documented; deferred |
| `input_common` isolation | — | Already clean, no action |
| `video_core ↔ audio_core` isolation | — | Already clean, no action |

**Net no-build edits this step: none.** Step 3 is an audit-only pass; every
candidate is build-sensitive and recorded here for execution in a compile
environment. This is consistent with Step 2.1 (static-analysis removals
deferred) and Step 4.2 (tests written but unrun).
