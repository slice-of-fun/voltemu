<!--
SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
SPDX-License-Identifier: GPL-3.0-or-later
-->

# `core` — Emulation Core

`src/core` is the heart of the emulator: it emulates the Nintendo Switch's CPU,
memory, and operating system (Horizon OS) at a high level (HLE), and wires
together every other subsystem (GPU, audio, input) into a running system.

## The `System` class

`core.h` / `core.cpp` define `Core::System`, the single root facade that owns and
exposes every major subsystem. It uses the pimpl idiom (`System::Impl`) to keep
the public header small. A typical lifecycle:

```
System::Initialize()                  // bootstrap subsystems
System::Load(emu_window, path, ...)   // detect format, load executable, set up process
System::Run()                         // spawn CPU host threads, begin scheduling
System::Pause() / System::IsPaused()
System::Shutdown()
```

Things `System` owns include: `Kernel::KernelCore`, `CpuManager`,
`Core::Memory::Memory`, `Core::DeviceMemory`, `Tegra::GPU`, `AudioCore::AudioCore`,
`Timing::CoreTiming`, `HID::HIDCore`, `Service::SM::ServiceManager`, and the
file-system/content providers.

## Directory map

| Directory | Role |
|-----------|------|
| `arm/` | ARMv8 CPU emulation — Dynarmic JIT (64/32-bit), NCE (native code execution), exclusive monitor for atomics |
| `hle/` | High-Level Emulation of the Horizon kernel and OS services (see below) |
| `loader/` | Executable/ROM loaders: NRO, NSO, NCA, XCI (cartridge), NSP (package), KIP, deconstructed dirs |
| `file_sys/` | Virtual filesystem, content archives, AES-CTR/XTS crypto, ROM FS, save data, patching |
| `crypto/` | Cryptographic primitives and key management |
| `memory/` | Guest-memory tooling: cheat engine (DMNT VM), memory debugging |
| `debugger/` | GDB stub, breakpoints, watchpoints, symbol resolution |
| `frontend/` | UI bridges: emulator window abstraction and applet stubs (keyboard, controller, profile select, web, etc.) |
| `internal_network/` | Network emulation: socket proxies, WiFi scan, host network integration |
| `tools/` | Debug utilities: memory freezer, RenderDoc integration |

## Inside `hle/`

This is the largest part of `core`. Two pillars:

- **`hle/kernel/`** — the Horizon kernel. Core objects are `K`-prefixed:
  `KProcess`, `KThread`, `KScheduler` (+ `GlobalSchedulerContext`), `KPageTable` /
  `KMemoryManager`, synchronization (`KEvent`, `KSession`, `KConditionVariable`,
  `KAddressArbiter`), resource sharing (`KSharedMemory`, `KTransferMemory`,
  `KCodeMemory`), and quotas (`KResourceLimit`). System calls live under
  `hle/kernel/svc/`.
- **`hle/service/`** — dozens of OS service modules dispatched over IPC, each
  built on `ServiceFramework<T>`. Notable: `sm` (service manager), `nvdrv`
  (GPU submission), `vi` (display/compositor), `hid` (controllers), `am` (applets),
  `audio`, `filesystem`, `nifm`/`sockets` (network), `acc` (accounts),
  `set` (settings), `nfc`/`nfp` (amiibo).

## Key top-level files

| File | Purpose |
|------|---------|
| `core_timing.{h,cpp}` | Nanosecond-precision event scheduler driving GPU/audio/input sync |
| `cpu_manager.{h,cpp}` | Multi-core coordination via host threads + fibers; single/multi-core modes |
| `memory.{h,cpp}` | Guest virtual-memory access wrappers |
| `device_memory.{h,cpp}` | Guest physical DRAM layout and allocation |
| `perf_stats.{h,cpp}` | Frame timing: system FPS, game FPS, emulation speed |
| `reporter.{h,cpp}` | Crash/error reporting |

## Where to start

- Boot/run flow → `core.cpp` (`System::Impl`)
- Add/inspect a syscall → `hle/kernel/svc/`
- Add/inspect an OS service → `hle/service/<name>/`
- CPU backend behavior → `arm/`
- Game loading/format detection → `loader/`
