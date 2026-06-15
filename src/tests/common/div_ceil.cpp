// SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
// SPDX-License-Identifier: GPL-3.0-or-later

#include "common/div_ceil.h"

#include <catch2/catch_test_macros.hpp>

#include "common/common_types.h"

namespace Common {

TEST_CASE("DivCeil: basic", "[common]")
{
    STATIC_REQUIRE(DivCeil(0U, 4U) == 0U);
    STATIC_REQUIRE(DivCeil(1U, 4U) == 1U);
    STATIC_REQUIRE(DivCeil(3U, 4U) == 1U);
    STATIC_REQUIRE(DivCeil(4U, 4U) == 1U);
    STATIC_REQUIRE(DivCeil(5U, 4U) == 2U);
    STATIC_REQUIRE(DivCeil(8U, 4U) == 2U);
    STATIC_REQUIRE(DivCeil(9U, 4U) == 3U);
}

TEST_CASE("DivCeil: divisor of one", "[common]")
{
    STATIC_REQUIRE(DivCeil(0U, 1U) == 0U);
    STATIC_REQUIRE(DivCeil(7U, 1U) == 7U);
    STATIC_REQUIRE(DivCeil(123456U, 1U) == 123456U);
}

TEST_CASE("DivCeilLog2: basic", "[common]")
{
    // alignment_log2 == 2  ->  divisor 4
    STATIC_REQUIRE(DivCeilLog2(0U, 2U) == 0U);
    STATIC_REQUIRE(DivCeilLog2(1U, 2U) == 1U);
    STATIC_REQUIRE(DivCeilLog2(4U, 2U) == 1U);
    STATIC_REQUIRE(DivCeilLog2(5U, 2U) == 2U);
    STATIC_REQUIRE(DivCeilLog2(8U, 2U) == 2U);

    // alignment_log2 == 12 ->  divisor 4096 (page size)
    STATIC_REQUIRE(DivCeilLog2(0x1000U, 12U) == 1U);
    STATIC_REQUIRE(DivCeilLog2(0x1001U, 12U) == 2U);
}

TEST_CASE("DivCeil matches DivCeilLog2 for power-of-two divisors", "[common]")
{
    STATIC_REQUIRE(DivCeil(5U, 4U) == DivCeilLog2(5U, 2U));
    STATIC_REQUIRE(DivCeil(0x1001U, 0x1000U) == DivCeilLog2(0x1001U, 12U));
}

} // namespace Common
