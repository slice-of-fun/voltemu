# Volt Emulator — Setup Guide

This guide walks you through setting up Volt Emulator from scratch, from installation to launching your first game.

---

## Table of Contents

- [Installation](#installation)
- [First Launch](#first-launch)
- [Providing Keys](#providing-keys)
- [Adding Games](#adding-games)
- [Controller Setup](#controller-setup)
- [Performance Tuning](#performance-tuning)
- [Docked vs Handheld Mode](#docked-vs-handheld-mode)
- [Save Data](#save-data)
- [Troubleshooting First Launch](#troubleshooting-first-launch)

---

## Installation

### Windows

1. Download `volt-windows-x64-vX.X.X.zip` from the [Releases](../../releases) page
2. Extract to a location of your choice (e.g. `C:\Emulators\Volt\`)
3. Run `volt.exe`

> Do not install into `C:\Program Files\` — write permissions are required for config and cache files.

### Linux

**AppImage (recommended):**
```bash
chmod +x Volt-x86_64.AppImage
./Volt-x86_64.AppImage
```

**Flatpak:**
```bash
flatpak install dev.volt_emu.volt
flatpak run dev.volt_emu.volt
```

**From package:**
```bash
# Debian/Ubuntu
sudo dpkg -i volt-emulator_x.x.x_amd64.deb

# Arch (AUR)
yay -S volt-emulator
```

### Android

1. Download `volt-android-vX.X.X.apk` from the Releases page
2. Enable "Install from unknown sources" in Android Settings → Security
3. Open the APK and install
4. Launch "Volt Emulator" from your app drawer

---

## First Launch

On first launch, Volt Emulator will:

1. Create configuration directories
2. Show a welcome screen
3. Prompt you to set up a game directory

You will see warnings about missing keys — this is expected. See [Providing Keys](#providing-keys).

---

## Providing Keys

Volt Emulator requires cryptographic keys to run commercial Switch software. These keys must come from **your own Nintendo Switch console**. Volt Emulator does not provide, distribute, or link to keys.

### Key Files Required

| File | Required For |
|------|-------------|
| `prod.keys` | All commercial games |
| `title.keys` | Some encrypted title-specific content |

### How to Obtain Keys

Use [Lockpick_RCM](https://github.com/shchmue/Lockpick_RCM) on your own Switch to dump your console's keys.

This process is beyond the scope of this guide. Resources are available in the Switch homebrew community.

### Where to Place Keys

| Platform | Keys Directory |
|----------|---------------|
| Windows | `%APPDATA%\Volt Emulator\keys\` |
| Linux | `~/.local/share/volt-emulator/keys/` |
| Android | Via **File → Install Keys** in the app |

Place `prod.keys` (and optionally `title.keys`) in the keys directory, then restart Volt Emulator.

You can also go to **File → Install Keys** and select your key file directly.

---

## Adding Games

Volt Emulator supports the following game formats:

| Format | Type | Notes |
|--------|------|-------|
| `.nsp` | eShop title / update / DLC | Most common digital format |
| `.xci` | Game cartridge dump | Full cartridge image |
| `.nca` | Raw content archive | Usually bundled inside NSP/XCI |
| `.nro` | Homebrew executable | No keys required |

### Add a Game Directory

1. Go to **File → Open Game Directory** (or **Add Game Directory** in Settings)
2. Select the folder containing your game files
3. Volt will scan and display all detected titles in the library

You can add multiple directories. Subdirectories are scanned recursively.

### Install Updates and DLC

1. Go to **File → Install Files (NSP/XCI)**
2. Select the update or DLC `.nsp` file
3. Volt will install it to the virtual NAND

After installation, updates and DLC are automatically applied when launching the base game.

---

## Controller Setup

### Automatic (recommended)

Plug in your controller before launching Volt. Most standard controllers are detected automatically:

- Xbox controllers (XInput)
- PlayStation controllers (via SDL2)
- Nintendo Switch Pro Controller (via SDL2 or native HID)
- Generic gamepads

### Manual Configuration

1. Go to **Settings → Controls**
2. Select a player slot (Player 1, Player 2, etc.)
3. Choose **Controller Type** (Pro Controller, Dual Joy-Con, Handheld, etc.)
4. Click each button and press the corresponding button on your physical controller

### Keyboard Controls (default)

| Switch Button | Keyboard Key |
|--------------|-------------|
| A | L |
| B | K |
| X | O |
| Y | I |
| D-Pad Up | Up Arrow |
| D-Pad Down | Down Arrow |
| D-Pad Left | Left Arrow |
| D-Pad Right | Right Arrow |
| L Stick | WASD |
| R Stick | TFGH |
| ZL | Q |
| ZR | E |
| L | 1 |
| R | 3 |
| + (Plus) | Return |
| - (Minus) | Backspace |
| Home | Backslash |
| Screenshot | F9 |

---

## Performance Tuning

### Recommended Settings for Best Performance

**Settings → Graphics:**
- Backend: **Vulkan** (required for good performance)
- Resolution: **1X** (native) — increase only if your GPU can handle it
- Async shader compilation: **Enabled**
- NVDEC emulation: **GPU**

**Settings → CPU:**
- Multicore: **Enabled**
- Accuracy: **Auto**

**Settings → System:**
- Docked mode: **Enabled** (for most games)

### GPU Driver Recommendations

| GPU | Recommended Driver |
|-----|--------------------|
| NVIDIA (Windows) | 555.85+ |
| NVIDIA (Linux) | 550+ (proprietary) |
| AMD (Windows) | 24.1+ (Adrenalin) |
| AMD (Linux) | Mesa 24.0+ (RADV) |
| Intel Arc (Windows) | 31.0.101.5330+ |
| Intel Arc (Linux) | Mesa 24.1+ (ANV) |

### Shader Cache

On first play, games will stutter during shader compilation. This is normal. A shader cache is built automatically and subsequent sessions will be smoother.

The shader cache is stored in:
- Windows: `%APPDATA%\Volt Emulator\shader\`
- Linux: `~/.cache/volt-emulator/shader/`

---

## Docked vs Handheld Mode

| Mode | Resolution | Performance |
|------|-----------|-------------|
| Docked | Up to 1080p | Higher GPU clock (better graphics) |
| Handheld | Up to 720p | Lower GPU clock (some games run differently) |

Change via **Settings → System → Docked Mode**.

Most games are designed primarily for docked mode. Some games with adaptive quality settings will look noticeably better in docked mode.

---

## Save Data

### Location

Save data is stored in a virtual NAND:

| Platform | Save Directory |
|----------|---------------|
| Windows | `%APPDATA%\Volt Emulator\nand\user\save\` |
| Linux | `~/.local/share/volt-emulator/nand/user/save\` |

### Backup Saves

To backup your saves:
1. Go to **File → Open Volt Folder**
2. Navigate to `nand/user/save/`
3. Copy the save directory for the title you want to backup

### Import Saves

Volt Emulator is compatible with save data format from Eden and Yuzu (same NAND structure). You can copy save directories between emulators.

---

## Troubleshooting First Launch

### "Missing keys" error
→ See [Providing Keys](#providing-keys)

### Game shows black screen
→ Ensure keys are installed correctly  
→ Try toggling **Docked Mode**  
→ Try switching to **Accurate** CPU mode  
→ Check the log for errors (**View → Log**)

### Very low FPS / poor performance
→ Enable **Async Shader Compilation**  
→ Set Resolution to **1X**  
→ Update GPU drivers  
→ Check CPU accuracy is set to **Auto**, not **Accurate**

### Controller not detected
→ Plug in controller before launching Volt  
→ Check **Settings → Controls** to configure manually  
→ Try a different USB port or Bluetooth pairing

### Game crashes on startup
→ Check for a required firmware version (some games need Switch firmware files)  
→ Check the log for specific error messages  
→ Search the issue tracker for known issues with this title

### Log Location

Logs are saved to:
- Windows: `%APPDATA%\Volt Emulator\log\volt_log.txt`
- Linux: `~/.local/share/volt-emulator/log/volt_log.txt`

Always include the log when reporting issues.
