# Phase 2 — Step 1 Execution Plan: Code Quality & Formatting

This plan operationalizes **Step 1** of `PHASE2_EXECUTION.md`. It is intentionally
conservative because formatting changes touch a 3,000+ file codebase and are easy
to get wrong.

---

## Critical finding (must resolve before formatting)

The repo currently has **conflicting `.clang-format` configs**:

| File | Style | Brace wrapping | Scope it actually governs |
|------|-------|----------------|---------------------------|
| `/.clang-format` (new, added in `de8c76c506`) | LLVM-based, custom | `AfterFunction: true` | **No source files** — there are no `.cpp/.h` at repo root |
| `/src/.clang-format` (upstream yuzu) | yuzu legacy | `BreakBeforeBraces: Attach` | All of `src/**` |
| `/src/dynarmic/.clang-format` (upstream, GPL) | `BasedOnStyle: google`, `ColumnLimit: 0` | google | `src/dynarmic/**` (vendored) |

Because `clang-format --style=file` resolves the **nearest** config, the new root
config formats nothing today, and CI is still enforcing the old `src/` rules.

**The two configs disagree on brace placement** — applying one vs. the other to the
whole tree produces two completely different (and both enormous) diffs.

This decision is already captured by the user choice made before planning, but the
mechanics below assume: **make the root `.clang-format` the single source of truth,
remove `src/.clang-format`, and leave `src/dynarmic/.clang-format` untouched** (it is
vendored upstream code with its own license header and must not be reformatted).

---

## Scope & exclusions

- **In scope:** `src/**/*.cpp` and `src/**/*.h`, excluding the directories below.
- **Excluded (vendored / upstream, do not reformat):**
  - `src/dynarmic/**` — has its own `.clang-format`, GPL upstream, tracked separately.
  - Any other vendored trees discovered during the dry run (verify none slipped in).
- **Tooling:** `clang-format` is **not installed locally**. We rely on either a local
  install (pinned version) or the existing CI `clang-format` job. **Version matters** —
  output differs across major versions. We pin to the CI version (Ubuntu 22.04 default,
  clang-format 14) to keep local and CI output identical.

---

## Execution steps

### 1.1 — Consolidate the format config (no code reformatted yet)
1. Decide root `.clang-format` is canonical (done — user-directed).
2. Add a `DisableFormat`-free, reviewed root config (already present).
3. **Remove `src/.clang-format`** so the root config governs `src/` (except dynarmic,
   which keeps its own).
4. Add an explicit ignore for vendored trees: create `src/dynarmic/` exclusion via the
   find command in CI (or a `.clang-format-ignore` if the pinned version supports it).
5. Update CI `build.yml` `clang-format` job to:
   - pin the clang-format version explicitly,
   - exclude `src/dynarmic/` from the `find` set,
   - keep `--dry-run --Werror` (becomes the gate after reformat lands).
6. **Commit:** `style(format): consolidate clang-format config to repo root`
   *(config only — zero source diff in this commit, easy to review)*

### 1.2 — Reformat the tree (mechanical, isolated commit)
1. Run `clang-format -i --style=file` over the in-scope file set (dynarmic excluded).
2. **Sanity gate before committing** (formatting must not change meaning):
   - `git diff --stat` — confirm only expected dirs touched, dynarmic untouched.
   - Confirm **no SPDX / copyright header lines** were altered:
     `git diff -U0 | grep -E 'SPDX|Copyright'` must be empty.
   - Build still compiles (CI or local) — whitespace-only changes should be a no-op.
3. **Commit (separate, mechanical):**
   `style(format): apply clang-format across src/`
   with a body noting it is a pure whitespace/formatting pass, no behavior change,
   and that vendored dynarmic was excluded.

> Keeping 1.1 (config) and 1.2 (the giant diff) in **separate commits** is the key
> review-ability decision: reviewers approve the config once, then the mechanical
> diff needs only a spot-check, not line-by-line reading.

### 1.3 — Naming conventions (audit-first, NOT bulk rename)
Naming changes are **not** mechanical and risk breaking the build / ABI. For Step 1
we only **audit and report**, deferring actual renames to scoped follow-up commits.
1. File names → `snake_case`: list violators (script, report only).
2. Class/struct → `PascalCase`: spot-audit public headers, report violators.
3. Constants → `UPPER_SNAKE_CASE`: report only.
4. Produce `docs/phases/PHASE2_NAMING_AUDIT.md` with findings + proposed renames.
5. Renames themselves are individual reviewed commits (deferred — out of Step 1.2's
   mechanical pass to avoid mixing semantic changes into the formatting diff).

### 1.4 — Modern C++ idioms (targeted, reviewed)
Also non-mechanical. Audit-then-fix in small commits:
1. `#define` numeric/string constants → `constexpr` (skip macro-function-like and
   conditional-compilation defines).
2. `typedef` → `using` in `src/common/` and `src/core/` public headers first.
3. Each conversion batch = its own commit, compiled before commit.

---

## Commit sequence (summary)

1. `style(format): consolidate clang-format config to repo root` — config only
2. `style(format): apply clang-format across src/` — mechanical whitespace diff
3. `ci: pin clang-format version and exclude vendored dynarmic` — (may fold into #1)
4. `docs(phase2): add naming convention audit` — report, no code change
5. *(follow-ups)* scoped naming renames + `constexpr`/`using` conversions

---

## Risks & mitigations

| Risk | Mitigation |
|------|------------|
| Reformat alters license/SPDX headers | Grep gate on `SPDX`/`Copyright` in the diff before commit |
| clang-format version drift (local ≠ CI) | Pin to clang-format 14 (CI default); document the version |
| Vendored dynarmic gets reformatted | Explicit exclude in find set + verify `git diff --stat` |
| Giant diff is unreviewable | Split config vs. reformat into separate commits |
| Whitespace pass changes behavior | Build must pass post-format (whitespace is a no-op) |
| Naming/idiom changes mixed into format diff | Keep 1.3/1.4 in separate semantic commits |

---

## What I need installed / available

- `clang-format` **14** locally (matching CI), OR run the reformat in a CI job.
  Without it, I can prepare 1.1 (config consolidation + CI update) and the 1.3 audit,
  but **1.2 (the actual reformat) must run where clang-format exists.**