# Contributing to Volt Emulator

Thank you for your interest in contributing. Volt Emulator is a community-driven project and every contribution matters — whether it's a bug report, documentation fix, performance improvement, or new feature.

---

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Ways to Contribute](#ways-to-contribute)
- [Development Setup](#development-setup)
- [Branching Strategy](#branching-strategy)
- [Commit Standards](#commit-standards)
- [Pull Request Process](#pull-request-process)
- [Code Standards](#code-standards)
- [Testing Requirements](#testing-requirements)
- [Documentation Requirements](#documentation-requirements)
- [Review Process](#review-process)
- [What We Will Not Accept](#what-we-will-not-accept)

---

## Code of Conduct

All contributors are expected to:

- Be respectful and constructive in all interactions
- Focus criticism on code and ideas, never on people
- Welcome newcomers and help them get started
- Maintain professional communication in issues, PRs, and discussions

Violations may result in removal from the project.

---

## Ways to Contribute

### Bug Reports
- Search existing issues first to avoid duplicates
- Use the bug report template
- Include: OS, GPU, driver version, game title and ID, reproduction steps, logs

### Performance Reports
- Include GPU/CPU profiles where possible
- Compare against a known baseline (Eden, previous Volt version)
- Attach relevant shader cache info if applicable

### Code Contributions
- Bug fixes
- Performance optimizations
- Compatibility improvements
- New features (discuss in an issue first for large changes)
- Refactoring (Phase 2 scope — coordinate with maintainers)

### Documentation
- Fix errors or unclear explanations
- Add missing documentation
- Translate documentation (future)

### Testing
- Test games and report compatibility
- Reproduce reported bugs
- Verify fixes on your hardware

---

## Development Setup

### Prerequisites

**All Platforms:**
- Git
- CMake 3.20+
- Python 3.8+ (build scripts)

**Windows:**
- Visual Studio 2022 with C++ workload
- Or: LLVM/Clang 16+
- vcpkg (optional)

**Linux:**
- GCC 12+ or Clang 15+
- Required packages: see `docs/BUILDING.md#linux`

**Android:**
- Android Studio
- NDK r25+
- SDK API level 30+

### Clone and Build

```bash
# Clone
git clone https://github.com/volt-emu/volt.git
cd volt

# Initialize all submodules
git submodule update --init --recursive

# Configure (Release)
cmake -B build \
      -DCMAKE_BUILD_TYPE=Release \
      -DVOLT_ENABLE_VULKAN=ON

# Build
cmake --build build --parallel $(nproc)

# Run
./build/bin/volt
```

For full platform-specific build instructions see [`docs/BUILDING.md`](docs/BUILDING.md).

---

## Branching Strategy

| Branch | Purpose |
|--------|---------|
| `main` | Stable, release-ready code |
| `dev` | Active development integration branch |
| `phase/N-description` | Phase-specific work branches |
| `feat/short-description` | Feature branches (branch from `dev`) |
| `fix/short-description` | Bug fix branches (branch from `dev` or `main`) |
| `perf/short-description` | Performance work branches |
| `refactor/short-description` | Cleanup/refactor branches |
| `docs/short-description` | Documentation-only branches |

### Rules

- **Never** push directly to `main`
- All changes go through pull requests
- Feature branches must branch from `dev`
- Hotfixes for `main` may branch from `main` and merge to both `main` and `dev`
- Delete branches after merge

---

## Commit Standards

Volt Emulator uses [Conventional Commits](https://www.conventionalcommits.org/).

### Format

```
<type>(<scope>): <short summary>

[optional body]

[optional footer]
```

### Types

| Type | When to Use |
|------|-------------|
| `feat` | New feature |
| `fix` | Bug fix |
| `perf` | Performance improvement |
| `refactor` | Code restructuring (no behavior change) |
| `docs` | Documentation only |
| `test` | Adding or fixing tests |
| `build` | Build system changes |
| `ci` | CI/CD changes |
| `rebrand` | Phase 1 rebranding changes |
| `chore` | Maintenance tasks |

### Scopes

`core`, `video_core`, `audio_core`, `input`, `frontend`, `android`, `common`, `networking`, `cmake`, `docs`

### Examples

```
feat(video_core): add Vulkan pipeline cache versioning

fix(core): correct NPDM permission flag parsing for homebrew

perf(video_core): reduce barrier overhead in async shader compilation

rebrand(android): update package name to dev.volt_emu.volt

docs(contributing): add branching strategy section
```

### Rules

- Summary line: 72 characters max, imperative mood ("add" not "added")
- Reference issues: `Closes #123`, `Fixes #456`, `Related to #789`
- Breaking changes: add `BREAKING CHANGE:` footer
- Each commit should represent one logical change
- Do not mix rebrand commits with functional changes

---

## Pull Request Process

### Before Submitting

- [ ] Code compiles without warnings on your platform
- [ ] Existing tests pass
- [ ] New tests added for new functionality
- [ ] Documentation updated if behavior changed
- [ ] Commit history is clean (squash WIP commits)
- [ ] Branch is up to date with `dev`
- [ ] Self-reviewed the diff

### PR Description Template

```markdown
## Summary
What does this PR do?

## Motivation
Why is this change needed?

## Changes
- List of specific changes

## Testing
How was this tested? Which games/scenarios?

## Screenshots (if UI change)

## Related Issues
Closes #N
```

### Size Guidelines

| PR Size | Lines Changed | Review Time |
|---------|--------------|-------------|
| Small   | < 200        | 1-2 days    |
| Medium  | 200–800      | 3-5 days    |
| Large   | 800–2000     | 1-2 weeks   |
| Epic    | > 2000       | Coordinate with maintainers first |

Large PRs should be split into smaller logical units whenever possible.

---

## Code Standards

See [`docs/standards/CODING_STANDARDS.md`](standards/CODING_STANDARDS.md) for the full C++ style guide.

**Key rules:**

- C++20
- 4-space indentation, no tabs
- `clang-format` enforced (`.clang-format` in repo root)
- `clang-tidy` clean (`.clang-tidy` in repo root)
- No raw owning pointers — use `std::unique_ptr`, `std::shared_ptr`
- Prefer `std::span`, `std::string_view`, `std::optional`
- All public APIs must be documented
- No `using namespace std;` in headers
- SPDX license header required on all new files

### New File Header Template

```cpp
// SPDX-FileCopyrightText: Copyright 2025 Volt Emulator Team
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

// ... code
```

---

## Testing Requirements

- Bug fixes must include a regression test where feasible
- New subsystems must include unit tests
- Performance-sensitive code should include benchmarks
- Tests live in `src/tests/`

Run tests:
```bash
cmake --build build --target test
# or
cd build && ctest --output-on-failure
```

---

## Documentation Requirements

Any PR that:
- Adds a new feature → update relevant `.md` doc or add one
- Changes existing behavior → update docs to reflect new behavior
- Adds a new configuration option → update `docs/CONFIGURATION.md`
- Changes build requirements → update `docs/BUILDING.md`
- Adds a new subsystem → add architecture note to `docs/architecture/`

---

## Review Process

1. CI must pass (build, tests, format checks)
2. At least **one maintainer approval** required for `dev`
3. At least **two maintainer approvals** required for `main`
4. Reviewer feedback must be addressed or discussed before merge
5. Maintainer merges (not the author)
6. Squash merge for feature branches, merge commit for release branches

---

## What We Will Not Accept

The following will be rejected regardless of technical quality:

- Code that enables or facilitates piracy
- Removal of license headers or copyright notices
- Removal of upstream attribution (Eden / Sudachi / Yuzu)
- Code that violates GPL-3.0 obligations
- AI-generated code submitted without human review and understanding
- Changes that introduce Nintendo proprietary code or assets
- Malicious code of any kind
- Breaking changes without documented migration path

---

## Questions?

Open a discussion in the repository or reach out via the community channels listed in `README.md`.
