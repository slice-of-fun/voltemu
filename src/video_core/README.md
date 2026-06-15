<!--
SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
SPDX-License-Identifier: GPL-3.0-or-later
-->

# `video_core` — GPU Emulation

`src/video_core` emulates the Switch's **NVIDIA Tegra X1 (Maxwell) GPU**. It
receives GPU command buffers from the guest, decodes them into engine method
calls, and translates the resulting draw/compute work onto a host graphics API
(Vulkan, OpenGL, or a null backend).

## The `GPU` class

`gpu.h` / `gpu.cpp` define `Tegra::GPU`, the top-level orchestrator. It owns the
GPU channels, the command pushers, the GPU MMU (`MemoryManager`), the `Host1x`
block (video decode + display), and the active `VideoCore::RendererBase` backend.
Key entry points: `BindRenderer()`, `PushGPUEntries()`, `FlushCommands()`, the
cache-coherence hooks (`FlushRegion`/`InvalidateRegion`, `OnCPURead`/`OnCPUWrite`),
and channel management (`AllocateChannel`/`BindChannel`).

## Command flow

The guest never issues "draw calls" directly — it writes *methods* (GPU register
writes) into a command buffer:

```
Guest command buffer
   → GPU::PushGPUEntries()         (routed per channel)
   → DmaPusher                     decode method/subchannel/args
   → Engines (Tegra::Engines)      Maxwell3D / KeplerCompute / Fermi2D / MaxwellDMA
   → RasterizerInterface           backend-agnostic Draw / DispatchCompute / Clear / fences
   → Renderer backend              Vulkan | OpenGL | Null  → host GPU
```

`Host1x` runs a parallel path via `CDmaPusher` for NVDEC (video decode) and VIC
(composition). Async GPU work is driven by `gpu_thread`. CPU↔GPU synchronization
is handled by the `FenceManager` and Host1x syncpoints.

## Directory map

| Path | Role |
|------|------|
| `engines/` | GPU engines: `maxwell_3d` (3D), `kepler_compute` (compute), `fermi_2d` (2D blit), `maxwell_dma` / `kepler_memory` (copies); plus `puller`, `draw_manager`, macro execution |
| `buffer_cache/` | Tracks/dedups host GPU buffers with dirty-region tracking |
| `texture_cache/` | Tracks/dedups images & views; guest↔host format translation, swizzle |
| `query_cache/` | Occlusion / primitive / transform-feedback counter queries |
| `control/` | Per-channel GPU state (`channel_state`) and channel scheduling |
| `host1x/` | NVDEC (H.264/VP8/VP9 via FFmpeg), VIC compositor, syncpoint manager |
| `renderer_vulkan/` | Primary backend — `RendererVulkan` + `vk_*` (scheduler, pipeline cache, caches) |
| `renderer_opengl/` | Fallback backend — `RendererOpenGL` + `gl_*` |
| `renderer_null/` | No-op backend for headless/testing |
| `vulkan_common/` | Shared Vulkan device/instance/allocator (VMA) plumbing |
| `textures/` | Guest texture decode: ASTC, BCn, deswizzle (multithreaded) |
| `host_shaders/` | GLSL/compute utility shaders compiled into the build |
| `macro/` | Maxwell macro interpreter/JIT for non-incrementing method groups |

## Key files

| File | Purpose |
|------|---------|
| `dma_pusher.{h,cpp}` | Parses guest command buffers; routes methods to engine subchannels |
| `memory_manager.{h,cpp}` | GPU MMU — virtual↔physical translation, page tables |
| `rasterizer_interface.h` | Pure-virtual contract every renderer backend implements |
| `fence_manager.h` | Async GPU completion tracking + cache flush on fence |
| `gpu_thread.{h,cpp}` | Async GPU worker thread |
| `fsr.{h,cpp}` | FidelityFX Super Resolution upscaling |

## Renderer backends

- **Vulkan** (`renderer_vulkan/`, primary) — `Vulkan::RendererVulkan`. Async
  pipeline compilation, timeline semaphores, descriptor pooling, present-stage
  post-processing (FSR/FXAA/SMAA).
- **OpenGL** (`renderer_opengl/`, fallback) — `OpenGL::RendererOpenGL`. Simpler,
  uses Direct State Access.
- **Null** (`renderer_null/`) — stubs all rendering; for headless runs.

Both real backends implement `RasterizerInterface`, so engine and cache code stays
backend-agnostic.

## Where to start

- Add a GPU method/feature → `engines/maxwell_3d.cpp`
- Backend-specific draw translation → `renderer_vulkan/vk_rasterizer.cpp` / `renderer_opengl/gl_rasterizer.cpp`
- Texture format support → `texture_cache/format_lookup_table.cpp`, `textures/`
- Address translation issues → `memory_manager.cpp`

> **Vendored note:** `src/video_core/smaa_area_tex.h` and `smaa_search_tex.h` are
> third-party MIT data files (SMAA, © Jimenez et al.) and must not be reformatted
> or modified as part of cleanup work.
