// SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
// SPDX-License-Identifier: GPL-3.0-or-later

#include "common/alignment.h"

#include <catch2/catch_test_macros.hpp>

#include "common/common_types.h"

namespace Common {

TEST_CASE("Alignment: AlignUp", "[common]")
{
    STATIC_REQUIRE(AlignUp(0U, 16) == 0U);
    STATIC_REQUIRE(AlignUp(1U, 16) == 16U);
    STATIC_REQUIRE(AlignUp(16U, 16) == 16U);
    STATIC_REQUIRE(AlignUp(17U, 16) == 32U);
    STATIC_REQUIRE(AlignUp(0x1000U, 0x1000) == 0x1000U);
    STATIC_REQUIRE(AlignUp(0x1001U, 0x1000) == 0x2000U);
    // Non-power-of-two alignment is supported.
    STATIC_REQUIRE(AlignUp(10U, 3) == 12U);
}

TEST_CASE("Alignment: AlignDown", "[common]")
{
    STATIC_REQUIRE(AlignDown(0U, 16) == 0U);
    STATIC_REQUIRE(AlignDown(15U, 16) == 0U);
    STATIC_REQUIRE(AlignDown(16U, 16) == 16U);
    STATIC_REQUIRE(AlignDown(31U, 16) == 16U);
    STATIC_REQUIRE(AlignDown(0x1FFFU, 0x1000) == 0x1000U);
    STATIC_REQUIRE(AlignDown(10U, 3) == 9U);
}

TEST_CASE("Alignment: AlignUpLog2", "[common]")
{
    STATIC_REQUIRE(AlignUpLog2(0U, 4) == 0U); // align to 16
    STATIC_REQUIRE(AlignUpLog2(1U, 4) == 16U);
    STATIC_REQUIRE(AlignUpLog2(16U, 4) == 16U);
    STATIC_REQUIRE(AlignUpLog2(17U, 4) == 32U);
}

TEST_CASE("Alignment: IsAligned", "[common]")
{
    STATIC_REQUIRE(IsAligned(0U, 16));
    STATIC_REQUIRE(IsAligned(16U, 16));
    STATIC_REQUIRE_FALSE(IsAligned(15U, 16));
    STATIC_REQUIRE_FALSE(IsAligned(17U, 16));
}

TEST_CASE("Alignment: Is4KBAligned and IsWordAligned", "[common]")
{
    STATIC_REQUIRE(Is4KBAligned(0x1000U));
    STATIC_REQUIRE(Is4KBAligned(0x2000U));
    STATIC_REQUIRE_FALSE(Is4KBAligned(0x1001U));

    STATIC_REQUIRE(IsWordAligned(0U));
    STATIC_REQUIRE(IsWordAligned(4U));
    STATIC_REQUIRE_FALSE(IsWordAligned(1U));
    STATIC_REQUIRE_FALSE(IsWordAligned(2U));
}

TEST_CASE("Alignment: DivideUp", "[common]")
{
    STATIC_REQUIRE(DivideUp(0U, 4U) == 0U);
    STATIC_REQUIRE(DivideUp(1U, 4U) == 1U);
    STATIC_REQUIRE(DivideUp(4U, 4U) == 1U);
    STATIC_REQUIRE(DivideUp(5U, 4U) == 2U);
    STATIC_REQUIRE(DivideUp(8U, 4U) == 2U);
}

TEST_CASE("Alignment: bit helpers", "[common]")
{
    STATIC_REQUIRE(LeastSignificantOneBit(0b10110U) == 0b00010U);
    STATIC_REQUIRE(LeastSignificantOneBit(0b10000U) == 0b10000U);
    STATIC_REQUIRE(ResetLeastSignificantOneBit(0b10110U) == 0b10100U);
    STATIC_REQUIRE(ResetLeastSignificantOneBit(0b10000U) == 0b00000U);

    STATIC_REQUIRE(IsPowerOfTwo(1U));
    STATIC_REQUIRE(IsPowerOfTwo(2U));
    STATIC_REQUIRE(IsPowerOfTwo(1024U));
    STATIC_REQUIRE_FALSE(IsPowerOfTwo(0U));
    STATIC_REQUIRE_FALSE(IsPowerOfTwo(3U));

    STATIC_REQUIRE(FloorPowerOfTwo(1U) == 1U);
    STATIC_REQUIRE(FloorPowerOfTwo(17U) == 16U);
    STATIC_REQUIRE(FloorPowerOfTwo(32U) == 32U);
    STATIC_REQUIRE(FloorPowerOfTwo(33U) == 32U);
}

} // namespace Common
