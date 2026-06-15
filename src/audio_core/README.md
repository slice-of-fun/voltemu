<!--
SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
SPDX-License-Identifier: GPL-3.0-or-later
-->

# `audio_core` — Audio Emulation

`src/audio_core` emulates the Switch's audio system: streaming playback and
capture, the audio renderer (DSP), and the emulated audio firmware (ADSP). It
produces PCM that is handed to a platform output backend (sink).

## The `AudioCore` class

`audio_core.h` / `audio_core.cpp` define `AudioCore::AudioCore`, which owns:

- `AudioManager` — event-driven coordinator that signals buffer completion
- `Sink::Sink output_sink` and `Sink::Sink input_sink` — backend audio I/O
- `ADSP::ADSP` — emulated audio firmware (audio renderer + Opus decoder)

## Three audio domains

| Domain | Manager | Sessions | Lifecycle |
|--------|---------|----------|-----------|
| **Audio-out** (playback) | `AudioOut::Manager` | up to 12 | game appends buffers → backend consumes → buffer event fires |
| **Audio-in** (capture) | `AudioIn::Manager` | up to 4 | game appends buffers → backend fills → buffer event fires |
| **Audio renderer** (DSP/synth) | `Renderer::Manager` | `MaxRendererSessions` | game submits params + command buffer → system builds command list → ADSP processes → sink |

All three rely on the shared `AudioManager` for event-driven callbacks. Each domain
splits into a thin service-facing wrapper (`In` / `Out`) and a `System` that owns
buffer/device state.

## ADSP

`adsp/` emulates the 32-bit audio sysmodule firmware. It hosts two apps:

- **AudioRenderer** (`adsp/apps/audio_renderer/`) — processes the command lists
  built by `renderer/`, performing voice mixing and effects, then emits final
  samples to the output sink.
- **OpusDecoder** (`adsp/apps/opus/`) — libopus-based decompression of Opus audio
  used by game assets.

Host ↔ ADSP communication uses mailboxes plus shared memory.

## Sink backends

The `Sink::Sink` interface abstracts platform audio I/O (conditionally compiled):

| Backend | Availability |
|---------|--------------|
| `cubeb_sink` | Desktop (`ENABLE_CUBEB`) |
| `sdl3_sink` | Non-Android fallback |
| `oboe_sink` | Android |
| `null_sink` | Always available (no-op) |

## Directory map

| Path | Role |
|------|------|
| `adsp/` | Emulated audio firmware kernel + apps (audio renderer, Opus) |
| `renderer/` | Host-side renderer: builds DSP command lists; voice/mix/effect/sink/splitter contexts |
| `renderer/command/` | Command encoding and DSP primitives (data sources, effects, mix, sinks) |
| `renderer/effect/` | Effect implementations (reverb, compressor, biquad, delay, I3DL2…) |
| `in/`, `out/` | Audio-in / audio-out `System` + buffer management |
| `opus/` | Hardware Opus decoder interface (`HardwareOpus`, decoder manager) |
| `sink/` | Sink interface + backends (cubeb, SDL3, oboe, null) |
| `device/` | Device session & buffer management |
| `common/` | Shared types: renderer params, wave buffer, workbuffer allocator |

## Key files

| File | Purpose |
|------|---------|
| `audio_manager.{h,cpp}` | Event coordinator thread; signals in/out on buffer completion |
| `audio_out_manager.{h,cpp}` | Up to 12 playback sessions |
| `audio_in_manager.{h,cpp}` | Up to 4 capture sessions |
| `audio_render_manager.{h,cpp}` | Coordinates renderer `System` instances |
| `audio_event.{h,cpp}` | Kernel event types for buffer signalling |

## Where to start

- Output not working → `sink/` backend for your platform
- Effects / mixing → `renderer/command/`, `renderer/effect/`
- DSP firmware behavior → `adsp/apps/audio_renderer/`
- Service-level audio API → see `core/hle/service/audio/`
