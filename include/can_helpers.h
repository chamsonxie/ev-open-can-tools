#pragma once

#include "can_frame_types.h"

inline uint8_t readMuxID(const CanFrame &frame)
{
    return frame.data[0] & 0x07;
}

inline bool isADSelectedInUI(const CanFrame &frame)
{
    return (frame.data[4] >> 5) & 0x01;
}

inline uint8_t readGTWAutopilot(const CanFrame &frame)
{
    return static_cast<uint8_t>((frame.data[5] >> 2) & 0x07);
}

inline uint8_t readDASAutopilotStatus(const CanFrame &frame)
{
    return frame.data[0] & 0x0F;
}

inline bool isDASAutopilotActive(uint8_t status)
{
    return status >= 3 && status <= 5;
}

inline uint8_t readVehicleGear(const CanFrame &frame)
{
    return static_cast<uint8_t>((frame.data[7] >> 3) & 0x07);
}

// DI_systemStatus（CAN ID 280 / 0x118）DI_gear：字节2的第5-7位
// 值：0=无效, 1=P, 2=R, 3=N, 4=D, 7=SNA
inline uint8_t readDIGear(const CanFrame &frame)
{
    return static_cast<uint8_t>((frame.data[2] >> 5) & 0x07);
}

inline bool isVehicleParked(uint8_t gear)
{
    // 将真正的驻车（1）视为已驻车。同时将无效（0）和SNA（7）视为已驻车：
    // 当DI休眠时（例如车辆锁闭且哨兵模式开启），它报告SNA，
    // 我们希望AP注入门打开，以便在冷接近时运行召唤解锁注入。
    // 行驶状态（R=2, N=3, D=4）和未知的中间值不被视为已驻车。
    return gear == 0 || gear == 1 || gear == 7;
}

inline const char *describeGTWAutopilot(uint8_t value)
{
    switch (value)
    {
    case 0:
        return "NONE";
    case 1:
        return "HIGHWAY";
    case 2:
        return "ENHANCED";
    case 3:
        return "SELF_DRIVING";
    case 4:
        return "BASIC";
    default:
        return "UNKNOWN";
    }
}

inline void setBit(CanFrame &frame, int bit, bool value)
{
    if (bit < 0 || bit >= 64)
        return; // 边界保护：CanFrame.data为8字节
    int byteIndex = bit / 8;
    int bitIndex = bit % 8;
    uint8_t mask = static_cast<uint8_t>(1U << bitIndex);
    if (value)
    {
        frame.data[byteIndex] |= mask;
    }
    else
    {
        frame.data[byteIndex] &= static_cast<uint8_t>(~mask);
    }
}
