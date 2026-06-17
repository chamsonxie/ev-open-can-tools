#pragma once

#include "can_frame_types.h"
#include <cstdint>

// Intel (little-endian) signal extraction
inline uint64_t extractIntel(const uint8_t *data, int start, int len)
{
    uint64_t v = 0;
    for (int i = 0; i < len; i++)
    {
        int b = start + i;
        if (data[b / 8] & (1 << (b % 8)))
            v |= (1ULL << i);
    }
    return v;
}

// Motorola (big-endian) signal extraction
inline uint64_t extractMotorola(const uint8_t *data, int start, int len)
{
    uint64_t v = 0;
    for (int i = 0; i < len; i++)
    {
        int m = start + i;
        int physBit = (m / 8) * 8 + (7 - (m % 8));
        if (data[physBit / 8] & (1 << (physBit % 8)))
            v |= (1ULL << i);
    }
    return v;
}

inline int64_t extractSigned(uint64_t raw, int len)
{
    if (raw & (1ULL << (len - 1)))
        return (int64_t)(raw | (~0ULL << len));
    return (int64_t)raw;
}

// 0x118 DI_systemStatus: byte 2 bits 5-7 = DI_gear
// 0=INVALID, 1=P, 2=R, 3=N, 4=D, 7=SNA
inline uint8_t readDIGear(const CanFrame &frame)
{
    return static_cast<uint8_t>((frame.data[2] >> 5) & 0x07);
}

inline bool isVehicleParked(uint8_t gear)
{
    return gear == 0 || gear == 1 || gear == 7;
}

inline void setBit(CanFrame &frame, int bit, bool value)
{
    if (bit < 0 || bit >= 64)
        return;
    int byteIndex = bit / 8;
    int bitIndex = bit % 8;
    uint8_t mask = static_cast<uint8_t>(1U << bitIndex);
    if (value)
        frame.data[byteIndex] |= mask;
    else
        frame.data[byteIndex] &= static_cast<uint8_t>(~mask);
}
