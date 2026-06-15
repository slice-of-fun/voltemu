# Phase 2 — Naming & Idiom Audit (Step 1.2 / 1.3)

Read-only audit of naming conventions and modernization candidates across `src/`
(excluding vendored `src/dynarmic/`). No code changed by this document — it scopes
the follow-up commits.

Tooling: `git grep` / `git ls-files` over the same file set CI formats.

---

## 1. File naming — ✅ Already compliant

All source file names (`*.cpp` / `*.h`) outside `src/dynarmic/` are already
`snake_case`. **0 violations.** No action needed.

---

## 2. `typedef` → `using` — 6 occurrences

Low risk, mechanical. Candidates:

| File | Line | Typedef |
|------|------|---------|
| `src/common/net/net.h` | 15 | anonymous `typedef struct { ... }` |
| `src/common/net/net.h` | 22 | `typedef struct Release { ... }` |
| `src/core/hle/service/nfc/common/amiibo_crypto.h` | 16 | `typedef struct evp_mac_ctx_st EVP_MAC_CTX;` ⚠️ mirrors OpenSSL's C API — **keep as-is** for ABI/compat clarity |
| `src/qt_common/abstract/frontend.h` | 55 | `typedef StandardButton Button;` |
| `src/qt_common/qt_common.h` | 32 | `typedef std::function<bool(std::size_t, std::size_t)> QtProgressCallback;` |
| `src/yuzu/migration_worker.h` | 10 | `typedef struct Emulator { ... }` |

**Recommendation:** Convert 5 (skip the OpenSSL-mirroring `EVP_MAC_CTX`). One small
commit, build-verified.

---

## 3. `#define` → `constexpr` — mostly NOT convertible

54 object-like `#define`s found. Categorized:

### 3a. Platform / conditional fallback macros — ❌ DO NOT convert
These define values only when the platform header didn't, or are `#ifdef`-guarded.
Converting to `constexpr` would break compilation on the platforms that *do* define
them. Examples:
- `MAX_PATH` (path_util.cpp — 260/1024 fallbacks)
- `MAP_NORESERVE`, `MAP_ALIGNED_SUPER`, `MFD_CLOEXEC`, `MEM_*_PLACEHOLDER` (host_memory.cpp)
- `_XCR_XFEATURE_ENABLED_MASK`, `COMPILED_HAS_SSE41`, `WORDS_BIGENDIAN`, `HAVE_BUILTIN_EXPECT`
- `PROCESS_POWER_THROTTLING_*` (Windows API fallbacks)

### 3b. Third-party library config defines — ❌ DO NOT convert
Must be macros (consumed by `#if` in vendored headers):
- `CPPHTTPLIB_*` (httplib.h), `SI_NO_CONVERSION` (config.h),
  `VMA_STATIC/DYNAMIC_VULKAN_FUNCTIONS` (vma.h), `YUZU_QT_MOVIE_MISSING`

### 3c. SMAA texture dimensions — ❌ DO NOT convert (vendored)
`smaa_area_tex.h` / `smaa_search_tex.h` are **vendored third-party files** (MIT,
© 2013 Jimenez et al.) containing the upstream SMAA lookup-texture data. The
`AREATEX_*` / `SEARCHTEX_*` `#define`s are part of that upstream data-file format
(and `AREATEX_SIZE` derives from them). Leave as-is to stay aligned with upstream —
treat like `src/dynarmic/`.

`U128_ZERO_INIT` (atomic_ops.h) is **platform-conditional** (two `#define`s + an
`#undef`) and used as an aggregate initializer — must remain a macro.

**Recommendation:** No `#define` → `constexpr` conversions in Step 1. Every candidate
is either platform-conditional, third-party-config, or vendored data.

### 3d. Vulkan extension-name string defines — ⚠️ leave
`VK_KHR_MAINTENANCE_7/8_EXTENSION_NAME` mirror Vulkan SDK naming conventions; keep
as macros for consistency with the SDK headers they supplement.

---

## 4. Class/struct naming — domain-specific lowercase (keep)

The non-PascalCase class names found in `src/core/` are **nvidia hardware-device
emulation classes** that deliberately mirror the real device/driver names:

`nvdevice`, `nvmap`, `nvdisp_disp0`, `nvhost_as_gpu`, `nvhost_ctrl`,
`nvhost_ctrl_gpu`, `nvhost_gpu`, `nvhost_nvdec`, `nvhost_nvdec_common`,
`nvhost_nvjpg`, `nvhost_vic`

**Recommendation:** **Keep as-is.** These match the hardware/driver node names they
emulate (e.g. `/dev/nvhost-gpu`); renaming them to PascalCase would *reduce* clarity
and break the mapping to documented Switch internals. Document this as an intentional
domain-specific exception (per ROADMAP item 2.2 "Document naming rationale for
domain-specific terms"). `fmt` is a namespace alias, not a violation.

---

## Proposed follow-up commits (each build-verified, separate)

1. `refactor(common): replace typedef with using` — 5 conversions, skip OpenSSL mirror
2. `docs(phase2): document nvidia device naming as intentional exception`

**Explicitly out of scope** (would be incorrect): converting platform-fallback,
third-party-config, and vendored-data (`SMAA`, `U128_ZERO_INIT`) `#define`s, and
renaming nvidia device classes.

---

## Cross-phase note (not Step 1 work)

`src/common/fs/fs_paths.h` still defines `VOLT_DIR "volt"` (plus legacy `YUZU_DIR`,
`SUDACHI_DIR`, `CITRON_DIR`, `SUYU_DIR`). The active app data dir is a **Phase 1
rebranding** item (`VOLT_DIR`/`"volt"`), not a Phase 2 cleanup task — flagged here so
it isn't lost, but it belongs to the Phase 1 string-rebrand workstream.
