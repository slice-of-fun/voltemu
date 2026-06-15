# Changelog

All notable changes to Volt Emulator are documented here.

This file follows [Keep a Changelog](https://keepachangelog.com/en/1.0.0/) format.  
Volt Emulator uses [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [Unreleased]

### Added
- Project documentation suite (README, CONTRIBUTING, ARCHITECTURE, BUILDING, CONFIGURATION, ROADMAP, CODING_STANDARDS, SECURITY, CHANGELOG, ATTRIBUTION)
- Rebranding scripts for Phase 1 identity migration
- Audit tooling for verifying complete rebrand coverage

### Changed
- Project name: Volt → Volt Emulator
- Application name: Volt → Volt Emulator
- Short name: Volt → Volt
- Organization: Volt Emulator Team → Volt Emulator Team
- Android package ID: `dev.volt_emu.volt` → `dev.volt_emu.volt`
- Executable name: `volt` → `volt`
- App ID: `dev.volt_emu.volt` → `dev.volt_emu.volt`
- CMake project: `volt` → `volt`
- CMake variable prefix: `VOLT_` → `VOLT_`

### Preserved
- All upstream license headers (GPL-3.0-or-later)
- All upstream copyright notices
- Full attribution to Volt, Sudachi, and Yuzu

---

## Notes on Versioning

Volt Emulator versioning begins at `0.1.0` for the initial Phase 1 release.

```
0.x.x — Pre-release / Phase 1-2 (Foundation)
1.0.0 — First stable release (end of Phase 3 / Performance)
1.x.x — Phase 4 UI iterations
2.0.0 — Phase 5 Feature release
```

---

## Upstream Reference

For changes inherited from Volt prior to this fork point, see the Volt project changelog at:  
https://git.volt-emu.dev/volt-emu/volt

For deeper upstream history:
- Sudachi changelog
- Yuzu changelog (archived)

All upstream changes are incorporated into Volt Emulator's initial codebase but are tracked in upstream repositories, not this changelog. This changelog covers only Volt Emulator-specific changes from the fork point onward.
