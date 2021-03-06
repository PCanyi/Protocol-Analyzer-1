// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// ============================================================================
// Copyright (c) 2017-2019, by Vitaly Grigoriev, <Vit.link420@gmail.com>.
// This file is part of ProtocolAnalyzer open source project under MIT License.
// ============================================================================


#include <utility>  // std::swap.

#include "../../include/framework/BinaryDataEngine.hpp"


namespace analyzer::framework::common::types
{
    /* ************************************************************************************************************* */
    /* ************************************************** Support ************************************************** */

    /**
     * @brief Support function that performs left byte round shift to the selected byte.
     *
     * @param [in] head - Pointer to start position in byte sequence.
     * @param [in] end - Pointer to end position in byte sequence (the element following the last one).
     * @param [in] newHead - Byte to which the left round byte shift is performed.
     */
    static void leftRoundBytesShiftBE (std::byte* head, const std::byte* end, std::byte* newHead) noexcept
    {
        if (head >= end || newHead <= head || newHead >= end) { return; }
        std::byte* next = newHead;

        while (head != next)
        {
            std::swap(*head++, *next++);
            if (next == end) {
                next = newHead;
            } else if (head == newHead) {
                newHead = next;
            }
        }
    }

    /**
     * @brief Support function that performs right byte round shift to the selected byte.
     *
     * @param [in] head - Pointer to start position in byte sequence.
     * @param [in] end - Pointer to end position in byte sequence (the element following the last one).
     * @param [in] newEnd - Byte to which the right round byte shift is performed.
     */
    static void rightRoundBytesShiftBE (const std::byte* head, std::byte* end, std::byte* newEnd) noexcept
    {
        if (head >= end || newEnd < head || (newEnd >= --end)) { return; }
        std::byte* prev = newEnd;
        const std::byte* const rend = std::prev(head);

        while (end != prev)
        {
            std::swap(*prev--, *end--);
            if (prev == rend) {
                prev = newEnd;
            } else if (end == newEnd) {
                newEnd = prev;
            }
        }
    }

    /**
     * @brief Support function that performs left byte direct shift to the selected byte in big-endian format.
     *
     * @param [in] head - Pointer to start position in byte sequence.
     * @param [in] end - Pointer to end position in byte sequence (the element following the last one).
     * @param [in] newHead - Byte to which the left direct byte shift is performed.
     * @param [in] fillByte - Value of the fill byte after the left byte shift. Default: 0x00.
     */
    static void leftDirectBytesShiftBE (std::byte* head, const std::byte* end, std::byte* newHead, const std::byte fillByte = LowByte) noexcept
    {
        if (head >= end || newHead <= head || newHead >= end) { return; }
        std::byte* next = newHead;

        while (next != end)
        {
            *head++ = *next++;
            if (head == newHead) {
                newHead = next;
            }
        }
        memset(head, static_cast<int32_t>(fillByte), static_cast<std::size_t>(end - head));
    }

    /**
     * @brief Support function that performs right byte direct shift to the selected byte in big-endian format.
     *
     * @param [in] head - Pointer to start position in byte sequence.
     * @param [in] end - Pointer to end position in byte sequence (the element following the last one).
     * @param [in] newEnd - Byte to which the right direct byte shift is performed.
     * @param [in] fillByte - Value of the fill byte after the right byte shift. Default: 0x00.
     */
    static void rightDirectBytesShiftBE (std::byte* head, std::byte* end, std::byte* newEnd, const std::byte fillByte = LowByte) noexcept
    {
        if (head >= end || newEnd < head || (newEnd >= --end)) { return; }
        std::byte* prev = newEnd;
        const std::byte* const rend = std::prev(head);

        while (prev != rend)
        {
            *end-- = *prev--;
            if (end == newEnd) {
                newEnd = prev;
            }
        }
        memset(head, static_cast<int32_t>(fillByte), static_cast<std::size_t>(end - prev));
    }

    /**
     * @brief Support function that performs left byte round shift by a specified byte offset in big-endian format.
     *
     * @param [in] head - Start position of the byte sequence.
     * @param [in] end - End position of the byte sequence (the element following the last one).
     * @param [in] shift - Byte offset for left round byte shift.
     */
    static inline void leftRoundBytesShiftBE (std::byte* head, const std::byte* end, const std::size_t shift) noexcept
    {
        leftRoundBytesShiftBE(head, end, head + shift);
    }

    /**
     * @brief Support function that performs left byte round shift by a specified byte offset in little-endian format.
     *
     * @param [in] head - Pointer to start position in byte sequence.
     * @param [in] end - Pointer to end position in byte sequence (the element following the last one).
     * @param [in] shift - Byte offset for left round byte shift.
     */
    static inline void leftRoundBytesShiftLE (const std::byte* head, std::byte* end, const std::size_t shift) noexcept
    {
        rightRoundBytesShiftBE(head, end, end - shift - 1);
    }

    /**
     * @brief Support function that performs right byte round shift by a specified byte offset in big-endian format.
     *
     * @param [in] head - Pointer to start position in byte sequence.
     * @param [in] end - Pointer to end position in byte sequence (the element following the last one).
     * @param [in] shift - Byte offset for right round byte shift.
     */
    static inline void rightRoundBytesShiftBE (const std::byte* head, std::byte* end, const std::size_t shift) noexcept
    {
        rightRoundBytesShiftBE(head, end, end - shift - 1);
    }

    /**
     * @brief Support function that performs right byte round shift by a specified byte offset in little-endian format.
     *
     * @param [in] head - Pointer to start position in byte sequence.
     * @param [in] end - Pointer to end position in byte sequence (the element following the last one).
     * @param [in] shift - Byte offset for right round byte shift.
     */
    static inline void rightRoundBytesShiftLE (std::byte* head, const std::byte* end, const std::size_t shift) noexcept
    {
        leftRoundBytesShiftBE(head, end, head + shift);
    }

    /**
     * @brief Support function that performs left byte direct shift by a specified byte offset in big-endian format.
     *
     * @param [in] head - Pointer to start position in byte sequence.
     * @param [in] end - Pointer to end position in byte sequence (the element following the last one).
     * @param [in] shift - Byte offset for left direct byte shift.
     * @param [in] fillByte - Value of the fill byte after the left direct byte shift. Default: 0x00.
     */
    static inline void leftDirectBytesShiftBE (std::byte* head, const std::byte* end, const std::size_t shift, const std::byte fillByte = LowByte) noexcept
    {
        leftDirectBytesShiftBE(head, end, head + shift, fillByte);
    }

    /**
     * @brief Support function that performs left byte direct shift by a specified byte offset in little-endian format.
     *
     * @param [in] head - Pointer to start position in byte sequence.
     * @param [in] end - Pointer to end position in byte sequence (the element following the last one).
     * @param [in] shift - Byte offset for left direct byte shift.
     * @param [in] fillByte - Value of the fill byte after the left direct byte shift. Default: 0x00.
     */
    static inline void leftDirectBytesShiftLE (std::byte* head, std::byte* end, const std::size_t shift, const std::byte fillByte = LowByte) noexcept
    {
        rightDirectBytesShiftBE(head, end, end - shift - 1, fillByte);
    }

    /**
     * @brief Support function that performs right byte direct shift by a specified byte offset in big-endian format.
     *
     * @param [in] head - Pointer to start position in byte sequence.
     * @param [in] end - Pointer to end position in byte sequence (the element following the last one).
     * @param [in] shift - Byte offset for right direct byte shift.
     * @param [in] fillByte - Value of the fill byte after the right direct byte shift. Default: 0x00.
     */
    static inline void rightDirectBytesShiftBE (std::byte* head, std::byte* end, const std::size_t shift, const std::byte fillByte = LowByte) noexcept
    {
        rightDirectBytesShiftBE(head, end, end - shift - 1, fillByte);
    }

    /**
     * @brief Support function that performs right byte direct shift by a specified byte offset in little-endian format.
     *
     * @param [in] head - Start position of the byte sequence.
     * @param [in] end - End position of the byte sequence (the element following the last one).
     * @param [in] shift - Byte offset for right direct byte shift.
     * @param [in] fillByte - Value of the fill byte after the right direct byte shift. Default: 0x00.
     */
    static inline void rightDirectBytesShiftLE (std::byte* head, const std::byte* end, const std::size_t shift, const std::byte fillByte = LowByte) noexcept
    {
        leftDirectBytesShiftBE(head, end, head + shift, fillByte);
    }

    /* ************************************************** Support ************************************************** */
    /* ************************************************************************************************************* */


    // Method that returns the correct position of selected byte in stored data in any data endian.
    std::size_t BinaryDataEngine::ByteStreamInformationEngine::GetBytePosition (const std::size_t index) const noexcept
    {
        if (storedData->dataEndianType == DATA_LITTLE_ENDIAN || (storedData->dataModeType & DATA_MODE_INDEPENDENT) != 0U)
        {
            return index;
        }
        // If data endian type is DATA_BIG_ENDIAN.
        return storedData->length - index - 1;
    }

    // Method that checks the byte under the specified index.
    bool BinaryDataEngine::ByteStreamInformationEngine::Test (const std::size_t index, const std::byte value) const noexcept
    {
        return (index < Length() && storedData->data[GetBytePosition(index)] == value);
    }

    // Method that returns byte sequence characteristic when all bytes have specified value in block of stored data.
    bool BinaryDataEngine::ByteStreamInformationEngine::All (std::size_t first, std::size_t last, const std::byte value) const noexcept
    {
        if (last == NPOS) { last = Length() - 1; }
        if (first > last || last >= Length()) { return false; }

        while (first <= last)
        {
            if (Test(first++, value) == false) {
                return false;
            }
        }
        return true;
    }

    // Method that returns byte sequence characteristic when any of the bytes have specified value in block of stored data.
    bool BinaryDataEngine::ByteStreamInformationEngine::Any (std::size_t first, std::size_t last, const std::byte value) const noexcept
    {
        if (last == NPOS) { last = Length() - 1; }
        if (first > last || last >= Length()) { return false; }

        while (first <= last)
        {
            if (Test(first++, value) == true) {
                return true;
            }
        }
        return false;
    }

    // Method that returns byte sequence characteristic when none of the bytes have a specified value in block of stored data.
    bool BinaryDataEngine::ByteStreamInformationEngine::None (std::size_t first, std::size_t last, const std::byte value) const noexcept
    {
        if (last == NPOS) { last = Length() - 1; }
        if (first > last || last >= Length()) { return false; }

        while (first <= last)
        {
            if (Test(first++, value) == true) {
                return false;
            }
        }
        return true;
    }

    // Method that returns a constant pointer to the value of byte under the specified index.
    const std::byte* BinaryDataEngine::ByteStreamInformationEngine::GetAt (const std::size_t index) const noexcept
    {
        return (index < Length() ? &storedData->data[GetBytePosition(index)] : nullptr);
    }


    // Method that sets the byte under the specified index to new value.
    BinaryDataEngine::ByteStreamTransformEngine& BinaryDataEngine::ByteStreamTransformEngine::Set (const std::size_t index, const std::byte fillByte) noexcept
    {
        if (*storedData == true)
        {
            storedData->data[storedData->byteStreamInformation.GetBytePosition(index)] = fillByte;
        }
        return *this;
    }

    // Method that performs direct left byte shift by a specified byte offset.
    BinaryDataEngine::ByteStreamTransformEngine& BinaryDataEngine::ByteStreamTransformEngine::ShiftLeft (const std::size_t shift, const std::byte fillByte) noexcept
    {
        if (*storedData == true && shift != 0)
        {
            if (shift >= storedData->length) {
                memset(storedData->data.get(), static_cast<int32_t>(fillByte), storedData->length);
                return *this;
            }

            if ((storedData->dataModeType & DATA_MODE_DEPENDENT) != 0U && storedData->dataEndianType == DATA_LITTLE_ENDIAN)
            {
                leftDirectBytesShiftLE(storedData->data.get(), storedData->data.get() + storedData->length, shift, fillByte);
            }
            else {  // If data endian type is DATA_BIG_ENDIAN or DATA_REVERSE_BIG_ENDIAN or if data handling mode type is DATA_MODE_INDEPENDENT.
                leftDirectBytesShiftBE(storedData->data.get(), storedData->data.get() + storedData->length, shift, fillByte);
            }
        }
        return *this;
    }

    // Method that performs direct right byte shift by a specified byte offset.
    BinaryDataEngine::ByteStreamTransformEngine& BinaryDataEngine::ByteStreamTransformEngine::ShiftRight (const std::size_t shift, const std::byte fillByte) noexcept
    {
        if (*storedData == true && shift != 0)
        {
            if (shift >= storedData->length) {
                memset(storedData->data.get(), static_cast<int32_t>(fillByte), storedData->length);
                return *this;
            }

            if ((storedData->dataModeType & DATA_MODE_DEPENDENT) != 0U && storedData->dataEndianType == DATA_LITTLE_ENDIAN) {
                rightDirectBytesShiftLE(storedData->data.get(), storedData->data.get() + storedData->length, shift, fillByte);
            }
            else {  // If data endian type is DATA_BIG_ENDIAN or if data handling mode type is DATA_MODE_INDEPENDENT.
                rightDirectBytesShiftBE(storedData->data.get(), storedData->data.get() + storedData->length, shift, fillByte);
            }
        }
        return *this;
    }

    // Method that performs round left bit shift by a specified byte offset.
    BinaryDataEngine::ByteStreamTransformEngine& BinaryDataEngine::ByteStreamTransformEngine::RoundShiftLeft (std::size_t shift) noexcept
    {
        if (*storedData == true && shift != 0)
        {
            if (shift >= storedData->length) {
                shift %= storedData->length;
            }

            if ((storedData->dataModeType & DATA_MODE_DEPENDENT) != 0U && storedData->dataEndianType == DATA_LITTLE_ENDIAN) {
                leftRoundBytesShiftLE(storedData->data.get(), storedData->data.get() + storedData->length, shift);
            }
            else {  // If data endian type is DATA_BIG_ENDIAN or if data handling mode type is DATA_MODE_INDEPENDENT.
                leftRoundBytesShiftBE(storedData->data.get(), storedData->data.get() + storedData->length, shift);
            }
        }
        return *this;
    }

    // Method that performs round right bit shift by a specified byte offset.
    BinaryDataEngine::ByteStreamTransformEngine& BinaryDataEngine::ByteStreamTransformEngine::RoundShiftRight (std::size_t shift) noexcept
    {
        if (*storedData == true && shift != 0)
        {
            if (shift >= storedData->length) {
                shift %= storedData->length;
            }

            if ((storedData->dataModeType & DATA_MODE_DEPENDENT) != 0U && storedData->dataEndianType == DATA_LITTLE_ENDIAN) {
                rightRoundBytesShiftLE(storedData->data.get(), storedData->data.get() + storedData->length, shift);
            }
            else {  // If data endian type is DATA_BIG_ENDIAN or if data handling mode type is DATA_MODE_INDEPENDENT.
                rightRoundBytesShiftBE(storedData->data.get(), storedData->data.get() + storedData->length, shift);
            }
        }
        return *this;
    }

    // Method that returns a pointer to the value of byte under the specified index.
    std::byte* BinaryDataEngine::ByteStreamTransformEngine::GetAt (const std::size_t index) noexcept
    {
        return (index < Length() ? &storedData->data[storedData->byteStreamInformation.GetBytePosition(index)] : nullptr);
    }

}  // namespace types.
