# Attribution

Volt Emulator is derived from prior open-source work. This document records all upstream projects, their contributors, and the legal obligations Volt Emulator carries as a result.

This file must never be removed, shortened, or altered to reduce attribution accuracy.

---

## Upstream Lineage

```
Volt Emulator
    └── Fork of Eden
            └── Fork of Sudachi
                    └── Fork of Yuzu
```

---

## Eden

**Repository:** https://git.eden-emu.dev/eden-emu/eden  
**License:** GNU General Public License v3.0 (GPL-3.0)

Eden is the direct upstream of Volt Emulator. Volt began as a fork of Eden and continues to incorporate its architecture and implementation as a foundation.

All Eden contributors retain copyright over their respective contributions. Eden itself is a derivative of Sudachi.

---

## Sudachi

**License:** GNU General Public License v3.0 (GPL-3.0)

Sudachi is the upstream of Eden. Significant portions of the core emulation logic, CPU emulation, and graphics backend originate from or were shaped by the Sudachi codebase.

---

## Yuzu

**Repository:** https://github.com/yuzu-emu/yuzu (archived)  
**License:** GNU General Public License v3.0 (GPL-3.0)

Yuzu is the original open-source Nintendo Switch emulator from which the upstream lineage flows. Major architectural decisions, subsystem designs, and implementations in Volt Emulator trace their origins to Yuzu contributors.

> Yuzu was discontinued in March 2024. Volt Emulator acknowledges and respects the contributions of the entire Yuzu development community.

---

## Third-Party Libraries

Volt Emulator incorporates third-party libraries under their respective licenses. See [`docs/legal/THIRD_PARTY_LICENSES.md`](legal/THIRD_PARTY_LICENSES.md) for the complete list.

Key dependencies include (non-exhaustive):

| Library | License | Purpose |
|---------|---------|---------|
| Dynarmic | BSD 2-Clause | ARM64 JIT recompiler |
| Dear ImGui | MIT | Debug UI |
| SDL2 | Zlib | Input and windowing |
| fmt | MIT | String formatting |
| spdlog | MIT | Logging |
| Vulkan Headers | Apache 2.0 | Vulkan API |
| SPIRV-Cross | Apache 2.0 | Shader cross-compilation |
| glslang | BSD / MIT | GLSL compiler |
| zstd | BSD / GPL-2.0 | Compression |
| lz4 | BSD 2-Clause | Compression |
| mbedtls | Apache 2.0 | Cryptography |
| opus | BSD 3-Clause | Audio codec |
| FFmpeg | LGPL-2.1 | Audio/video codec |
| Boost | Boost Software License | Utilities |
| nlohmann/json | MIT | JSON parsing |
| toml11 | MIT | TOML config |
| catch2 | BSL-1.0 | Unit testing |
| LLVM (optional) | Apache 2.0 | Shader compilation |

---

## Original Volt Emulator Contributions

Code written originally for Volt Emulator (not derived from upstream) is copyright of the Volt Emulator Team and individual contributors, and is licensed under GPL-3.0.

Contributors to Volt Emulator are listed in [`CONTRIBUTORS.md`](../CONTRIBUTORS.md).

---

## Legal Notice

Volt Emulator is an independent open-source project. It is not affiliated with, endorsed by, or connected to Nintendo Co., Ltd. in any way.

"Nintendo Switch" is a registered trademark of Nintendo Co., Ltd.

The Volt Emulator project does not distribute, encourage, or facilitate piracy of copyrighted game software.
