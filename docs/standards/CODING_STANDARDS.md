# Volt Emulator — C++ Coding Standards

All code in Volt Emulator follows these standards. PRs that violate them will be asked to correct the violation before merge. These rules are enforced automatically via `clang-format` and `clang-tidy` in CI.

---

## Language Version

**C++20** is the standard. Use C++20 features where they improve clarity or performance. Do not use features not yet widely supported by MSVC, GCC 12, and Clang 15.

Approved C++20 features:
- `std::span`
- `std::bit_cast`
- `std::string_view` (also in C++17, prefer everywhere)
- `std::optional` / `std::variant` (also C++17)
- Designated initializers
- `[[likely]]` / `[[unlikely]]`
- Concepts and requires clauses (for template constraints)
- Ranges (use with care — has compile-time cost)
- `std::format` (use `fmt::format` as fallback if not available everywhere)
- Coroutines (only in `experimental/`)

---

## Formatting

Formatting is enforced by `.clang-format`. Never manually override formatting in PRs — let the tool do it.

Key rules (summarized from `.clang-format`):

```
IndentWidth:        4
TabWidth:           4
UseTab:             Never
ColumnLimit:        120
BreakBeforeBraces:  Allman
PointerAlignment:   Left
```

Run before committing:
```bash
clang-format -i $(git diff --name-only HEAD | grep -E '\.(cpp|h|hpp)$')
```

---

## Naming Conventions

| Category | Convention | Example |
|----------|-----------|---------|
| Files | `snake_case` | `memory_manager.cpp` |
| Classes / Structs | `PascalCase` | `class MemoryManager` |
| Functions / Methods | `PascalCase` | `void AllocateMemory()` |
| Variables (local) | `snake_case` | `u32 page_count` |
| Variables (member) | `snake_case` | `u32 page_count` |
| Constants / `constexpr` | `UPPER_SNAKE_CASE` | `constexpr u32 PAGE_SIZE = 4096` |
| Namespaces | `PascalCase` | `namespace VideoCore` |
| Enum values | `PascalCase` | `enum class State { Running, Paused }` |
| Template parameters | `PascalCase` | `template <typename ValueType>` |
| Macros (avoid!) | `VOLT_UPPER_SNAKE` | `VOLT_ASSERT(...)` |

### Do not use:
- Hungarian notation (`m_member`, `g_global`, `p_ptr`)
- Abbreviated names unless universally understood (`cpu`, `gpu`, `fps`, `dma`)
- Single-letter names except loop indices (`i`, `j`, `k`) and trivial lambdas

---

## File Structure

Every `.h` / `.hpp` file:

```cpp
// SPDX-FileCopyrightText: Copyright 2025 Volt Emulator Team
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

// System includes first (alphabetical)
#include <memory>
#include <string>
#include <vector>

// Third-party includes second
#include <vulkan/vulkan.h>

// Project includes last (alphabetical within group)
#include "common/types.h"
#include "video_core/renderer_base.h"

namespace Volt::VideoCore {

// ... declarations

} // namespace Volt::VideoCore
```

Every `.cpp` file:

```cpp
// SPDX-FileCopyrightText: Copyright 2025 Volt Emulator Team
// SPDX-License-Identifier: GPL-3.0-or-later

// Corresponding header first
#include "video_core/vulkan/vk_device.h"

// Then same order as headers: system, third-party, project
#include <algorithm>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "common/logging/log.h"

namespace Volt::VideoCore {

// ... implementation

} // namespace Volt::VideoCore
```

---

## Type Aliases

Use the project's type aliases from `common/types.h`:

```cpp
using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using s8  = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;
using s64 = std::int64_t;
using f32 = float;
using f64 = double;
using VAddr = u64;   // Virtual address in emulated space
using PAddr = u64;   // Physical address in emulated space
```

Do not use `int`, `long`, `unsigned` for emulation-related values. Always be explicit about width.

---

## Memory & Ownership

### Rule: No raw owning pointers

```cpp
// BAD
Texture* texture = new Texture(desc);

// GOOD
auto texture = std::make_unique<Texture>(desc);
```

### Ownership rules

| Scenario | Type to use |
|----------|-------------|
| Sole ownership, heap-allocated | `std::unique_ptr<T>` |
| Shared ownership | `std::shared_ptr<T>` |
| Non-owning reference to object | `T&` or `T*` (raw, non-owning) |
| Optional non-owning reference | `T*` (nullptr = absent) |
| Slice / array view | `std::span<T>` |
| String view | `std::string_view` |
| Optional value | `std::optional<T>` |
| Error-or-value | `std::expected<T, E>` (C++23) or custom `Result<T>` |

### Never:
- `delete` anything manually (use smart pointers)
- Store raw pointers in containers (use `unique_ptr` or indices)
- Use `new[]` / `delete[]` (use `std::vector`)

---

## Error Handling

### Emulation errors (recoverable)
Use `Result` / `std::expected` for operations that can legitimately fail:

```cpp
[[nodiscard]] Result<FileHandle> OpenFile(std::string_view path);
```

### Programming errors (bugs)
Use `VOLT_ASSERT` for invariants that must never fail:

```cpp
VOLT_ASSERT(page_count > 0, "Page count must be positive");
```

### Unimplemented HLE stubs
Use the standard stub logging pattern:

```cpp
Result MyService::UnimplementedFunction(HLERequestContext& ctx) {
    LOG_WARNING(Service_MyService, "(STUBBED) called");
    IPC::ResponseBuilder rb{ctx, 2};
    rb.Push(ResultSuccess);
    return ResultSuccess;
}
```

### Never use exceptions for control flow
Exceptions are disabled (`-fno-exceptions`) in most build configurations. Use `Result` types.

---

## Classes

```cpp
class MemoryManager {
public:
    // Construction
    explicit MemoryManager(Core::System& system);
    ~MemoryManager();

    // No implicit copies of large objects
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;

    // Move is OK if cheap
    MemoryManager(MemoryManager&&) = default;
    MemoryManager& operator=(MemoryManager&&) = default;

    // Public API — documented
    /// Allocates a contiguous virtual memory region of `size` bytes.
    /// Returns the virtual address of the allocation, or VAddr{0} on failure.
    [[nodiscard]] VAddr Allocate(std::size_t size, MemoryPermission permission);

    /// Frees a previously allocated region.
    void Free(VAddr address, std::size_t size);

private:
    // Private implementation details
    struct PageTableEntry { /* ... */ };

    Core::System& system;
    std::vector<PageTableEntry> page_table;
};
```

### Rules:
- Mark single-argument constructors `explicit` unless implicit conversion is intentional
- Delete copy constructor/assignment for non-trivially-copyable owning types
- `[[nodiscard]]` on all functions returning `Result`, pointers, or computed values
- Keep constructors simple — complex init goes in an `Initialize()` method
- Prefer composition over inheritance; avoid deep inheritance hierarchies
- Pure interfaces use `= 0` virtual functions; no data members

---

## Functions

```cpp
// Good: small, single responsibility, named clearly
[[nodiscard]] u32 CalculateChecksum(std::span<const u8> data) noexcept;

// Good: output parameter only when returning multiple values
void DecodeInstruction(u32 opcode, Instruction& out_instruction);

// Bad: too many parameters — use a struct
void SetupPipeline(VkDevice device, VkRenderPass pass, VkShaderModule vert,
                   VkShaderModule frag, u32 width, u32 height, bool depth_test,
                   bool blending); // <- wrap in PipelineCreateInfo struct instead
```

- Function length: aim for < 50 lines; > 100 lines needs justification
- One responsibility per function
- `[[nodiscard]]` on non-void return values where ignoring is a bug
- `noexcept` where the function genuinely cannot throw
- Prefer return values over output parameters for single values
- Use `std::tuple` / structs for multiple return values

---

## Namespaces

```cpp
namespace Volt {         // Top-level project namespace
namespace Core {         // Subsystem namespace
namespace Memory {       // Sub-subsystem namespace

// All code lives in appropriate namespace
// No "using namespace" in headers
// "using namespace" in .cpp files is OK for local utility namespaces

} // namespace Memory
} // namespace Core
} // namespace Volt
```

Never: `using namespace std;` anywhere in the codebase.

---

## Logging

```cpp
#include "common/logging/log.h"

// Module-specific log calls:
LOG_TRACE(Loader, "Loading NSP: {}", path.string());
LOG_DEBUG(HW_GPU, "Draw call: {} vertices, primitive={}", count, primitive);
LOG_INFO(Service_AM, "Application launched: {:#018x}", title_id);
LOG_WARNING(Service_HID, "(STUBBED) GetNpadStyleSet called");
LOG_ERROR(Loader, "Failed to decrypt NCA: key not found");
LOG_CRITICAL(Core, "Unrecoverable CPU exception at PC={:#018x}", pc);
```

- Use the correct module tag — never `LOG_INFO(Common, ...)` for service-specific logs
- Do not use `printf`, `std::cout`, or `std::cerr`
- Trace logs are stripped in release builds — use liberally for debugging
- Critical logs immediately precede fatal exits

---

## Performance-Sensitive Code

In hot paths (called per frame, per draw call, per audio sample):

```cpp
// Prefer branch hints when probability is well-known
if ([[likely]] cache.Contains(key)) {
    return cache.Get(key);
}

// Prefer std::span over pointer+size pairs — zero overhead
void ProcessVertices(std::span<const Vertex> vertices);

// Avoid std::function in hot paths — use templates
template <typename Callback>
void ForEachDrawCall(Callback&& callback); // zero overhead, inlineable

// Mark small, frequently-called functions inline (let the compiler decide for larger ones)
[[nodiscard]] inline u32 HashState(const PipelineState& state) noexcept;
```

Performance PRs must include profiling data (perf, Tracy, RenderDoc, or equivalent).

---

## Thread Safety

```cpp
class CommandQueue {
public:
    // Document thread safety explicitly
    /// Thread-safe. Can be called from any thread.
    void Enqueue(GPUCommand command);

    /// NOT thread-safe. Must be called only from the GPU thread.
    void ProcessAll();

private:
    std::mutex queue_mutex;
    std::vector<GPUCommand> pending_commands;
};
```

Rules:
- Document every class's thread-safety contract in its header
- Prefer lock-free structures for high-frequency inter-thread communication
- Never hold a mutex across a blocking call
- Never take two locks simultaneously without a defined lock order (prevents deadlock)
- Use `std::atomic` for simple shared counters/flags

---

## Testing

Tests use [Catch2](https://github.com/catchorg/Catch2).

```cpp
// SPDX-FileCopyrightText: Copyright 2025 Volt Emulator Team
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch_test_macros.hpp>
#include "common/math/alignment.h"

TEST_CASE("AlignUp rounds up to next power of two", "[common][math]") {
    REQUIRE(Common::AlignUp(0u, 4096u) == 0u);
    REQUIRE(Common::AlignUp(1u, 4096u) == 4096u);
    REQUIRE(Common::AlignUp(4096u, 4096u) == 4096u);
    REQUIRE(Common::AlignUp(4097u, 4096u) == 8192u);
}

TEST_CASE("AlignDown rounds down", "[common][math]") {
    REQUIRE(Common::AlignDown(4097u, 4096u) == 4096u);
}
```

Rules:
- Every new utility function in `common/` needs a test
- Test names are descriptive sentences
- Tags: `[subsystem][component]`
- No test should depend on external files or network

---

## Checklist for New Code

Before submitting a PR, verify:

- [ ] SPDX header present on all new files
- [ ] `clang-format` applied
- [ ] `clang-tidy` clean (no new warnings)
- [ ] No raw owning pointers
- [ ] All public API functions documented with `///` doc comments
- [ ] Thread safety documented for shared classes
- [ ] `[[nodiscard]]` applied where ignoring return is a bug
- [ ] No `using namespace std`
- [ ] Correct type aliases used (`u32` not `unsigned int`)
- [ ] Log calls use correct module tag
- [ ] Tests added for new utilities
