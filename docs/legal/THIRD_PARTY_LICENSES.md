# Third-Party Licenses

Volt Emulator incorporates third-party open-source libraries. Their respective licenses are listed below. All license terms are honored.

This document must be distributed with any binary release of Volt Emulator.

---

## Dynarmic

**Purpose:** ARM64 JIT recompiler  
**License:** BSD 2-Clause  
**Repository:** https://github.com/merryhime/dynarmic

```
Copyright (c) 2016 MerryMage
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
```

---

## Dear ImGui

**Purpose:** Immediate-mode debug UI  
**License:** MIT  
**Repository:** https://github.com/ocornut/imgui

```
Copyright (c) 2014-2024 Omar Cornut

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
```

---

## SDL2

**Purpose:** Input handling, windowing, audio output  
**License:** Zlib License  
**Repository:** https://github.com/libsdl-org/SDL

```
Copyright (C) 1997-2024 Sam Lantinga

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the
use of this software.

Permission is granted to anyone to use this software for any purpose, including
commercial applications, and to alter it and redistribute it freely, subject to
the following restrictions:
  1. The origin of this software must not be misrepresented; you must not claim
     that you wrote the original software.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
```

---

## {fmt}

**Purpose:** String formatting  
**License:** MIT  
**Repository:** https://github.com/fmtlib/fmt

```
Copyright (c) 2012-present, Victor Zverovich and {fmt} contributors

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction...
[Full MIT License text]
```

---

## spdlog

**Purpose:** Fast logging library  
**License:** MIT  
**Repository:** https://github.com/gabime/spdlog

MIT License — Copyright (c) 2016 Gabi Melman

---

## Vulkan Headers

**Purpose:** Vulkan API definitions  
**License:** Apache 2.0  
**Repository:** https://github.com/KhronosGroup/Vulkan-Headers

Copyright 2015-2024 The Khronos Group Inc.  
Apache License, Version 2.0

---

## SPIRV-Cross

**Purpose:** Shader cross-compilation (SPIRV → GLSL/HLSL/MSL)  
**License:** Apache 2.0  
**Repository:** https://github.com/KhronosGroup/SPIRV-Cross

Copyright 2015-2021 Arm Limited  
Apache License, Version 2.0

---

## glslang

**Purpose:** GLSL/HLSL compiler to SPIRV  
**License:** BSD / MIT (dual)  
**Repository:** https://github.com/KhronosGroup/glslang

Copyright (C) 2002-2005 3Dlabs Inc. Ltd.  
Copyright (C) 2012-2016 LunarG, Inc.  
BSD 2-Clause and MIT License

---

## zstd (Zstandard)

**Purpose:** Compression  
**License:** BSD 3-Clause and GPL-2.0 (dual)  
**Repository:** https://github.com/facebook/zstd

Copyright (c) Meta Platforms, Inc. and affiliates.  
BSD 3-Clause License

---

## lz4

**Purpose:** Fast compression  
**License:** BSD 2-Clause  
**Repository:** https://github.com/lz4/lz4

Copyright (c) 2011-2020 Yann Collet  
BSD 2-Clause License

---

## Mbed TLS

**Purpose:** Cryptographic primitives (AES, SHA, RSA)  
**License:** Apache 2.0  
**Repository:** https://github.com/Mbed-TLS/mbedtls

Copyright The Mbed TLS Contributors  
Apache License, Version 2.0

---

## Opus

**Purpose:** Audio codec  
**License:** BSD 3-Clause  
**Repository:** https://opus-codec.org/

Copyright 2001-2011 Xiph.Org, Skype Limited, Octasic, Jean-Marc Valin, Timothy B. Terriberry, CSIRO, Gregory Maxwell, Mark Borgerding, Erik de Castro Lopo  
BSD 3-Clause License

---

## FFmpeg

**Purpose:** Audio and video codec support  
**License:** LGPL-2.1-or-later (core); GPL-2.0-or-later (some components)  
**Repository:** https://ffmpeg.org/

Copyright (c) the FFmpeg developers  
LGPL-2.1-or-later / GPL-2.0-or-later

> Volt Emulator links FFmpeg dynamically to maintain LGPL compliance. GPL components are not used unless explicitly opted in by the user.

---

## Boost

**Purpose:** General-purpose C++ utilities  
**License:** Boost Software License 1.0  
**Repository:** https://www.boost.org/

Copyright Beman Dawes, David Abrahams, 1998-2005.  
Copyright Rene Rivera 2004-2007.

---

## nlohmann/json

**Purpose:** JSON parsing  
**License:** MIT  
**Repository:** https://github.com/nlohmann/json

Copyright (c) 2013-2022 Niels Lohmann  
MIT License

---

## toml11

**Purpose:** TOML configuration parsing  
**License:** MIT  
**Repository:** https://github.com/ToruNiina/toml11

Copyright (c) 2017 Toru Niina  
MIT License

---

## Catch2

**Purpose:** Unit test framework  
**License:** BSL-1.0 (Boost Software License)  
**Repository:** https://github.com/catchorg/Catch2

Copyright (c) 2022 Two Blue Cubes Ltd.  
Boost Software License, Version 1.0

---

## Qt6

**Purpose:** Desktop UI framework  
**License:** LGPL-3.0 (open source)  
**Repository:** https://www.qt.io/

Copyright (C) The Qt Company Ltd.  
GNU Lesser General Public License v3.0

> Volt Emulator uses Qt6 under the LGPL-3.0 open-source license.

---

## VMA (Vulkan Memory Allocator)

**Purpose:** Vulkan GPU memory management  
**License:** MIT  
**Repository:** https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator

Copyright (c) 2017-2024 Advanced Micro Devices, Inc.  
MIT License

---

## LLVM (optional)

**Purpose:** Shader compilation backend (optional, when enabled)  
**License:** Apache 2.0 with LLVM Exceptions  
**Repository:** https://llvm.org/

Copyright (c) 2003-2023 University of Illinois at Urbana-Champaign  
Apache 2.0 with LLVM Exceptions

---

## Robin Hood Hashing

**Purpose:** High-performance hash map  
**License:** MIT  
**Repository:** https://github.com/martinus/robin-hood-hashing

Copyright (c) 2018-2021 Martin Ankerl  
MIT License

---

*This document is maintained manually. If you discover a missing attribution, please open an issue.*
