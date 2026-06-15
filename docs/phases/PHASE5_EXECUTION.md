# Phase 5 — New Features Execution

This document tracks the execution of **Phase 5: New Features** as defined in the project `ROADMAP.md`.

The primary goal of this phase is to introduce Volt Emulator-original features that differentiate it from upstream projects. Because this involves designing net-new architecture, careful planning and static structuring will be prioritized where possible until a build environment is established.

---

## Progress Overview

- [ ] Step 1: Per-Game Profiles
- [ ] Step 2: Cloud Save Synchronization
- [ ] Step 3: Theme System
- [x] Step 3.1: Base Theme Implementation (Completed early in Phase 4)
- [ ] Step 4: Plugin System
- [ ] Step 5: Enhanced Controller Support
- [ ] Step 6: Performance Analytics

---

## Step 1: Per-Game Profiles

**Objective:** Allow users to set specific emulation settings on a per-game basis.

- [ ] Architecture design document
- [ ] Profile schema definition
- [ ] Profile creation and management UI
- [ ] Profile inheritance (global → game-specific override)
- [ ] Profile import / export
- [ ] Profile versioning

## Step 2: Cloud Save Synchronization

**Objective:** Seamlessly sync save data across devices.

- [ ] Provider abstraction layer (generic cloud API)
- [ ] Manual sync trigger
- [ ] Automatic sync on game close
- [ ] Conflict resolution UI
- [ ] Sync history and rollback
- [ ] Encryption of uploaded save data

## Step 3: Theme System

**Objective:** Expand the theme engine introduced in Phase 4.

- [x] Theme format specification (Completed)
- [x] Light / dark / system default base themes (Completed)
- [ ] Custom color scheme support UI
- [ ] Custom icon pack support
- [ ] Theme preview
- [ ] Community theme import

## Step 4: Plugin System

**Objective:** Allow third-party extensions without compiling the core emulator.

- [ ] Plugin API design document
- [ ] Plugin sandbox / safety model
- [ ] Plugin loader
- [ ] Example plugins (cheat engine, screenshot tool)
- [ ] Plugin manager UI

## Step 5: Enhanced Controller Support

**Objective:** Improve input options for power users.

- [ ] Nintendo Switch Pro Controller native HID support
- [ ] Joy-Con pair emulation on desktop
- [ ] Motion controls on desktop via paired phone
- [ ] Input recording and playback (for TAS)
- [ ] Macro system

## Step 6: Performance Analytics

**Objective:** Provide deep insights into emulation bottlenecks.

- [ ] Per-frame GPU timing breakdown
- [ ] CPU core utilization display
- [ ] Memory usage tracker (emulated + host)
- [ ] Frame time graph with history
- [ ] Exportable performance logs
- [ ] Benchmark mode (fixed seed, deterministic replay)

---

*This document will be updated as work progresses.*
