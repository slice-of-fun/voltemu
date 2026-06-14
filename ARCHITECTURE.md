# Volt Emulator — Architecture Overview

This document describes the high-level architecture of Volt Emulator: how subsystems are organized, how they interact, and the design principles guiding the codebase.

---

## Design Principles

1. **Correctness first** — correct emulation behavior before optimization
2. **Modular boundaries** — subsystems own their state; no hidden global dependencies
3. **Clear ownership** — every resource has a clear owner; prefer RAII
4. **Minimal coupling** — modules communicate through defined interfaces, not internals
5. **Testability** — subsystems are designed to be independently unit-testable
6. **Performance awareness** — hot paths are profiled, not guessed

---

## Top-Level Structure

```
src/
├── core/              # System emulation (CPU, memory, OS services, filesystem)
├── video_core/        # GPU emulation (Vulkan backend, shader compiler, display)
├── audio_core/        # Audio emulation (DSP, audio renderer, output backend)
├── input/             # Input emulation (controller, keyboard, mouse, motion)
├── frontend/          # UI layer (Qt6 frontend, game library, settings)
├── android/           # Android-specific frontend and JNI bridge
├── common/            # Shared utilities (logging, math, containers, threading)
├── networking/        # Network emulation (LDN, LAN play)
└── experimental/      # Unstable or in-development features
```

---

## Subsystem Map

```
┌─────────────────────────────────────────────────────┐
│                    Frontend (Qt6)                    │
│          Game Library · Settings · Debug UI          │
└───────────────────────┬─────────────────────────────┘
                        │
              ┌─────────▼──────────┐
              │   Emulation Core    │
              │  (System / Kernel)  │
              └──┬──────┬──────┬───┘
                 │      │      │
        ┌────────▼──┐ ┌─▼────┐ ┌▼───────────┐
        │ CPU Core  │ │Memory│ │ OS Services │
        │(Dynarmic) │ │ Map  │ │  (HLE/LLE) │
        └───────────┘ └──────┘ └────────────┘
                 │      │      │
        ┌────────▼──────▼──────▼────────────┐
        │         Hardware Abstraction        │
        └──────┬──────────┬──────────┬───────┘
               │          │          │
        ┌──────▼──┐ ┌─────▼──┐ ┌────▼─────┐
        │  Video  │ │ Audio  │ │  Input   │
        │  Core   │ │  Core  │ │  System  │
        │(Vulkan) │ │ (DSP)  │ │(HID/SDL) │
        └─────────┘ └────────┘ └──────────┘
```

---

## Core (`src/core/`)

The Core subsystem is the heart of Volt Emulator. It manages:

### ARM CPU Emulation
- **Backend:** [Dynarmic](https://github.com/merryhime/dynarmic) — a JIT recompiler for ARM64
- Each virtual CPU core maps to a Dynarmic `A64::Jit` instance
- Dynarmic callbacks handle memory reads/writes, SVC dispatch, and undefined instruction handling
- Multi-core emulation uses one host thread per emulated core

### Memory Management
- Emulates the Switch's memory layout: DRAM regions, MMIO, code memory, stack
- Uses host virtual memory (`mmap`/`VirtualAlloc`) for the emulated address space
- Physical memory tracked through a page table structure
- Memory permissions enforced (read/write/execute) for correctness

### Kernel / OS Services (HLE)
- Implements Nintendo Switch kernel services at the HLE (High-Level Emulation) level
- Services implemented via IPC message dispatch
- Key services: filesystem (fsp-srv), display (vi), audio (audren), input (hid), network (ldn)
- NPDM (title metadata) parsing for title identification and permission checking

### Loader
- Supports: NSP, XCI, NCA, NRO (homebrew), NSO
- Handles title key decryption (requires user-provided keys)
- Resolves and maps executable sections into the emulated address space

### Filesystem
- Emulates Switch filesystem hierarchy
- Supports: SD card image, installed titles, saves, DLC
- Content metadata (CNMT) parsing for update/DLC management

---

## Video Core (`src/video_core/`)

### Maxwell GPU Emulation
- Emulates the NVIDIA Tegra X1's Maxwell/Pascal GPU
- Processes GPU commands submitted via GPFIFO channels
- Rasterizer implementation dispatches draw calls to the host GPU

### Vulkan Renderer
- Primary rendering backend — required for acceptable performance
- Key components:
  - `VKDevice` — device selection, feature negotiation
  - `VKSwapchain` — presents frames to the OS window
  - `VKPipeline` — PSO management with cache
  - `VKDescriptorPool` — descriptor management
  - `VKStagingBuffer` — host→device data upload
  - `VKQueryPool` — GPU timing

### Shader Compilation Pipeline
```
Maxwell Shader Bytecode
        │
        ▼
   Shader Decoder
        │
        ▼
   Shader IR (SSA form)
        │
        ▼
   SPIRV Emitter (via SPIRV-Cross / glslang)
        │
        ▼
  Vulkan Shader Module
        │
        ▼
  Pipeline State Object
```

- Async shader compilation runs on a thread pool to reduce stutter
- Pipeline cache persisted to disk for warm-start performance
- Shader cache keyed by shader hash + GPU state hash

### Texture Cache
- Manages emulated GPU texture → host GPU texture mapping
- Handles format conversions (e.g., BC7 → RGBA)
- Tracks GPU texture writes for cache invalidation

---

## Audio Core (`src/audio_core/`)

- Emulates the Switch's audio renderer (AudioRenderer service)
- DSP command list processor
- Output backends: SDL2 (all platforms), AAudio (Android), WASAPI (Windows)
- Resampling and sample rate conversion
- Effect processing (reverb, delay, mix)

---

## Input (`src/input/`)

- Emulates Nintendo Switch HID (Human Interface Device) service
- Input backends: SDL2 (gamepad), native keyboard/mouse
- Supports: Pro Controller, Joy-Con pair, Handheld mode
- Motion/gyroscope emulation
- Touch input emulation (Android: passthrough; desktop: mouse)
- Input profiles: per-game binding sets

---

## Frontend — Qt6 (`src/frontend/`)

- Primary desktop UI using Qt6
- Game library browser
- Settings management
- In-game overlay (FPS, frame time)
- Debug windows: log viewer, GPU debug, CPU debug
- Controller configuration UI
- Software keyboard (for games that require it)

### Key Frontend Design Rules
- Frontend never calls emulation core directly from the UI thread
- All emulation state access goes through a thread-safe `EmulationSession` interface
- Qt signals/slots used for async updates from emulation thread to UI

---

## Android Frontend (`src/android/`)

- Android-specific UI using Jetpack Compose
- JNI bridge between Java/Kotlin and C++ emulation core
- Surface management for Vulkan rendering on Android
- Android-specific input handling (touch, gamepad via Android Input API)
- Integration with Android storage access framework

---

## Common Utilities (`src/common/`)

Shared infrastructure used across all subsystems:

| Component | Purpose |
|-----------|---------|
| `logging/` | Structured logging with level and module filtering |
| `math/` | Fixed-point, vector, matrix math utilities |
| `memory/` | Arena allocator, pool allocator, ring buffer |
| `thread/` | Thread pool, worker queue, synchronized primitives |
| `file/` | Cross-platform file I/O, path utilities |
| `crypto/` | AES, SHA, RSA wrappers (via mbedtls) |
| `container/` | Intrusive list, LRU cache, ring buffer |
| `string/` | UTF-8/UTF-16 conversion, string utilities |
| `time/` | High-resolution timer, wall clock |

---

## Networking (`src/networking/`)

- LDN (Local Delivery Network) emulation — Switch local multiplayer
- LAN play via UDP tunneling
- Network interface abstraction for host network access

---

## Threading Model

```
Main Thread (Qt UI)
    │
    ├── EmulationThread        ← runs CPU emulation
    │       ├── CPUCore[0]
    │       ├── CPUCore[1]
    │       └── CPUCore[2..N]
    │
    ├── GPUThread              ← processes GPU command lists
    │
    ├── ShaderCompilePool      ← async shader compilation workers
    │       └── Worker[0..N]
    │
    ├── AudioThread            ← audio DSP + output
    │
    └── IOThread               ← filesystem and IPC I/O
```

**Cross-thread communication rules:**
- Emulation core → UI: via posted Qt events / signals
- UI → Emulation core: via `EmulationSession` command queue (lock-free MPSC)
- GPU thread ↔ CPU thread: via command ring buffer

---

## Data Flow — Frame Rendering

```
1. CPU executes game code
        │
2. Game submits GPU commands via GPFIFO
        │
3. GPUThread processes command list
        │
4. Rasterizer calls Vulkan renderer
        │
5. Shader compiler resolves pipeline (cache hit or async compile)
        │
6. Vulkan draw calls execute on host GPU
        │
7. Frame presented via VKSwapchain → OS window / Android Surface
        │
8. Frame timing synchronized to target refresh rate
```

---

## Key Interfaces

### `EmulationSession`
The primary interface between the UI and the emulation core. All UI interactions with the emulator go through this class.

```cpp
class EmulationSession {
public:
    void LoadGame(const std::filesystem::path& path);
    void Pause();
    void Resume();
    void Stop();
    void SaveState(u32 slot);
    void LoadState(u32 slot);
    EmulationState GetState() const;
    PerformanceStats GetStats() const;
};
```

### `VideoCore::RendererBase`
Abstract renderer interface allowing multiple backends (Vulkan is current primary):

```cpp
class RendererBase {
public:
    virtual void SwapBuffers(const Tegra::FramebufferConfig* framebuffer) = 0;
    virtual void ReportDriverStats() = 0;
    virtual RasterizerInterface* GetRasterizer() = 0;
};
```

---

## Adding a New Subsystem

1. Create directory under `src/` with descriptive name
2. Write a `README.md` in the directory explaining its purpose
3. Create a `CMakeLists.txt` defining the module as a library target
4. Define a clear public API in header files
5. Keep implementation details in `.cpp` files
6. Add unit tests in `src/tests/<subsystem>/`
7. Document the subsystem in `docs/architecture/`
8. Wire into the main `EmulationSession` if needed

---

## Performance-Critical Paths

The following are hot paths — changes here require profiling justification:

- Dynarmic JIT dispatch loop
- GPU command list processor
- Texture cache lookup (called per draw call)
- Shader pipeline lookup (called per draw call)
- Audio renderer DSP loop
- Input polling (called every frame)

Any PR modifying these paths must include before/after profiling data.
