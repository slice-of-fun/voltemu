# Building Volt Emulator

This guide covers building Volt Emulator from source on all supported platforms.

---

## Table of Contents

- [Prerequisites](#prerequisites)
- [Clone](#clone)
- [Windows](#windows)
- [Linux](#linux)
- [Android](#android)
- [CMake Options](#cmake-options)
- [Troubleshooting](#troubleshooting)

---

## Prerequisites

### All Platforms

- **Git** 2.30+
- **CMake** 3.20+
- **Python** 3.8+ (for build scripts)

### Submodules

Volt Emulator uses Git submodules for most dependencies:

```bash
git submodule update --init --recursive
```

This is **required** before any build. Missing submodules are the most common build failure.

---

## Clone

```bash
git clone https://github.com/volt-emu/volt.git
cd volt
git submodule update --init --recursive
```

---

## Windows

### Option A — Visual Studio 2022 (Recommended)

**Required:**
- Visual Studio 2022 (Community or higher)
- Workloads: "Desktop development with C++" 
- Individual components: "C++ CMake tools for Windows"
- Vulkan SDK: https://vulkan.lunarg.com/sdk/home#windows (1.3+)

**Build:**

```powershell
# From the repo root in Developer PowerShell for VS 2022
cmake -B build ^
      -G "Visual Studio 17 2022" ^
      -A x64 ^
      -DCMAKE_BUILD_TYPE=Release

cmake --build build --config Release --parallel
```

Output: `build\bin\Release\volt.exe`

### Option B — LLVM/Clang (Alternative)

```powershell
cmake -B build ^
      -G "Ninja" ^
      -DCMAKE_C_COMPILER=clang-cl ^
      -DCMAKE_CXX_COMPILER=clang-cl ^
      -DCMAKE_BUILD_TYPE=Release

cmake --build build --parallel
```

### Windows Notes

- MinGW is **not supported** — use MSVC or Clang-CL
- The Vulkan SDK must be installed before configuring CMake
- Windows Defender may slow builds — add the repo and build directories to exclusions

---

## Linux

### Ubuntu / Debian

**Install dependencies:**

```bash
sudo apt update
sudo apt install -y \
    build-essential \
    cmake \
    ninja-build \
    git \
    python3 \
    pkg-config \
    libsdl2-dev \
    libvulkan-dev \
    vulkan-validationlayers-dev \
    libglfw3-dev \
    libfmt-dev \
    libboost-dev \
    libopus-dev \
    nasm \
    clang-15 \
    clang-format-15 \
    clang-tidy-15 \
    qtbase5-dev \
    qtbase5-private-dev \
    qt6-base-dev \
    qt6-base-private-dev \
    libqt6opengl6-dev \
    zlib1g-dev \
    libzstd-dev \
    liblz4-dev \
    libmbedtls-dev
```

**Build:**

```bash
cmake -B build \
      -G Ninja \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_C_COMPILER=clang-15 \
      -DCMAKE_CXX_COMPILER=clang++-15

cmake --build build --parallel $(nproc)
```

Output: `build/bin/volt`

### Arch Linux / Manjaro

```bash
sudo pacman -S --needed \
    base-devel cmake ninja git python \
    sdl2 vulkan-headers vulkan-icd-loader \
    fmt boost opus qt6-base \
    zstd lz4 mbedtls clang
```

Then run the same CMake commands as above.

### Fedora

```bash
sudo dnf install -y \
    gcc-c++ cmake ninja-build git python3 \
    SDL2-devel vulkan-headers vulkan-loader-devel \
    fmt-devel boost-devel opus-devel \
    qt6-qtbase-devel zlib-devel zstd-devel \
    lz4-devel mbedtls-devel clang
```

### Linux Notes

- GCC 12+ is minimum; Clang 15+ recommended
- Vulkan drivers must be installed separately for your GPU:
  - NVIDIA: proprietary driver (550+) or `vulkan-nvidia` package
  - AMD: Mesa `vulkan-radeon`
  - Intel: Mesa `vulkan-intel`
- Qt6 is required for the desktop frontend; Qt5 is not supported in new code

---

## Android

### Prerequisites

- **Android Studio** Hedgehog (2023.1) or newer
- **Android NDK** r25c or newer
- **Android SDK** API level 30 minimum (target API 34)
- Vulkan-capable Android device or emulator

### Build

```bash
cd src/android

# Debug build
./gradlew assembleDebug

# Release build (requires signing config)
./gradlew assembleRelease
```

Or open `src/android/` in Android Studio and build from IDE.

### Signing for Release

Create `src/android/keystore.properties`:

```properties
storeFile=path/to/your.keystore
storePassword=your_store_password
keyAlias=your_key_alias
keyPassword=your_key_password
```

### Android Notes

- NDK r25c is strongly recommended — other versions may have compiler bugs
- The minimum supported Android API is 30 (Android 11)
- Vulkan is required — OpenGL ES is not supported on Android
- 64-bit ARM (arm64-v8a) is the only supported ABI

---

## CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `VOLT_ENABLE_VULKAN` | ON | Build Vulkan rendering backend |
| `VOLT_ENABLE_OPENGL` | OFF | Build OpenGL backend (legacy, limited) |
| `VOLT_ENABLE_LTO` | OFF | Enable link-time optimization (Release only) |
| `VOLT_USE_BUNDLED_SDL2` | ON | Use bundled SDL2 submodule |
| `VOLT_USE_BUNDLED_FMT` | ON | Use bundled fmt submodule |
| `VOLT_USE_BUNDLED_FFMPEG` | ON | Use bundled FFmpeg |
| `VOLT_ENABLE_TESTS` | ON | Build unit tests |
| `VOLT_ENABLE_ASAN` | OFF | Enable AddressSanitizer (Debug) |
| `VOLT_ENABLE_TSAN` | OFF | Enable ThreadSanitizer (Debug) |
| `VOLT_ENABLE_COVERAGE` | OFF | Enable code coverage (Debug) |
| `VOLT_WARNINGS_AS_ERRORS` | OFF | Treat all warnings as errors |
| `VOLT_ENABLE_TRACY` | OFF | Enable Tracy profiler integration |
| `VOLT_ENABLE_LLD` | OFF | Use LLD linker (faster linking) |

### Recommended Debug Build

```bash
cmake -B build-debug \
      -G Ninja \
      -DCMAKE_BUILD_TYPE=Debug \
      -DVOLT_ENABLE_ASAN=ON \
      -DVOLT_ENABLE_TESTS=ON \
      -DVOLT_WARNINGS_AS_ERRORS=ON

cmake --build build-debug --parallel $(nproc)
```

### Recommended Release Build

```bash
cmake -B build-release \
      -G Ninja \
      -DCMAKE_BUILD_TYPE=Release \
      -DVOLT_ENABLE_LTO=ON \
      -DVOLT_ENABLE_VULKAN=ON

cmake --build build-release --parallel $(nproc)
```

---

## Running Tests

```bash
cd build
ctest --output-on-failure --parallel $(nproc)
```

Or run a specific test binary directly:

```bash
./build/bin/tests/volt_tests_common
./build/bin/tests/volt_tests_core
```

---

## Troubleshooting

### "Could not find Vulkan"
Install the Vulkan SDK (Windows) or `vulkan-headers` + `vulkan-loader` packages (Linux).

### "submodule not found" / missing headers
Run: `git submodule update --init --recursive`

### CMake version too old
CMake 3.20+ required. Install from: https://cmake.org/download/

### Build is very slow (Windows)
- Use Ninja generator instead of MSBuild: `-G Ninja`
- Exclude build and source directories from Windows Defender scanning
- Use `clangd` or disable Intellisense while building

### Linker errors on Linux (undefined references)
Ensure all submodules are initialized. Check that system packages match the minimum versions. Try with bundled dependencies: `-DVOLT_USE_BUNDLED_SDL2=ON`.

### Android NDK not found
Set `ANDROID_NDK_HOME` environment variable to your NDK path, or configure in Android Studio SDK settings.

### Shader compilation errors at runtime
Ensure you have a Vulkan 1.1+ capable GPU and up-to-date drivers. Report the specific error with GPU model and driver version in the issue tracker.

---

## CI Build Reference

The CI builds on every PR. To replicate the CI build locally:

```bash
# Matches CI config exactly
cmake -B build \
      -G Ninja \
      -DCMAKE_BUILD_TYPE=Release \
      -DVOLT_ENABLE_TESTS=ON \
      -DVOLT_WARNINGS_AS_ERRORS=ON \
      -DVOLT_ENABLE_VULKAN=ON

cmake --build build --parallel 4
cd build && ctest --output-on-failure
```
