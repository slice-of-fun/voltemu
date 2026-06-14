// SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
// SPDX-License-Identifier: GPL-3.0-or-later

#include "common/random.h"

#include <optional>
#include <random>

namespace Common::Random {
[[nodiscard]] static std::random_device& GetGlobalRandomDevice() noexcept
{
    static std::random_device g_random_device{};
    return g_random_device;
}
[[nodiscard]] u32 Random32(u32 seed) noexcept
{
    return GetGlobalRandomDevice()();
}
[[nodiscard]] u64 Random64(u64 seed) noexcept
{
    return GetGlobalRandomDevice()();
}
[[nodiscard]] std::mt19937 GetMT19937() noexcept
{
    return std::mt19937(GetGlobalRandomDevice()());
}
} // namespace Common::Random
