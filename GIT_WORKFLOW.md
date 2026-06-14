# Git Workflow

This document defines the branching model, commit practices, and release workflow for Volt Emulator.

---

## Branch Structure

```
main          ─────────────────────────────────────── (stable, tagged releases)
               ▲              ▲              ▲
dev           ─┼──────────────┼──────────────┼──────── (integration branch)
               │              │              │
feat/foo      ─┘        fix/bar       perf/baz
```

| Branch Pattern | Purpose | Base Branch | Merges Into |
|----------------|---------|-------------|-------------|
| `main` | Stable releases only | — | — |
| `dev` | Active development | `main` | `main` (via release PR) |
| `feat/<name>` | New features | `dev` | `dev` |
| `fix/<name>` | Bug fixes | `dev` | `dev` |
| `perf/<name>` | Performance work | `dev` | `dev` |
| `refactor/<name>` | Code cleanup | `dev` | `dev` |
| `docs/<name>` | Documentation | `dev` | `dev` |
| `rebrand/<name>` | Phase 1 rebranding | `dev` | `dev` |
| `phase/<N>-<name>` | Phase-wide work | `dev` | `dev` |
| `hotfix/<name>` | Critical fix | `main` | `main` + `dev` |
| `release/v<X.Y.Z>` | Release staging | `dev` | `main` |

---

## Commit Standards

### Format

```
<type>(<scope>): <summary>

[body — explain WHY, not what. What is in the diff.]

[footer — issue refs, breaking change notice]
```

### Types

| Type | Use for |
|------|---------|
| `feat` | New feature or capability |
| `fix` | Bug fix |
| `perf` | Performance improvement |
| `refactor` | Code change with no behavior change |
| `docs` | Documentation changes only |
| `test` | Adding or correcting tests |
| `build` | Build system or dependency changes |
| `ci` | CI/CD pipeline changes |
| `rebrand` | Phase 1 identity changes |
| `chore` | Housekeeping (update .gitignore, etc.) |
| `revert` | Reverting a previous commit |

### Scopes

`core`, `video_core`, `audio_core`, `input`, `frontend`, `android`, `common`, `networking`, `cmake`, `docs`, `ci`, `experimental`

### Rules

- **Imperative mood:** "add support for X" not "added support for X"
- **72 char limit** on summary line
- **One logical change per commit** — don't bundle unrelated fixes
- **Reference issues:** `Closes #123`, `Fixes #456`, `Related #789`
- **Breaking changes:** add `BREAKING CHANGE: <explanation>` in footer

### Good Examples

```
feat(video_core): add async pipeline cache validation

Pipelines were previously validated synchronously on load, causing
noticeable hitching when launching games with large shader caches.
This moves validation to a background thread pool, unblocking the
emulation thread immediately.

Closes #204
```

```
fix(core): correct NCA header version check for firmware 18.x

The version byte interpretation changed in firmware 18.0 — the old
check was accepting invalid headers and silently loading corrupted
content. Added version-aware parsing with a fallback path.

Fixes #318
```

```
perf(video_core): reduce Vulkan descriptor pool fragmentation

Pool sizing was too conservative, causing frequent reallocation under
titles that batch many draw calls per frame. Increased initial pool
size and added growth factor tuning. ~8% frame time improvement on
tested titles.
```

### Bad Examples

```
fix stuff           ← not descriptive
WIP                 ← never commit WIP to shared branches
fixed the bug       ← past tense, too vague
video core changes  ← no type, no scope, not imperative
```

---

## Working on a Feature

```bash
# 1. Start from up-to-date dev
git checkout dev
git pull origin dev

# 2. Create feature branch
git checkout -b feat/my-feature

# 3. Work in small, logical commits
git add -p                          # stage hunks, not whole files
git commit -m "feat(scope): ..."

# 4. Keep branch up to date with dev
git fetch origin
git rebase origin/dev               # rebase, not merge

# 5. Clean up history before PR
git rebase -i origin/dev            # squash WIP commits

# 6. Push and open PR
git push origin feat/my-feature
# Open PR on GitHub: feat/my-feature → dev
```

---

## Pull Request Rules

- **Title** matches commit format: `feat(scope): summary`
- **Description** uses the PR template
- All CI checks must pass before merge
- At least 1 maintainer approval for `dev`
- At least 2 maintainer approvals for `main`
- Author does not merge their own PR
- Branch deleted after merge

### Merge Strategy

| Target | Method | Reason |
|--------|--------|--------|
| `dev` | Squash merge | Keeps `dev` history clean |
| `main` | Merge commit | Preserves PR context in history |
| Hotfix | Cherry-pick to both `main` and `dev` | Surgical, no extra noise |

---

## Release Process

```
1. Cut release branch from dev
   git checkout -b release/v1.0.0 dev

2. Update version in CMakeLists.txt
   set(VOLT_VERSION_MAJOR 1)
   set(VOLT_VERSION_MINOR 0)
   set(VOLT_VERSION_PATCH 0)

3. Update CHANGELOG.md
   - Move [Unreleased] entries to [1.0.0] section
   - Add release date

4. Final QA on release branch
   - Build all platforms
   - Smoke test on reference titles

5. Merge release branch into main
   git checkout main
   git merge --no-ff release/v1.0.0

6. Tag the release
   git tag -a v1.0.0 -m "Volt Emulator v1.0.0"
   git push origin main --tags

7. Merge back into dev (pick up version bump and changelog)
   git checkout dev
   git merge --no-ff main

8. Delete release branch
   git branch -d release/v1.0.0
```

---

## Hotfix Process

```
1. Branch from main
   git checkout -b hotfix/critical-crash main

2. Fix the issue
   git commit -m "fix(core): ..."

3. Merge into main
   git checkout main
   git merge --no-ff hotfix/critical-crash
   git tag -a v1.0.1 -m "Volt Emulator v1.0.1"
   git push origin main --tags

4. Backport to dev
   git checkout dev
   git cherry-pick <commit-hash>
   git push origin dev

5. Delete hotfix branch
   git branch -d hotfix/critical-crash
```

---

## Rebase Policy

- **Always rebase** feature/fix branches onto `dev` before PR
- **Never rebase** `dev` or `main` (public branches)
- **Never force-push** to `dev` or `main`
- Force-push to personal feature branches is allowed

---

## Tagging Convention

```
v<MAJOR>.<MINOR>.<PATCH>[-<prerelease>]

Examples:
v0.1.0          — First Phase 1 release
v0.1.1          — Hotfix
v0.2.0          — Phase 2 complete
v1.0.0          — First stable release
v1.0.0-rc.1     — Release candidate
v1.0.0-beta.1   — Beta
```

---

## Git Configuration Recommendations

```bash
# Enable rebase on pull (avoids merge commits in local history)
git config --global pull.rebase true

# Better diff output
git config --global diff.algorithm histogram

# Auto-stash before rebase
git config --global rebase.autoStash true

# Sign commits (recommended for maintainers)
git config --global commit.gpgsign true
```
