/**
 * @file Tests.cpp
 * @brief Unit tests for the file @ref ByteUtilities.hpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <ByteUtilities.hpp>
#include <doctest/doctest.h>

/**************************************************************************************
 * Test Section for [Bit operation]
 **************************************************************************************/
TEST_SUITE("[Bit operation]") {
    TEST_CASE_TEMPLATE("Bit mask test - compile time", TestType, int16_t, int32_t, int64_t,
                       size_t, uint16_t, uint32_t, uint64_t) {
        constexpr TestType nValueA = 0x37AB;
        constexpr TestType nValueB = 0xB7AB;

        // 0011 0111 1010 1011 -> 0011 0110 0000 0000
        REQUIRE((ByteUtilities::CreateBitMask_<9, 5, TestType>::Mask & nValueA) == 0x3600);

        // 0011 0111 1010 1011 -> 0000 0000 0000 0001
        REQUIRE((ByteUtilities::CreateBitMask_<0, 1, TestType>::Mask & nValueA) == 0x0001);

        // 0011 0111 1010 1011 -> 0000 0000 0000 0000
        REQUIRE((ByteUtilities::CreateBitMask_<0, 0, TestType>::Mask & nValueA) == 0x0000);

        // 1011 0111 1010 1011 -> 1000 0000 0000 0000
        REQUIRE((ByteUtilities::CreateBitMask_<15, 1, TestType>::Mask & nValueB) == 0x8000);

        // 0011 0111 1010 1011 -> 0000 0000 0000 0000
        REQUIRE((ByteUtilities::CreateBitMask_<15, 0, TestType>::Mask & nValueB) == 0x0000);
    }

    TEST_CASE("Bit slice test") {
        // 0011 0111 1010 1011 -> 0000 0000 0001 1011
        REQUIRE(ByteUtilities::GetBitSlice(0x37AB, 9, 5) == 0x001B);
        // @todo: more test cases
    }

    TEST_CASE_TEMPLATE("Set bit", TestType, int16_t, int32_t, int64_t, size_t, uint16_t,
                       uint32_t, uint64_t) {
        // 1011 0111 1010 1011
        TestType nValue = 0xB7AB;

        ByteUtilities::SetBit(nValue, 15, false);
        REQUIRE(nValue == TestType(0x37ab));

        ByteUtilities::SetBit(nValue, 15, 1);
        REQUIRE(nValue == TestType(0xB7ab));

        ByteUtilities::SetBit(nValue, 0, false);
        REQUIRE(nValue == TestType(0xB7aa));

        ByteUtilities::SetBit(nValue, 0, true);
        REQUIRE(nValue == TestType(0xB7ab));

        ByteUtilities::SetBit(nValue, 6, true);
        REQUIRE(nValue == TestType(0xB7eb));

        ByteUtilities::SetBit(nValue, 6, 0);
        REQUIRE(nValue == TestType(0xB7ab));

        // Set most significant bit
        nValue = 0;

        ByteUtilities::SetBit(nValue,
                              (sizeof(TestType) * 8) - 1, 1);

        if constexpr (sizeof(TestType) == 2)
            REQUIRE(nValue == TestType(0x80'00));
        else if constexpr (sizeof(TestType) == 4)
            REQUIRE(nValue == TestType(0x80'00'00'00));
        else if constexpr (sizeof(TestType) == 8)
            REQUIRE(nValue == TestType(0x80'00'00'00'00'00'00'00));
        else
            static_assert(std::is_integral<TestType>::value && false, "Missing test case");
    }

    TEST_CASE_TEMPLATE("Get bit", TestType, int16_t, int32_t, int64_t, size_t, uint16_t,
                       uint32_t, uint64_t) {
        // 1011 0111 1010 1011
        TestType nValue = 0xB7AB;

        REQUIRE(ByteUtilities::GetBit(nValue, 0) == 1);
        REQUIRE(ByteUtilities::GetBit(nValue, 2) == 0);
        REQUIRE(ByteUtilities::GetBit(nValue, 15) == true);
        REQUIRE(ByteUtilities::GetBit(nValue, 11) == false);

        if constexpr (sizeof(TestType) == 2) {
            REQUIRE(ByteUtilities::GetBit(TestType(0x8000), 0) == false);
            REQUIRE(ByteUtilities::GetBit(TestType(0x8000), (sizeof(TestType) * 8) - 1) == 1);
        } else if constexpr (sizeof(TestType) == 4) {
            REQUIRE(ByteUtilities::GetBit(TestType(0x80'00'00'00), 0) == false);
            REQUIRE(ByteUtilities::GetBit(TestType(0x80'00'00'00), (sizeof(TestType) * 8) - 1) == 1);
        } else if constexpr (sizeof(TestType) == 8) {
            REQUIRE(ByteUtilities::GetBit(TestType(0x80'00'00'00'00'00'00'00), 0) == false);
            REQUIRE(ByteUtilities::GetBit(TestType(0x80'00'00'00'00'00'00'00),
                                          (sizeof(TestType) * 8) - 1) == 1);
        } else {
            static_assert(std::is_integral<TestType>::value && false, "Missing test case");
        }
    }

    TEST_CASE_TEMPLATE("Flip bit", TestType, int16_t, int32_t, int64_t, size_t, uint16_t,
                       uint32_t, uint64_t) {
        // 1011 0111 1010 1011
        TestType nValue = 0xB7AB;

        ByteUtilities::FlipBit(nValue, 15);
        REQUIRE(nValue == TestType(0x37ab));

        ByteUtilities::FlipBit(nValue, 15);
        REQUIRE(nValue == TestType(0xB7ab));

        ByteUtilities::FlipBit(nValue, 0);
        REQUIRE(nValue == TestType(0xB7aa));

        ByteUtilities::FlipBit(nValue, 0);
        REQUIRE(nValue == TestType(0xB7ab));

        ByteUtilities::FlipBit(nValue, 6);
        REQUIRE(nValue == TestType(0xB7eb));

        ByteUtilities::FlipBit(nValue, 6);
        REQUIRE(nValue == TestType(0xB7ab));

        // Flip most and least significant bit
        nValue = TestType(0x1);
        ByteUtilities::FlipBit(nValue, 0); // turn to 0x0
        ByteUtilities::FlipBit(nValue, (sizeof(TestType) * 8) - 1);
        ByteUtilities::FlipBit(nValue, 3);

        if constexpr (sizeof(TestType) == 2)
            REQUIRE(nValue == TestType(0x80'08));
        else if constexpr (sizeof(TestType) == 4)
            REQUIRE(nValue == TestType(0x80'00'00'08));
        else if constexpr (sizeof(TestType) == 8)
            REQUIRE(nValue == TestType(0x80'00'00'00'00'00'00'08));
        else
            static_assert(std::is_integral<TestType>::value && false, "Missing test case");
    }
}
/**************************************************************************************
 * Test Section for [Byte operation]
 **************************************************************************************/

TEST_SUITE("[Byte operation]") {
    TEST_CASE("Get byte test") {
        // 0xAB CD EF 01 23 45 67 89
        constexpr uint64_t nInt = 0xABCDEF0123456789;

        REQUIRE(ByteUtilities::GetByte(nInt, 0) == 0x89u);
        REQUIRE(ByteUtilities::GetByte(nInt, 1) == 0x67u);
        REQUIRE(ByteUtilities::GetByte(nInt, 2) == 0x45u);
        REQUIRE(ByteUtilities::GetByte(nInt, 3) == 0x23u);
        REQUIRE(ByteUtilities::GetByte(nInt, 4) == 0x01u);
        REQUIRE(ByteUtilities::GetByte(nInt, 5) == 0xEFu);
        REQUIRE(ByteUtilities::GetByte(nInt, 6) == 0xCDu);
        REQUIRE(ByteUtilities::GetByte(nInt, 7) == 0xABu);
    }

    TEST_CASE("Get byte test - compile time") {
        // 0xAB CD EF 01 23 45 67 89
        volatile uint64_t nInt = 0xABCDEF0123456789;

        volatile uint64_t *pInt = new uint64_t();
        *pInt = nInt;

        REQUIRE(ByteUtilities::GetByte<0>(*pInt) == 0x89u);
        REQUIRE(ByteUtilities::GetByte<1>(*pInt) == 0x67u);
        REQUIRE(ByteUtilities::GetByte<2>(*pInt) == 0x45u);
        REQUIRE(ByteUtilities::GetByte<3>(*pInt) == 0x23u);
        REQUIRE(ByteUtilities::GetByte<4>(*pInt) == 0x01u);
        REQUIRE(ByteUtilities::GetByte<5>(*pInt) == 0xEFu);
        REQUIRE(ByteUtilities::GetByte<6>(*pInt) == 0xCDu);
        REQUIRE(ByteUtilities::GetByte<7>(*pInt) == 0xABu);

        delete pInt;
    }

#include "iostream"

    TEST_CASE("Set byte test - compile time") {
        // 0xAB CD EF 01 23 45 67 89
        constexpr uint64_t uResult = 0xABCDEF0123456789;

        volatile uint64_t *pInt = new uint64_t();
        *pInt = 0;

        ByteUtilities::SetByte<0>(*pInt, 0x89);
        ByteUtilities::SetByte<1>(*pInt, 0x67);
        ByteUtilities::SetByte<2>(*pInt, 0x45);
        ByteUtilities::SetByte<3>(*pInt, 0x23);
        ByteUtilities::SetByte<4>(*pInt, 0x01);
        ByteUtilities::SetByte<5>(*pInt, 0xEF);
        ByteUtilities::SetByte<6>(*pInt, 0xCD);
        ByteUtilities::SetByte<7>(*pInt, 0xAB);

        REQUIRE(*pInt == uResult);

        delete pInt;
    }
}
