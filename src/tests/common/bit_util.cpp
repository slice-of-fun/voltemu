// SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
// SPDX-License-Identifier: GPL-3.0-or-later

#include "common/bit_util.h"

#include <catch2/catch_test_macros.hpp>

#include "common/common_types.h"

namespace Common {

TEST_CASE("BitUtil: BitSize", "[common]")
{
    STATIC_REQUIRE(BitSize<u8>() == 8);
    STATIC_REQUIRE(BitSize<u16>() == 16);
    STATIC_REQUIRE(BitSize<u32>() == 32);
    STATIC_REQUIRE(BitSize<u64>() == 64);
}

TEST_CASE("BitUtil: MostSignificantBit", "[common]")
{
    STATIC_REQUIRE(MostSignificantBit32(1U) == 0U);
    STATIC_REQUIRE(MostSignificantBit32(2U) == 1U);
    STATIC_REQUIRE(MostSignificantBit32(0xFFFFFFFFU) == 31U);
    STATIC_REQUIRE(MostSignificantBit32(0x80000000U) == 31U);

    STATIC_REQUIRE(MostSignificantBit64(1ULL) == 0U);
    STATIC_REQUIRE(MostSignificantBit64(0x8000000000000000ULL) == 63U);
}

TEST_CASE("BitUtil: Log2Floor", "[common]")
{
    STATIC_REQUIRE(Log2Floor32(1U) == 0U);
    STATIC_REQUIRE(Log2Floor32(2U) == 1U);
    STATIC_REQUIRE(Log2Floor32(3U) == 1U);
    STATIC_REQUIRE(Log2Floor32(4U) == 2U);
    STATIC_REQUIRE(Log2Floor32(1023U) == 9U);
    STATIC_REQUIRE(Log2Floor32(1024U) == 10U);

    STATIC_REQUIRE(Log2Floor64(1024ULL) == 10U);
    STATIC_REQUIRE(Log2Floor64(0x8000000000000000ULL) == 63U);
}

TEST_CASE("BitUtil: Log2Ceil", "[common]")
{
    STATIC_REQUIRE(Log2Ceil32(1U) == 0U);
    STATIC_REQUIRE(Log2Ceil32(2U) == 1U);
    STATIC_REQUIRE(Log2Ceil32(3U) == 2U);
    STATIC_REQUIRE(Log2Ceil32(4U) == 2U);
    STATIC_REQUIRE(Log2Ceil32(5U) == 3U);
    STATIC_REQUIRE(Log2Ceil32(1024U) == 10U);
    STATIC_REQUIRE(Log2Ceil32(1025U) == 11U);

    STATIC_REQUIRE(Log2Ceil64(1024ULL) == 10U);
    STATIC_REQUIRE(Log2Ceil64(1025ULL) == 11U);
}

TEST_CASE("BitUtil: IsPow2", "[common]")
{
    STATIC_REQUIRE(IsPow2(1U));
    STATIC_REQUIRE(IsPow2(2U));
    STATIC_REQUIRE(IsPow2(256U));
    STATIC_REQUIRE_FALSE(IsPow2(0U));
    STATIC_REQUIRE_FALSE(IsPow2(3U));
    STATIC_REQUIRE_FALSE(IsPow2(255U));
}

TEST_CASE("BitUtil: NextPow2", "[common]")
{
    REQUIRE(NextPow2(1U) == 1U);
    REQUIRE(NextPow2(2U) == 2U);
    REQUIRE(NextPow2(3U) == 4U);
    REQUIRE(NextPow2(5U) == 8U);
    REQUIRE(NextPow2(1024U) == 1024U);
    REQUIRE(NextPow2(1025U) == 2048U);
}

TEST_CASE("BitUtil: Bit", "[common]")
{
    STATIC_REQUIRE(Bit<0>(0b0001U));
    STATIC_REQUIRE_FALSE(Bit<1>(0b0001U));
    STATIC_REQUIRE(Bit<3>(0b1000U));
    STATIC_REQUIRE_FALSE(Bit<2>(0b1000U));
}

} // namespace Common
