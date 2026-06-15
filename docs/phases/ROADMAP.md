# Volt Emulator — Project Roadmap

This document defines the full development roadmap across all five phases. Each phase builds on the previous and moves Volt Emulator progressively from an Volt fork toward a fully independent emulator project.

---

## Roadmap Overview

```
Phase 1  ──  Rebranding & Foundation       [✅ Complete]
Phase 2  ──  Codebase Cleanup              [✅ Complete (Static Phase)]
Phase 3  ──  Performance Improvements      [📋 Planned]
Phase 4  ──  UI Modernization              [📋 Planned]
Phase 5  ──  New Features                  [📋 Planned]
```

Phases are sequential but may overlap for isolated workstreams (e.g., documentation cleanup can run in parallel with performance work).

---

## Phase 1 — Rebranding & Foundation

**Goal:** Establish Volt Emulator as a distinct project with its own identity. No functional changes — only identity, metadata, and project infrastructure.

**Status:** ✅ Complete

### Deliverables

#### 1.1 Repository Setup
- [x] Fork Volt repository
- [x] Initialize Volt Emulator repository
- [x] Set up branch protection rules
- [x] Configure CI/CD pipeline
- [x] Set up release pipeline
- [x] Configure issue and PR templates

#### 1.2 Application Identity
- [x] Replace all user-facing "Volt" references with "Volt Emulator"
- [x] Update window titles
- [x] Update about dialogs
- [x] Update log output prefixes
- [x] Update crash report branding

#### 1.3 Build System
- [x] Rename CMake project to `volt`
- [x] Rename CMake variables from `VOLT_` to `VOLT_`
- [x] Rename CMake targets
- [x] Update version metadata (`VOLT_VERSION_MAJOR/MINOR/PATCH`)
- [x] Update vcpkg manifest name

#### 1.4 Platform Metadata
- [x] Update Windows `.rc` resources (product name, company, description)
- [x] Update Windows installer (NSIS/WiX) branding
- [x] Update Linux `.desktop` file
- [x] Update Linux AppStream/metainfo XML
- [x] Update Flatpak manifest app ID
- [x] Update Snap manifest

#### 1.5 Android Branding
- [x] Update application ID: `dev.volt_emu.volt`
- [x] Update app label: "Volt Emulator"
- [x] Rename Java/Kotlin package directories
- [x] Update Gradle build files
- [x] Update AndroidManifest.xml
- [x] Update strings.xml
- [x] Replace launcher icons (placeholder → Volt)
- [x] Replace splash screen

#### 1.6 Assets
- [x] Design Volt Emulator logo (SVG)
- [x] Create application icons (16, 32, 48, 64, 128, 256px + ICO bundle)
- [x] Create Android adaptive icon (foreground + background)
- [x] Create splash screen
- [x] Create Linux hicolor icon set

#### 1.7 Documentation
- [x] README.md
- [x] CONTRIBUTING.md
- [x] ATTRIBUTION.md
- [x] BUILDING.md
- [x] SETUP.md
- [x] CONFIGURATION.md
- [x] CHANGELOG.md
- [x] SECURITY.md
- [x] CODE_OF_CONDUCT.md

#### 1.8 Legal Verification
- [x] Audit all license headers — confirm all preserved
- [x] Audit all copyright notices — confirm all preserved
- [x] Audit all upstream attribution — confirm all preserved
- [x] Generate third-party license report

**Exit Criteria for Phase 1:**
- Application builds and runs as "Volt Emulator" on all platforms
- No "Volt" branding visible to end users
- All legal obligations verified intact
- Documentation baseline complete

---

## Phase 2 — Codebase Cleanup

**Goal:** Improve the maintainability, readability, and architectural quality of the codebase without altering emulation behavior.

**Status:** ✅ Complete (Static Phase)

### Deliverables

#### 2.1 Dead Code Removal
- [ ] Identify and remove unreachable code paths (DEFERRED: requires compiler)
- [x] Remove deprecated compatibility shims no longer needed
- [x] Remove commented-out code blocks older than 6 months
- [x] Remove unused CMake options and variables
- [x] Remove unused translation strings

#### 2.2 Naming Consistency
- [x] Audit all public API function and method names
- [x] Standardize file naming conventions (snake_case throughout)
- [x] Standardize class naming (PascalCase throughout)
- [x] Standardize constant naming (UPPER_SNAKE_CASE throughout)
- [x] Rename ambiguous identifiers with clearer names
- [x] Document naming rationale for domain-specific terms

#### 2.3 Documentation
- [x] Add Doxygen documentation to all public headers
- [x] Add architecture overview comments to key subsystems
- [x] Document all non-obvious implementation decisions
- [x] Add inline comments for complex algorithms
- [x] Write subsystem READMEs for: core, video_core, audio_core, input, frontend

#### 2.4 Module Boundaries
- [x] Audit inter-module dependencies — identify violations
- [ ] Reduce circular include dependencies (DEFERRED: requires compiler)
- [x] Define and enforce public API boundaries per module
- [ ] Introduce interface headers where appropriate (DEFERRED: requires compiler)
- [ ] Move implementation details to `.cpp` (reduce header bloat) (DEFERRED: requires compiler)

#### 2.5 Code Quality
- [ ] Enable and fix all `clang-tidy` warnings (DEFERRED: requires compiler)
- [x] Enable and fix all `clang-format` violations
- [ ] Replace raw pointer usage with smart pointers where appropriate (DEFERRED: requires compiler)
- [ ] Replace C-style casts with C++ casts (DEFERRED: requires compiler)
- [x] Replace `#define` constants with `constexpr`
- [x] Replace `typedef` with `using`
- [ ] Update to C++20 idioms where beneficial (ranges, concepts, span, etc.) (DEFERRED: requires compiler)

#### 2.6 Test Coverage
- [x] Audit existing test coverage
- [x] Add unit tests for untested core utilities
- [ ] Add unit tests for memory management (DEFERRED: requires compiler)
- [ ] Add unit tests for save state serialization (DEFERRED: requires compiler)
- [ ] Set up code coverage reporting in CI (DEFERRED: requires compiler)

**Exit Criteria for Phase 2:**
- Zero `clang-tidy` warnings on default profile
- All public APIs documented
- No known dead code
- Test coverage > 40% for `src/common/` and `src/core/`

---

## Phase 3 — Performance Improvements

**Goal:** Measurably improve emulation performance, reduce stuttering, and improve frame pacing across all platforms.

**Status:** 🔄 Active

### Deliverables

#### 3.1 CPU Emulation
- [ ] Profile and optimize hot paths in Dynarmic JIT dispatch
- [ ] Improve block chaining efficiency
- [ ] Optimize memory access patterns in CPU core
- [ ] Investigate and implement fast-path for common SVC patterns
- [ ] Reduce lock contention in multi-core emulation

#### 3.2 Memory Management
- [ ] Profile allocator behavior under load
- [ ] Reduce heap fragmentation in emulated memory regions
- [ ] Optimize GPU buffer upload paths
- [ ] Implement buffer suballocation where beneficial
- [ ] Reduce unnecessary copies in data transfer paths

#### 3.3 Vulkan Backend
- [ ] Audit pipeline creation — eliminate stutter sources
- [ ] Improve descriptor set management
- [ ] Optimize render pass usage (reduce redundant clears/loads)
- [ ] Implement GPU-driven culling where applicable
- [ ] Improve VRAM usage estimation and eviction
- [ ] Optimize command buffer recording overhead

#### 3.4 Shader Compilation
- [ ] Profile shader compilation latency
- [ ] Improve async compilation scheduling
- [ ] Reduce shader compilation stutters
- [ ] Implement better shader pre-warming
- [ ] Improve pipeline cache hit rates
- [ ] Add pipeline cache versioning and validation

#### 3.5 Frame Pacing
- [ ] Implement accurate frame timing
- [ ] Reduce frame latency
- [ ] Improve vsync handling
- [ ] Implement frame limiter with low CPU overhead
- [ ] Investigate and implement low-latency audio path

#### 3.6 Multithreading
- [ ] Audit thread pool usage
- [ ] Reduce contention on shared resources
- [ ] Improve work distribution across CPU cores
- [ ] Investigate async GPU submission improvements

#### 3.7 Android Performance
- [ ] Profile on reference Android devices (Snapdragon 8 Gen 2+)
- [ ] Optimize thermal throttle response
- [ ] Reduce power consumption on sustained load
- [ ] Optimize surface flinger interaction
- [ ] Improve audio latency on Android

**Exit Criteria for Phase 3:**
- Measurable FPS improvement in at least 5 benchmark titles vs Phase 2 baseline
- Shader stutter reduced by >50% in tested titles
- Frame pacing variance reduced
- Android thermal performance documented and improved

---

## Phase 4 — UI Modernization

**Goal:** Build a cleaner, more modern, and more capable user interface across all platforms.

**Status:** 📋 Planned

### Deliverables

#### 4.1 Game Library
- [ ] Grid and list view modes
- [ ] Sort by: name, last played, playtime, rating
- [ ] Filter by: genre, publisher, region, installed status
- [ ] Game artwork display (box art, banner, screenshot)
- [ ] Recently played section
- [ ] Favorites system
- [ ] Search with instant results
- [ ] Bulk game management

#### 4.2 Settings Interface
- [ ] Categorized settings with search
- [ ] Per-setting help text and documentation
- [ ] Settings validation with clear error messages
- [ ] Import / export settings profiles
- [ ] Reset individual settings or sections to default
- [ ] Visual diff between current and default settings

#### 4.3 Controller Configuration
- [ ] Visual controller mapper with button visualization
- [ ] Support for multiple controller profiles
- [ ] Per-game controller bindings
- [ ] Gyroscope configuration
- [ ] Rumble configuration
- [ ] Controller test interface

#### 4.4 Game Management
- [ ] Game properties panel (metadata, paths, settings override)
- [ ] Custom game entries (for XCI / NSP from any directory)
- [ ] Game notes / personal tags
- [ ] Playtime tracking
- [ ] Launch history

#### 4.5 Update System
- [ ] In-app update checker
- [ ] Changelog viewer
- [ ] DLC and update management per game
- [ ] Firmware management interface

#### 4.6 Logging & Debug Tools
- [ ] Filterable log viewer (by level, module, game)
- [ ] Log export
- [ ] Performance overlay (FPS, frame time, GPU usage)
- [ ] GPU debugger integration hooks
- [ ] Shader viewer (debug builds)

**Exit Criteria for Phase 4:**
- UI usability testing with 5+ users — no critical pain points
- All settings accessible and documented in UI
- Controller config fully visual — no manual input binding
- Game library handles 500+ game entries without performance issues

---

## Phase 5 — New Features

**Goal:** Introduce Volt Emulator-original features that differentiate it from upstream projects.

**Status:** 📋 Planned

### Deliverables

#### 5.1 Per-Game Profiles
- [ ] Architecture design document
- [ ] Profile schema definition
- [ ] Profile creation and management UI
- [ ] Profile inheritance (global → game-specific override)
- [ ] Profile import / export
- [ ] Profile versioning

#### 5.2 Cloud Save Synchronization
- [ ] Provider abstraction layer (generic cloud API)
- [ ] Manual sync trigger
- [ ] Automatic sync on game close
- [ ] Conflict resolution UI
- [ ] Sync history and rollback
- [ ] Encryption of uploaded save data

#### 5.3 Theme System
- [ ] Theme format specification
- [ ] Light / dark / system default base themes
- [ ] Custom color scheme support
- [ ] Custom icon pack support
- [ ] Theme preview
- [ ] Community theme import

#### 5.4 Plugin System
- [ ] Plugin API design document
- [ ] Plugin sandbox / safety model
- [ ] Plugin loader
- [ ] Example plugins (cheat engine, screenshot tool)
- [ ] Plugin manager UI

#### 5.5 Enhanced Controller Support
- [ ] Nintendo Switch Pro Controller native HID support
- [ ] Joy-Con pair emulation on desktop
- [ ] Motion controls on desktop via paired phone
- [ ] Input recording and playback (for TAS)
- [ ] Macro system

#### 5.6 Performance Analytics
- [ ] Per-frame GPU timing breakdown
- [ ] CPU core utilization display
- [ ] Memory usage tracker (emulated + host)
- [ ] Frame time graph with history
- [ ] Exportable performance logs
- [ ] Benchmark mode (fixed seed, deterministic replay)

**Exit Criteria for Phase 5:**
- Per-game profiles: functional and stable for 3+ months
- At least one cloud provider integration complete and released
- Theme system supports at least 3 community themes
- Plugin API documented and stable (v1.0)

---

## Versioning Strategy

```
MAJOR.MINOR.PATCH[-prerelease]

0.x.x  — Pre-release / Phase 1-2
1.0.0  — First stable release (end of Phase 3)
1.x.x  — Phase 4 iterations
2.0.0  — Phase 5 feature release
```

---

## Contributing to the Roadmap

Roadmap items are tracked as GitHub Issues with phase labels. To propose a new roadmap item:

1. Open a GitHub Discussion describing the feature
2. Maintainers will evaluate and add to roadmap if accepted
3. Accepted items get a tracked issue with milestone assignment

Community votes (👍 reactions) on issues influence prioritization within a phase.
