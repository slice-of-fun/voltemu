<h1 align="center">
  <br>
  <a href="https://github.com/pushkarverse/volt-emu"><img src="./dist/volt.icon/Assets/volt.png" alt="Volt Emulator" width="200"></a>
  <br>
  <b>Volt Emulator</b>
  <br>
</h1>

<h4 align="center"><b>Volt Emulator</b> is a free and open-source (FOSS) Nintendo Switch emulator, derived from Eden, Sudachi, and Yuzu.
It is written in C++ with portability in mind, with builds for Windows, Linux, Android, and more.
</h4>

<p align="center">
    <a href="https://github.com/pushkarverse/volt-emu/releases">
        <img src="https://img.shields.io/github/v/release/pushkarverse/volt-emu?color=5865F2&label=Latest%20Release&logo=github"
            alt="Latest Release">
    </a>
    <a href="https://github.com/pushkarverse/volt-emu/issues">
        <img src="https://img.shields.io/github/issues/pushkarverse/volt-emu?color=orange&label=Issues&logo=github"
            alt="Issues">
    </a>
    <a href="https://github.com/pushkarverse/volt-emu/blob/dev/LICENSE.txt">
        <img src="https://img.shields.io/badge/License-GPL--3.0-blue.svg"
            alt="License: GPL-3.0">
    </a>
</p>

<p align="center">
  <a href="#compatibility">Compatibility</a> |
  <a href="#development">Development</a> |
  <a href="#building">Building</a> |
  <a href="#download">Download</a> |
  <a href="#attribution">Attribution</a> |
  <a href="#license">License</a>
</p>

## Compatibility

Volt Emulator is capable of running most commercial Nintendo Switch games at full speed, provided you meet the necessary hardware requirements.

A compatibility list will be available in a future update. For now, games that work on Eden are expected to work on Volt Emulator as well.

## Development

Development takes place on [GitHub](https://github.com/pushkarverse/volt-emu). For discussions, issues, and contributions, please use the GitHub repository.

If you would like to contribute, please read [`CONTRIBUTING.md`](CONTRIBUTING.md) and [`docs/standards/CODING_STANDARDS.md`](docs/standards/CODING_STANDARDS.md) before submitting a pull request.

## Documentation

- [Setup Guide](docs/SETUP.md) — How to install and configure Volt Emulator
- [Building](docs/BUILDING.md) — How to build from source
- [Architecture](docs/architecture/ARCHITECTURE.md) — Project structure overview
- [Roadmap](docs/phases/ROADMAP.md) — Development roadmap
- [Configuration](docs/CONFIGURATION.md) — Settings and options reference

## Building

See the [Building Guide](docs/BUILDING.md) for platform-specific instructions (Windows, Linux, Android).

For information on provided development tooling, see the [Tools directory](./tools).

## Download

You can download the latest releases from the [Releases page](https://github.com/pushkarverse/volt-emu/releases).

## Attribution

Volt Emulator is built on the foundational work of:

- **[Eden](https://git.eden-emu.dev/eden-emu/eden)** — Direct upstream fork
- **[Sudachi](https://github.com/sudachi-emu/sudachi)** — Eden's upstream
- **[Yuzu](https://github.com/yuzu-emu/yuzu)** — Original Nintendo Switch emulator (archived)
- **[Dynarmic](https://github.com/merryhime/dynarmic)** — ARM JIT recompiler
- All third-party library authors listed in [`docs/legal/THIRD_PARTY_LICENSES.md`](docs/legal/THIRD_PARTY_LICENSES.md)

Full upstream attribution is documented in [`ATTRIBUTION.md`](ATTRIBUTION.md).

## License

Volt Emulator is licensed under the **GPL-3.0-or-later**. Refer to the [LICENSE.txt](LICENSE.txt) file.

All upstream copyright notices and license headers are preserved in full as required by the GPL.
