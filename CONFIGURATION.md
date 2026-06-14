# Volt Emulator — Configuration Reference

This document describes all configuration options available in Volt Emulator.

---

## Configuration File Location

| Platform | Path |
|----------|------|
| Windows  | `%APPDATA%\Volt Emulator\config\qt-config.ini` |
| Linux    | `~/.config/volt-emulator/qt-config.ini` |
| macOS    | `~/Library/Application Support/Volt Emulator/config/qt-config.ini` |
| Android  | Managed via in-app Settings UI |

The configuration file is INI format. Most settings are manageable through the **Settings** dialog in the UI. Manual editing is supported but changes require a restart to take effect.

---

## General Settings

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| `confirm_before_exit` | bool | true | Show confirmation dialog before closing |
| `pause_when_in_background` | bool | false | Pause emulation when window loses focus |
| `hide_mouse_on_idle` | bool | true | Hide mouse cursor after 2 seconds of inactivity |
| `screenshot_path` | path | `~/Pictures/Volt` | Directory for saved screenshots |
| `language` | string | `en` | UI language (ISO 639-1 code) |

---

## Game Library

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| `game_dirs` | string list | `[]` | List of directories to scan for games |
| `show_add_ons` | bool | true | Show DLC and updates in game list |
| `game_list_icon_size` | int | 64 | Size of game icons in list view (pixels) |
| `game_list_row_1` | string | `title_name` | Primary game list column |
| `game_list_row_2` | string | `title_id` | Secondary game list column |

---

## CPU Settings

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| `cpu_accuracy` | enum | `Auto` | CPU accuracy mode: `Auto`, `Accurate`, `Unsafe` |
| `cpu_backend` | enum | `Dynarmic` | CPU backend (currently only `Dynarmic`) |
| `multicore_enabled` | bool | true | Enable multi-core CPU emulation |
| `async_cpu_boost` | bool | false | Allow CPU to run ahead of GPU (reduces idle wait) |

### CPU Accuracy Modes

- **Auto** — Automatically selects best accuracy level for the running title
- **Accurate** — Full accuracy; some titles require this for correct behavior
- **Unsafe** — Relaxes accuracy constraints; may improve FPS at cost of correctness

---

## GPU / Renderer Settings

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| `renderer_backend` | enum | `Vulkan` | Rendering backend: `Vulkan`, `OpenGL` |
| `vulkan_device` | string | (first device) | Name of Vulkan device to use |
| `resolution_setup` | enum | `1X` | Internal resolution: `1X`, `2X`, `3X`, `4X` |
| `scaling_filter` | enum | `Bilinear` | Upscaling filter: `Nearest`, `Bilinear`, `Bicubic`, `Gaussian`, `ScaleForce` |
| `anti_aliasing` | enum | `None` | Anti-aliasing: `None`, `FXAA`, `SMAA` |
| `fullscreen_mode` | enum | `Borderless` | `Borderless`, `Exclusive` |
| `aspect_ratio` | enum | `Default_16_9` | `Default_16_9`, `Force_4_3`, `Stretch` |
| `use_vsync` | bool | true | Enable vertical sync |
| `max_fps` | int | 0 | Frame cap (0 = unlimited) |
| `use_asynchronous_gpu_emulation` | bool | true | Process GPU commands on a dedicated thread |
| `nvdec_emulation` | enum | `GPU` | Video decode: `Off`, `CPU`, `GPU` |
| `accelerate_astc` | bool | true | Decode ASTC textures on GPU |
| `async_shader_compilation` | bool | true | Compile shaders asynchronously (reduces stutter) |

### Resolution Multipliers

| Setting | Internal Resolution | Notes |
|---------|-------------------|-------|
| 1X | 720p / 1080p | Native Switch resolution |
| 2X | 1440p / 2160p | Good balance for 1440p monitors |
| 3X | 2160p+ | High-end GPUs only |
| 4X | 4K+ | Very demanding |

---

## Audio Settings

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| `sink_id` | string | `auto` | Audio output device ID (`auto` = system default) |
| `audio_output_engine` | string | `auto` | Backend: `auto`, `sdl2`, `wasapi`, `aaudio` |
| `enable_audio_stretching` | bool | true | Time-stretch audio when emulation runs slow |
| `output_volume` | float | 1.0 | Output volume multiplier (0.0 – 1.0) |

---

## Input / Controls

Input bindings are stored in a separate file:

| Platform | Path |
|----------|------|
| Windows  | `%APPDATA%\Volt Emulator\config\input_profiles\` |
| Linux    | `~/.config/volt-emulator/input_profiles/` |

Each `.ini` file in this directory represents one input profile.

### Controller Settings (per profile)

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| `player_0_type` | enum | `ProController` | Controller type: `ProController`, `DualJoycon`, `SingleJoyconLeft`, `SingleJoyconRight`, `Handheld`, `GameCube` |
| `player_0_connected` | bool | true | Whether this player slot is active |
| `player_0_vibration_enabled` | bool | true | Enable rumble for this player |
| `player_0_motion_enabled` | bool | true | Enable motion controls for this player |

Button mappings follow the pattern:
```ini
player_0_button_a=engine:sdl,guid:030000004c050000cc09000000006800,port:0,button:0
```

Use the **Controller Configuration** dialog to set these visually.

---

## System Settings

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| `use_docked_mode` | bool | true | Emulate Switch in docked mode (1080p, full CPU/GPU) |
| `current_user` | int | 0 | Active user profile slot (0–7) |
| `language_index` | int | 1 | System language: see Language Index table |
| `region_index` | int | 1 | System region: see Region Index table |
| `time_zone_index` | int | 0 | System timezone |
| `rng_seed_enabled` | bool | false | Use fixed RNG seed |
| `rng_seed` | u32 | 0 | Fixed RNG seed value (if enabled) |
| `custom_rtc_enabled` | bool | false | Use custom real-time clock |
| `custom_rtc` | u64 | 0 | Custom RTC value (Unix timestamp) |

### Language Index

| Value | Language |
|-------|----------|
| 0 | Japanese |
| 1 | English (American) |
| 2 | French |
| 3 | German |
| 4 | Italian |
| 5 | Spanish |
| 6 | Chinese (Simplified) |
| 7 | Korean |
| 8 | Dutch |
| 9 | Portuguese |
| 10 | Russian |
| 11 | Chinese (Traditional) |
| 12 | English (British) |
| 13 | French (Canadian) |
| 14 | Spanish (Latin American) |
| 15 | Chinese (Simplified, 2) |

---

## Data Storage Paths

| Data Type | Windows | Linux |
|-----------|---------|-------|
| Config | `%APPDATA%\Volt Emulator\config\` | `~/.config/volt-emulator/` |
| Saves | `%APPDATA%\Volt Emulator\nand\` | `~/.local/share/volt-emulator/nand/` |
| Shader Cache | `%APPDATA%\Volt Emulator\shader\` | `~/.cache/volt-emulator/shader/` |
| Log Files | `%APPDATA%\Volt Emulator\log\` | `~/.local/share/volt-emulator/log/` |
| Screenshots | `%USERPROFILE%\Pictures\Volt\` | `~/Pictures/Volt/` |
| Keys | `%APPDATA%\Volt Emulator\keys\` | `~/.local/share/volt-emulator/keys/` |

---

## Keys

Volt Emulator requires Nintendo Switch cryptographic keys to run commercial software. These are not provided and must be obtained from a user's own Switch console.

Place keys in the keys directory:

```
keys/
├── prod.keys       # Production keys (required for commercial games)
├── title.keys      # Title-specific keys (for specific encrypted titles)
└── dev.keys        # Development keys (optional, for homebrew)
```

Keys can also be installed via **File → Install Keys** in the UI.

---

## Per-Game Settings (Phase 5)

Per-game settings override global settings for a specific title. They are stored in:

```
config/custom/
└── <TITLE_ID_HEX>.ini
```

Not all settings support per-game override. The per-game settings system is planned for Phase 5.

---

## Log Level Configuration

Log verbosity can be configured per module in the Filter text box in the Log viewer:

```
*:Warning HW.GPU:Debug Service.AM:Trace
```

Format: `MODULE:LEVEL` separated by spaces. `*` matches all modules.

Levels: `Trace`, `Debug`, `Info`, `Warning`, `Error`, `Critical`
