# ⚡ Volt Emulator

> **Fast. Stable. Open.**

Volt Emulator is an open-source Nintendo Switch emulator focused on performance, stability, and modern user experience. It is built upon the Eden codebase and traces its upstream lineage through Eden → Sudachi → Yuzu.

---

## Table of Contents

- [About](#about)
- [Status](#status)
- [Features](#features)
- [Screenshots](#screenshots)
- [Getting Started](#getting-started)
- [Building from Source](#building-from-source)
- [Configuration](#configuration)
- [Contributing](#contributing)
- [Roadmap](#roadmap)
- [Legal & Attribution](#legal--attribution)
- [License](#license)

---

## About

Volt Emulator began as a fork of Eden with the goal of evolving into a fully independent emulator project. The mission is to build a high-performance, modern, open-source Nintendo Switch emulator that prioritizes:

- **Performance** — optimized CPU, GPU, and memory paths
- **Stability** — rigorous testing and careful engineering
- **Compatibility** — broad game support across titles
- **Maintainability** — clean architecture and documented code
- **User Experience** — modern, intuitive interface on all platforms

### Upstream Lineage

```
Volt Emulator  ←  Eden  ←  Sudachi  ←  Yuzu
```

All upstream contributions, license obligations, and copyright notices are preserved in full.

---

## Status

| Platform | Build | Status |
|----------|-------|--------|
| Windows  | MSVC / MinGW | 🟡 In Progress |
| Linux    | GCC / Clang  | 🟡 In Progress |
| Android  | NDK          | 🟡 In Progress |

> Volt Emulator is currently in Phase 1 (Rebranding & Foundation). Active development is ongoing.

---

## Features

### Current (Inherited from Eden)
- Nintendo Switch emulation via HLE (High-Level Emulation)
- Vulkan and OpenGL rendering backends
- ARM64 CPU emulation via Dynarmic
- Audio emulation
- Controller support (keyboard, XInput, SDL)
- Save state support
- Shader caching
- Mod support
- Android support

### Planned (Volt-Specific)
- Per-game profiles
- Cloud save synchronization
- Modern theme system
- Plugin architecture
- Enhanced performance analytics
- Improved shader management
- Better frame pacing
- Reduced stuttering

---

## Getting Started

### System Requirements

**Minimum:**
| Component | Requirement |
|-----------|-------------|
| OS | Windows 10 64-bit / Ubuntu 22.04 / Android 9 |
| CPU | x86-64 with AVX2 support |
| RAM | 8 GB |
| GPU | Vulkan 1.1 capable |
| Storage | 30 GB free |

**Recommended:**
| Component | Requirement |
|-----------|-------------|
| OS | Windows 11 / Ubuntu 24.04 / Android 12 |
| CPU | 8-core x86-64 (e.g. Ryzen 5 5600, Core i5-12600K) |
| RAM | 16 GB |
| GPU | NVIDIA RTX / AMD RX 6000+ / Vulkan 1.3 |
| Storage | SSD, 50 GB free |

### Quick Start

1. Download the latest release from the [Releases](../../releases) page
2. Extract to a directory of your choice
3. Launch `volt` (Linux) or `volt.exe` (Windows)
4. Go to **File → Open Game Directory** and point to your Switch game folder
5. Double-click a game to launch

For full setup instructions, see [`docs/SETUP.md`](docs/SETUP.md).

---

## Building from Source

See [`docs/BUILDING.md`](docs/BUILDING.md) for platform-specific build instructions.

**Quick summary:**
```bash
git clone https://github.com/volt-emu/volt.git
cd volt
git submodule update --init --recursive
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

---

## Configuration

Volt stores its configuration in:

| Platform | Path |
|----------|------|
| Windows  | `%APPDATA%\Volt Emulator\` |
| Linux    | `~/.config/volt-emulator/` |
| Android  | Internal storage / `Android/data/dev.volt_emu.volt/` |

See [`docs/CONFIGURATION.md`](docs/CONFIGURATION.md) for all available settings.

---

## Contributing

We welcome contributions of all kinds. Please read:

- [`CONTRIBUTING.md`](CONTRIBUTING.md) — contribution workflow
- [`docs/standards/CODING_STANDARDS.md`](docs/standards/CODING_STANDARDS.md) — C++ style guide
- [`docs/ARCHITECTURE.md`](docs/architecture/ARCHITECTURE.md) — codebase overview

---

## Roadmap

| Phase | Focus | Status |
|-------|-------|--------|
| Phase 1 | Rebranding & Foundation | 🔄 Active |
| Phase 2 | Codebase Cleanup | 📋 Planned |
| Phase 3 | Performance Improvements | 📋 Planned |
| Phase 4 | UI Modernization | 📋 Planned |
| Phase 5 | New Features | 📋 Planned |

See [`docs/phases/ROADMAP.md`](docs/phases/ROADMAP.md) for the full roadmap.

---

## Legal & Attribution

Volt Emulator is built upon the work of many contributors across the emulator lineage:

- **Eden** — direct upstream fork base
- **Sudachi** — upstream of Eden
- **Yuzu** — original upstream project

All copyright notices, license headers, and attribution requirements are preserved throughout the codebase. See [`ATTRIBUTION.md`](ATTRIBUTION.md) and [`LICENSE`](LICENSE) for full details.

> Nintendo Switch is a trademark of Nintendo Co., Ltd. Volt Emulator is an independent project with no affiliation with or endorsement from Nintendo.

---

## License

Volt Emulator is licensed under the **GNU General Public License v3.0** (GPL-3.0), inherited from its upstream projects.

See [`LICENSE`](LICENSE) for the full license text.

Individual components may carry additional licenses — see each file's SPDX header and `docs/legal/THIRD_PARTY_LICENSES.md`.
