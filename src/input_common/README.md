<!--
SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
SPDX-License-Identifier: GPL-3.0-or-later
-->

# `input_common` — Input Abstraction

`src/input_common` is the input layer: it receives input from diverse physical
sources (keyboard, mouse, gamepads via SDL, GameCube adapter, Joy-Cons over HID,
UDP/Cemuhook, touch, TAS playback) and exposes it as emulated Switch controller
inputs (buttons, analog sticks, motion, NFC, rumble, LEDs).

## Architecture

Three layers:

- **`InputSubsystem`** (`main.{h,cpp}`) — the facade. `Initialize()` constructs and
  owns every driver (engine), holds the `MappingFactory`, enumerates devices via
  `GetInputDevices()`, and coordinates configuration mode
  (`BeginMapping()` / `StopMapping()`) across all engines.
- **`InputEngine`** (`input_engine.{h,cpp}`) — the base driver abstraction. Each
  engine keeps a thread-safe `PadIdentifier → ControllerData` map and exposes
  `SetButton()` / `SetAxis()` / `SetMotion()` for subclasses to push hardware
  events. Registered callbacks fire on change; in configuration mode events are
  routed to the `MappingFactory` instead of updating live state.
- **Factories** (`input_poller.h`, `input_mapping.h`) — `InputFactory` /
  `OutputFactory` build input/output devices from a `ParamPackage` spec;
  `MappingFactory` turns raw events captured during remapping into a serializable
  `ParamPackage`.

### Data flow

```
hardware event → driver (e.g. SDLDriver) → engine.SetButton(id, btn, state)
   → ControllerData updated → callbacks fire
   → emulated controller reads via engine.GetButton(id, btn)
```

A `PadIdentifier` = device `UUID` + `port` + `pad`, so multiple identical
controllers can coexist.

## Drivers (`drivers/`)

| Driver | Purpose |
|--------|---------|
| `keyboard` | Desktop keyboard → buttons |
| `mouse` | Mouse position / clicks / motion |
| `sdl_driver` | Cross-platform gamepads via SDL3 (gated on `HAVE_SDL3`) |
| `joycon` | Direct Joy-Con over HID/Bluetooth (full protocol: IRS, NFC, ringcon) |
| `gc_adapter` | GameCube controller USB adapter via libusb (gated on `ENABLE_LIBUSB`) |
| `udp_client` | Cemuhook UDP network controllers (phone/PC motion) |
| `touch_screen` | Emulated touchscreen |
| `tas_input` | Tool-Assisted Speedrun script playback |
| `camera` | Camera image input (QR/photo) |
| `virtual_gamepad` | Always-present software controller |
| `virtual_amiibo` | Virtual amiibo NFC tag emulation |
| `android` | Android native input via JNI (gated on `__ANDROID__`) |

## Helpers (`helpers/`)

| Helper | Purpose |
|--------|---------|
| `stick_from_buttons` | Synthesize an analog stick from 4 buttons + modifier |
| `touch_from_buttons` | Map buttons to touch coordinates |
| `joycon_driver` | State machine + HID transport for raw Joy-Con comms |
| `joycon_protocol/` | Sub-protocols: calibration, common, IRS, NFC, ringcon, rumble, poller, types |
| `udp_protocol` | Cemuhook packet codec (structure, CRC, serialization) |

## Where to start

- Add a new input source → subclass `InputEngine` under `drivers/`, register in `main.cpp`
- Remapping/config behavior → `input_mapping.{h,cpp}` + engine configuration mode
- Joy-Con protocol work → `helpers/joycon_protocol/`
