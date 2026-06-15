# Phase 3 — Performance Improvements Execution

This document tracks the execution of **Phase 3: Performance Improvements** as defined in the project `ROADMAP.md`.

The primary goal of this phase is to measurably improve emulation performance, reduce stuttering, and improve frame pacing across all platforms. Due to the high risk of breaking runtime behavior when adjusting JIT blocks and Vulkan pipelines without continuous profiling, steps are executed carefully and marked with constraints where applicable.

---

## Progress Overview

- [ ] Step 1: CPU Emulation
- [ ] Step 2: Memory Management
- [ ] Step 3: Vulkan Backend
- [ ] Step 4: Shader Compilation
- [ ] Step 5: Frame Pacing
- [ ] Step 6: Multithreading
- [ ] Step 7: Android Performance

---

## Step 1: CPU Emulation

**Objective:** Improve the speed and efficiency of the emulated ARM CPU threads.

- [ ] Profile and optimize hot paths in Dynarmic JIT dispatch.
- [ ] Improve block chaining efficiency.
- [ ] Optimize memory access patterns in CPU core.
- [ ] Investigate and implement fast-path for common SVC patterns.
- [ ] Reduce lock contention in multi-core emulation.

## Step 2: Memory Management

**Objective:** Reduce memory overhead and improve data transfer speeds.

- [ ] Profile allocator behavior under load.
- [ ] Reduce heap fragmentation in emulated memory regions.
- [ ] Optimize GPU buffer upload paths.
- [ ] Implement buffer suballocation where beneficial.
- [ ] Reduce unnecessary copies in data transfer paths.

## Step 3: Vulkan Backend

**Objective:** Improve GPU command processing and eliminate renderer stutters.

- [ ] Audit pipeline creation — eliminate stutter sources.
- [ ] Improve descriptor set management.
- [ ] Optimize render pass usage (reduce redundant clears/loads).
- [ ] Implement GPU-driven culling where applicable.
- [ ] Improve VRAM usage estimation and eviction.
- [ ] Optimize command buffer recording overhead.

## Step 4: Shader Compilation

**Objective:** Reduce in-game hitches caused by shader translation and compilation.

- [ ] Profile shader compilation latency.
- [ ] Improve async compilation scheduling.
- [ ] Reduce shader compilation stutters.
- [ ] Implement better shader pre-warming.
- [ ] Improve pipeline cache hit rates.
- [ ] Add pipeline cache versioning and validation.

## Step 5: Frame Pacing

**Objective:** Ensure smooth frame delivery and synchronized audio.

- [ ] Implement accurate frame timing.
- [ ] Reduce frame latency.
- [ ] Improve vsync handling.
- [ ] Implement frame limiter with low CPU overhead.
- [ ] Investigate and implement low-latency audio path.

## Step 6: Multithreading

**Objective:** Better distribute work across host CPU cores and reduce locks.

- [ ] Audit thread pool usage.
- [ ] Reduce contention on shared resources (`std::mutex` audits).
- [ ] Improve work distribution across CPU cores.
- [ ] Investigate async GPU submission improvements.

## Step 7: Android Performance

**Objective:** Optimize the emulator specifically for mobile constraints.

- [ ] Profile on reference Android devices (Snapdragon 8 Gen 2+).
- [ ] Optimize thermal throttle response.
- [ ] Reduce power consumption on sustained load.
- [ ] Optimize surface flinger interaction.
- [ ] Improve audio latency on Android.

---

*This document will be updated as work progresses.*
