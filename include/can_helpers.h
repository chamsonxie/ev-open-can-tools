#pragma once

#include "can_frame_types.h"
#include <cstdint>

// ── Signal extraction primitives ──

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

// ── Unified signal parsing structs ──

struct DI_systemStatus_s
{
    uint8_t gear;
    uint8_t accelPedalPos;
    uint8_t regenLight;
    uint8_t brakePedalState;
    uint8_t systemState;
    uint8_t tractionControlMode;
    uint8_t epbRequest;
    uint8_t trackModeState;
    uint8_t immobilizerState;
    uint8_t proximity;
    uint8_t keepAliveRequest;
};

struct DI_speed_s
{
    uint16_t vehicleSpeed;
    uint8_t uiSpeed;
    uint8_t uiSpeedUnits;
};

struct DAS_status2_s
{
    uint8_t longCollisionWarning;
    int16_t ppOffsetDesiredRamp;
    uint8_t driverInteractionLevel;
    uint8_t robState;
    uint8_t radarTelemetry;
    uint8_t lssState;
    uint8_t accReport;
    uint8_t pmmCameraFaultReason;
    uint8_t pmmSysFaultReason;
    uint8_t pmmRadarFaultReason;
    uint8_t pmmUltrasonicsFaultReason;
    uint8_t activationFailureStatus;
    uint8_t pmmLoggingRequest;
    uint8_t pmmObstacleSeverity;
    uint16_t accSpeedLimit;
};

struct IBST_status_s
{
    uint8_t driverBrakeApply;
    int16_t brakeRodTravel;
    uint8_t internalState;
    uint8_t iBoosterStatus;
};

struct VCFRONT_lighting_s
{
    uint8_t indicatorLeftRequest;
    uint8_t indicatorRightRequest;
    uint8_t hazardLightRequest;
    uint8_t lowBeamLeftStatus;
    uint8_t lowBeamRightStatus;
    uint8_t highBeamLeftStatus;
    uint8_t highBeamRightStatus;
    uint8_t drlLeftStatus;
    uint8_t drlRightStatus;
    uint8_t turnSignalLeftStatus;
    uint8_t turnSignalRightStatus;
    uint8_t parkLeftStatus;
    uint8_t parkRightStatus;
    uint8_t fogLeftStatus;
    uint8_t fogRightStatus;
    uint8_t sideRepeaterLeftStatus;
    uint8_t sideRepeaterRightStatus;
};

struct E32_brakeLight_s
{
    uint8_t brakeLight;  // 0=off, 1=on (data[0] low nibble 0xC→0xD)
};

// ── Unified signal parsing functions ──

inline DI_systemStatus_s parseDI_systemStatus(const uint8_t *data, uint8_t dlc)
{
    DI_systemStatus_s s = {};
    if (dlc < 7) return s;
    s.gear              = (data[2] >> 5) & 0x07;
    s.accelPedalPos     = data[4];
    s.regenLight        = (data[3] >> 2) & 0x01;
    s.brakePedalState   = (data[2] >> 3) & 0x03;
    s.systemState       = data[2] & 0x07;
    s.tractionControlMode = data[5] & 0x07;
    s.epbRequest        = (data[5] >> 4) & 0x03;
    s.trackModeState    = data[6] & 0x03;
    s.immobilizerState  = (data[3] >> 3) & 0x07;
    s.proximity         = (data[5] >> 6) & 0x01;
    s.keepAliveRequest  = (data[5] >> 7) & 0x01;
    return s;
}

inline DI_speed_s parseDI_speed(const uint8_t *data, uint8_t dlc)
{
    DI_speed_s s = {};
    if (dlc < 5) return s;
    s.vehicleSpeed  = (uint16_t)extractIntel(data, 12, 12);
    s.uiSpeed       = data[3];
    s.uiSpeedUnits  = data[4] & 0x01;
    return s;
}

inline DAS_status2_s parseDAS_status2(const uint8_t *data, uint8_t dlc)
{
    DAS_status2_s s = {};
    if (dlc < 7) return s;
    s.longCollisionWarning      = (uint8_t)extractIntel(data, 48, 4);
    s.ppOffsetDesiredRamp       = (int16_t)extractIntel(data, 40, 8);
    s.driverInteractionLevel    = (uint8_t)extractIntel(data, 38, 2);
    s.robState                  = (uint8_t)extractIntel(data, 36, 2);
    s.radarTelemetry            = (uint8_t)extractIntel(data, 34, 2);
    s.lssState                  = (uint8_t)extractIntel(data, 31, 3);
    s.accReport                 = (uint8_t)extractIntel(data, 26, 5);
    s.pmmCameraFaultReason      = (uint8_t)extractIntel(data, 24, 2);
    s.pmmSysFaultReason         = (uint8_t)extractIntel(data, 21, 3);
    s.pmmRadarFaultReason       = (uint8_t)extractIntel(data, 19, 2);
    s.pmmUltrasonicsFaultReason = (uint8_t)extractIntel(data, 16, 3);
    s.activationFailureStatus   = (uint8_t)extractIntel(data, 14, 2);
    s.pmmLoggingRequest         = (uint8_t)extractIntel(data, 13, 1);
    s.pmmObstacleSeverity       = (uint8_t)extractIntel(data, 10, 3);
    s.accSpeedLimit             = (uint16_t)extractIntel(data, 0, 10);
    return s;
}

inline IBST_status_s parseIBST_status(const uint8_t *data, uint8_t dlc)
{
    IBST_status_s s = {};
    if (dlc < 3) return s;
    s.driverBrakeApply = (uint8_t)extractIntel(data, 16, 2);
    s.brakeRodTravel   = (int16_t)extractIntel(data, 21, 12);
    s.internalState    = (data[2] >> 2) & 0x07;
    s.iBoosterStatus   = (data[1] >> 4) & 0x07;
    return s;
}

inline VCFRONT_lighting_s parseVCFRONT_lighting(const uint8_t *data, uint8_t dlc)
{
    VCFRONT_lighting_s s = {};
    if (dlc < 8) return s;
    s.indicatorLeftRequest   = data[0] & 0x03;
    s.indicatorRightRequest  = (data[0] >> 2) & 0x03;
    s.hazardLightRequest     = (data[0] >> 4) & 0x0F;
    s.lowBeamLeftStatus      = (data[3] >> 4) & 0x03;
    s.lowBeamRightStatus     = (data[3] >> 6) & 0x03;
    s.highBeamLeftStatus     = data[4] & 0x03;
    s.highBeamRightStatus    = (data[4] >> 2) & 0x03;
    s.drlLeftStatus          = (data[4] >> 4) & 0x03;
    s.drlRightStatus         = (data[4] >> 6) & 0x03;
    s.fogLeftStatus          = data[5] & 0x03;
    s.fogRightStatus         = (data[5] >> 2) & 0x03;
    s.sideRepeaterLeftStatus = (data[5] >> 6) & 0x03;
    s.sideRepeaterRightStatus= data[6] & 0x03;
    s.turnSignalLeftStatus   = (data[6] >> 2) & 0x03;
    s.turnSignalRightStatus  = (data[6] >> 4) & 0x03;
    s.parkLeftStatus         = (data[6] >> 6) & 0x03;
    s.parkRightStatus        = data[7] & 0x03;
    return s;
}

inline E32_brakeLight_s parseE32_brakeLight(const uint8_t *data, uint8_t dlc)
{
    E32_brakeLight_s s = {};
    if (dlc < 1) return s;
    s.brakeLight = data[0] & 0x01;
    return s;
}
