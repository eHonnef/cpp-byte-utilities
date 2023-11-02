/**
 * @file ByteUtilities.hpp
 * @author eHonnef (contact@honnef.dev)
 * @brief Byte operations and utilities.
 * @version 0.0.1
 * @date 2023-01-21
 *
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 ********************************************************************************/
#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

/**
 * @brief Static class containing the static utility functions for bytes/bits operations.
 * All functions does not throw exceptions.
 *
 */
class ByteUtilities {
public:
    /*****************************************************************************************************
     * Bits operation section
     *****************************************************************************************************/

    /**
     * @brief Create a Bit Mask integer. Does not throw exception.
     * Usage Example: aInt & CreateBitMask(0, 10). // Will mask the first 10 bits of aInt
     *
     * @tparam T Type of the bitmask, should be any integer
     * @param uPos Position of the first bit to make the mask.
     * @param uLen Number of bits to include in the bitmask.
     * @return T The bit mask.
     */
    template<typename T>
    static inline T CreateBitMask(size_t uPos, size_t uLen) noexcept {
        static_assert(std::is_integral<T>::value, "T should be an integer");

        return ((T(1u) << uLen) - T(1u)) << uPos;
    }

    /**
     * @brief Create a Bit Mask integer in compile time.
     * Usage Example: aInt & CreateBitMask_<int, 0, 10>::Mask. // Will mask the first 10 bits of aInt
     *
     * @tparam _uPos Position of the first bit to make the mask.
     * @tparam _uLen Number of bits to include in the bitmask.
     * @tparam T Type of the bitmask, should be any integer.
     */
    template<size_t _uPos, size_t _uLen, typename T>
    struct CreateBitMask_ {
        enum {
            Mask = ((T(1u) << _uLen) - T(1u)) << _uPos
        };
    };

    /**
     * @brief Return an integer with the slice of bits of @ref nInt shifted to the right. Does not
     * throw exception. Usage example: GetBitSlice(0x37AB, 9, 5) will return 0x001B. Which the binary
     * will look like: 0011 0111 1010 1011 -> 0000 0000 0001 1011
     *
     * @tparam T Type of the integer, should be any integer.
     * @param nInt Integer to slice.
     * @param uPos Position of the first bit to slice.
     * @param uLen Slice length.
     * @return T The integer with the slice in the least significant bits.
     */
    template<typename T>
    static inline T GetBitSlice(T nInt, size_t uPos, size_t uLen) noexcept {
        static_assert(std::is_integral<T>::value, "T should be an integer");

        return (nInt & CreateBitMask<T>(uPos, uLen)) >> uPos;
    }

    /**
     * @brief Returns the bit value at @ref uPos. Does not throw exception.
     *
     * @tparam T Integer's type.
     * @param nInt Integer to get the bit value from.
     * @param uPos Bit position to get the value from.
     * @return true If the bit value is 1 (one).
     * @return false If the bit value is 0 (zero).
     */
    template<typename T>
    static inline bool GetBit(T nInt, size_t uPos) noexcept {
        static_assert(std::is_integral<T>::value, "T should be an integer");

        return (nInt >> uPos) & T(1u);
    }

    /**
     * @brief Set the value of a single bit of @ref nInt at @ref uPos using @ref bBitValue. Inplace
     * operation. Does not throw exception.
     *
     * @tparam T Integer's type. Inplace operation, this integer will be changed.
     * @param nInt Integer to set the bit value. Inplace operation, this integer will be changed.
     * @param uPos Bit position to set the value
     * @param bBitValue Bit value to be set, true (or 1) will set the bit to 1 (one), false will set
     * the bit to 0 (zero).
     */
    template<typename T>
    static inline void SetBit(T &nInt, size_t uPos, bool bBitValue) noexcept {
        static_assert(std::is_integral<T>::value, "T should be an integer");

        nInt ^= (-(!!T(bBitValue)) ^ nInt) & (T(1u) << uPos);
    }

    /**
     * @brief Flips the bit value of @ref nInt at @ref uPos. Inplace operation. Does not throw
     * exception.
     *
     * @tparam T Integer's type.
     * @param nInt Integer to set the bit value. Inplace operation, this integer will be changed.
     * @param uPos Bit position to flip.
     */
    template<typename T>
    static inline void FlipBit(T &nInt, size_t uPos) noexcept {
        static_assert(std::is_integral<T>::value, "T should be an integer");

        nInt ^= T(1u) << uPos;
    }

    /*****************************************************************************************************
     * Bytes operation section
     *****************************************************************************************************/

    /**
     * @brief Returns the byte at Nth position of the integer @ref nInt. Does not throw exception.
     * Usage example: GetByte(0xAB01CD, 1) // Will return 0x01.
     *
     * @tparam T Type of the integer, should be any integer.
     * @param nInt Integer to get the byte from.
     * @param uPos The byte position.
     * @return uint8_t The byte from @ref nInt.
     */
    template<typename T>
    static inline uint8_t GetByte(T nInt, size_t uPos) noexcept {
        static_assert(std::is_integral<T>::value, "T should be an integer");

        return (nInt >> (8 * uPos)) & 0xff;
    }

    /**
     * @brief Returns the byte at Nth position of the integer @ref nInt. Creates the mask in compile
     * time, apply the mask in runtime. Does not throw exception.
     * Usage example: GetByte<1>(0xAB01CD) // Will return 0x01.
     *
     * @tparam _uPos The byte position.
     * @tparam T Type of the integer, should be any integer.
     * @param nInt Integer to get the byte from.
     * @return uint8_t The byte from @ref nInt.
     */
    template<size_t _uPos, typename T>
    static inline uint8_t GetByte(T nInt) noexcept {
        static_assert(SanityCheck<(_uPos * 8), (8 - 1), T>::Value,
                      "T should be an integer || [_uPos] is out of bounds");

        return (nInt & CreateBitMask_<_uPos * 8, 8, T>::Mask) >> (_uPos * 8);
    }

    /**
     * @brief Will set the value of @ref nInt. Inplace operation. Does not throw exception.
     * Usage example: int nInt = 0xAB01CD; SetBye<1>(nInt, 0xFF); // Will set 0xAB01CD to 0xABFFCD.
     *
     * @tparam _uPos Byte position.
     * @tparam T Integer's type.
     * @param[out] nInt Integer to set byte, Inplace operation, this integer will be changed.
     * @param uByteValue Byte value to be written.
     */
    template<size_t _uPos, typename T>
    static inline void SetByte(T &nInt, uint8_t uByteValue) noexcept {
        static_assert(SanityCheck<(_uPos * 8), (8 - 1), T>::Value,
                      "T should be an integer || [_uPos] is out of bounds");

        nInt |= (CreateBitMask_<_uPos * 8, 8, T>::Mask & (static_cast<T>(uByteValue) << _uPos * 8));
    }

public:
    ByteUtilities() = delete;

    ByteUtilities(const ByteUtilities &) = delete;

    ByteUtilities &operator=(const ByteUtilities &) = delete;

private:
    /**
     * @brief Internal usage. Checks in compile time for range and if the given type is an integer.
     *
     */
    template<size_t _uPos, size_t _uLen, typename T>
    struct SanityCheck {
        enum {
            Value = _uLen > 0 && (_uPos + _uLen - 1) < (sizeof(T) * 8) && std::is_integral<T>::value
        };
    };

}; // class ByteUtilities
