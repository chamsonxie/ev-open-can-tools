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

struct ID273UI_vehicleControl_s
{
    uint8_t accessoryPowerRequest;     // bit 0,  1
    uint8_t frontFogSwitch;            // bit 3,  1
    uint8_t summonActive;              // bit 4,  1
    uint8_t frunkRequest;              // bit 5,  1
    uint8_t wiperMode;                 // bit 6,  2
    uint8_t steeringBacklightEnabled;  // bit 8,  1
    uint8_t steeringButtonMode;        // bit 9,  3
    uint8_t walkUpUnlock;              // bit 12, 1
    uint8_t walkAwayLock;              // bit 13, 1
    uint8_t unlockOnPark;              // bit 14, 1
    uint8_t globalUnlockOn;            // bit 15, 1
    uint8_t childDoorLockOn;           // bit 16, 1
    uint8_t lockRequest;               // bit 17, 3
    uint8_t alarmEnabled;              // bit 20, 1
    uint8_t intrusionSensorOn;         // bit 21, 1
    uint8_t stop12vSupport;            // bit 22, 1
    uint8_t rearFogSwitch;             // bit 23, 1
    uint8_t mirrorFoldRequest;         // bit 24, 2
    uint8_t mirrorHeatRequest;         // bit 26, 1
    uint8_t remoteStartRequest;        // bit 27, 3
    uint8_t seeYouHomeLightingOn;      // bit 30, 1
    uint8_t powerOff;                  // bit 31, 1
    uint8_t displayBrightnessLevel;    // bit 32, 8
    uint8_t ambientLightingEnabled;    // bit 40, 1
    uint8_t autoHighBeamEnabled;       // bit 41, 1
    uint8_t frontLeftSeatHeatReq;      // bit 42, 2
    uint8_t frontRightSeatHeatReq;     // bit 44, 2
    uint8_t rearLeftSeatHeatReq;       // bit 46, 2
    uint8_t rearCenterSeatHeatReq;     // bit 48, 2
    uint8_t rearRightSeatHeatReq;      // bit 50, 2
    uint8_t autoFoldMirrorsOn;         // bit 52, 1
    uint8_t mirrorDipOnReverse;        // bit 53, 1
    uint8_t remoteClosureRequest;      // bit 54, 2
    uint8_t wiperRequest;              // bit 56, 3
    uint8_t domeLightSwitch;           // bit 59, 2
    uint8_t honkHorn;                  // bit 61, 1
    uint8_t driveStateRequest;         // bit 62, 1
    uint8_t rearWindowLockout;         // bit 63, 1
};

struct ID212BMS_status_s
{
    uint8_t hvacPowerRequest;         // bit 0,  1
    uint8_t notEnoughPowerForDrive;   // bit 1,  1
    uint8_t notEnoughPowerForSupport; // bit 2,  1
    uint8_t preconditionAllowed;      // bit 3,  1
    uint8_t updateAllowed;            // bit 4,  1
    uint8_t activeHeatingWorthwhile;  // bit 5,  1
    uint8_t cpMiaOnHvs;               // bit 6,  1
    uint8_t pcsPwmEnabled;            // bit 7,  1
    uint8_t contactorState;           // bit 8,  3
    uint8_t uiChargeStatus;           // bit 11, 3
    uint8_t ecuLogUploadRequest;      // bit 14, 2
    uint8_t hvState;                  // bit 16, 3
    uint16_t isolationResistance;     // bit 19, 10 (10 kOhm/bit)
    uint8_t chargeRetryCount;         // bit 51, 3  — reserved for future use
    uint8_t chargeRequest;            // bit 29, 1
    uint8_t keepWarmRequest;          // bit 30, 1
    uint8_t bmsState;                 // bit 32, 4
    uint8_t diLimpRequest;            // bit 36, 1
    uint8_t okToShipByAir;            // bit 37, 1
    uint8_t okToShipByLand;           // bit 38, 1
    uint16_t chgPowerAvailable;       // bit 40, 11 (0.125 kW/bit)
    uint8_t smStateRequest;           // bit 56, 4
};

struct VCLEFT_doorStatus_s
{
    uint8_t frontLatchStatus;
    uint8_t rearLatchStatus;
    uint8_t frontLatchSwitch;
    uint8_t rearLatchSwitch;
    uint8_t frontHandlePulled;
    uint8_t rearHandlePulled;
    uint8_t frontRelActuatorSwitch;
    uint8_t rearRelActuatorSwitch;
    uint8_t frontHandlePWM;
    uint8_t rearHandlePWM;
    uint8_t frontIntSwitchPressed;
    uint8_t rearIntSwitchPressed;
    uint8_t mirrorTiltXPosition;
    uint8_t mirrorTiltYPosition;
    uint8_t mirrorState;
    uint8_t mirrorFoldState;
    uint8_t mirrorRecallState;
    uint8_t mirrorHeatState;
    uint8_t mirrorDipped;
    uint8_t frontHandlePulledPersist;
};

struct VCRIGHT_doorStatus_s
{
    uint8_t frontLatchStatus;
    uint8_t rearLatchStatus;
    uint8_t frontLatchSwitch;
    uint8_t rearLatchSwitch;
    uint8_t frontHandlePulled;
    uint8_t rearHandlePulled;
    uint8_t frontRelActuatorSwitch;
    uint8_t rearRelActuatorSwitch;
    uint8_t frontHandlePWM;
    uint8_t rearHandlePWM;
    uint8_t frontHandlePulledPersist;
    uint8_t frontIntSwitchPressed;
    uint8_t rearIntSwitchPressed;
    uint8_t mirrorTiltXPosition;
    uint8_t mirrorTiltYPosition;
    uint8_t mirrorState;
    uint8_t mirrorFoldState;
    uint8_t mirrorRecallState;
    uint8_t trunkLatchStatus;
    uint8_t mirrorDipped;
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

inline ID273UI_vehicleControl_s parseID273UI_vehicleControl(const uint8_t *data, uint8_t dlc)
{
    ID273UI_vehicleControl_s s = {};
    if (dlc < 8) return s;
    s.accessoryPowerRequest    = (uint8_t)extractIntel(data,  0, 1);
    s.frontFogSwitch           = (uint8_t)extractIntel(data,  3, 1);
    s.summonActive             = (uint8_t)extractIntel(data,  4, 1);
    s.frunkRequest             = (uint8_t)extractIntel(data,  5, 1);
    s.wiperMode                = (uint8_t)extractIntel(data,  6, 2);
    s.steeringBacklightEnabled = (uint8_t)extractIntel(data,  8, 1);
    s.steeringButtonMode       = (uint8_t)extractIntel(data,  9, 3);
    s.walkUpUnlock             = (uint8_t)extractIntel(data, 12, 1);
    s.walkAwayLock             = (uint8_t)extractIntel(data, 13, 1);
    s.unlockOnPark             = (uint8_t)extractIntel(data, 14, 1);
    s.globalUnlockOn           = (uint8_t)extractIntel(data, 15, 1);
    s.childDoorLockOn          = (uint8_t)extractIntel(data, 16, 1);
    s.lockRequest              = (uint8_t)extractIntel(data, 17, 3);
    s.alarmEnabled             = (uint8_t)extractIntel(data, 20, 1);
    s.intrusionSensorOn        = (uint8_t)extractIntel(data, 21, 1);
    s.stop12vSupport           = (uint8_t)extractIntel(data, 22, 1);
    s.rearFogSwitch            = (uint8_t)extractIntel(data, 23, 1);
    s.mirrorFoldRequest        = (uint8_t)extractIntel(data, 24, 2);
    s.mirrorHeatRequest        = (uint8_t)extractIntel(data, 26, 1);
    s.remoteStartRequest       = (uint8_t)extractIntel(data, 27, 3);
    s.seeYouHomeLightingOn     = (uint8_t)extractIntel(data, 30, 1);
    s.powerOff                 = (uint8_t)extractIntel(data, 31, 1);
    s.displayBrightnessLevel   = (uint8_t)extractIntel(data, 32, 8);
    s.ambientLightingEnabled   = (uint8_t)extractIntel(data, 40, 1);
    s.autoHighBeamEnabled      = (uint8_t)extractIntel(data, 41, 1);
    s.frontLeftSeatHeatReq     = (uint8_t)extractIntel(data, 42, 2);
    s.frontRightSeatHeatReq    = (uint8_t)extractIntel(data, 44, 2);
    s.rearLeftSeatHeatReq      = (uint8_t)extractIntel(data, 46, 2);
    s.rearCenterSeatHeatReq    = (uint8_t)extractIntel(data, 48, 2);
    s.rearRightSeatHeatReq     = (uint8_t)extractIntel(data, 50, 2);
    s.autoFoldMirrorsOn        = (uint8_t)extractIntel(data, 52, 1);
    s.mirrorDipOnReverse       = (uint8_t)extractIntel(data, 53, 1);
    s.remoteClosureRequest     = (uint8_t)extractIntel(data, 54, 2);
    s.wiperRequest             = (uint8_t)extractIntel(data, 56, 3);
    s.domeLightSwitch          = (uint8_t)extractIntel(data, 59, 2);
    s.honkHorn                 = (uint8_t)extractIntel(data, 61, 1);
    s.driveStateRequest        = (uint8_t)extractIntel(data, 62, 1);
    s.rearWindowLockout        = (uint8_t)extractIntel(data, 63, 1);
    return s;
}

inline ID212BMS_status_s parseID212BMS_status(const uint8_t *data, uint8_t dlc)
{
    ID212BMS_status_s s = {};
    if (dlc < 8) return s;
    s.hvacPowerRequest         = (uint8_t)extractIntel(data,  0, 1);
    s.notEnoughPowerForDrive   = (uint8_t)extractIntel(data,  1, 1);
    s.notEnoughPowerForSupport = (uint8_t)extractIntel(data,  2, 1);
    s.preconditionAllowed      = (uint8_t)extractIntel(data,  3, 1);
    s.updateAllowed            = (uint8_t)extractIntel(data,  4, 1);
    s.activeHeatingWorthwhile  = (uint8_t)extractIntel(data,  5, 1);
    s.cpMiaOnHvs               = (uint8_t)extractIntel(data,  6, 1);
    s.pcsPwmEnabled            = (uint8_t)extractIntel(data,  7, 1);
    s.contactorState           = (uint8_t)extractIntel(data,  8, 3);
    s.uiChargeStatus           = (uint8_t)extractIntel(data, 11, 3);
    s.ecuLogUploadRequest      = (uint8_t)extractIntel(data, 14, 2);
    s.hvState                  = (uint8_t)extractIntel(data, 16, 3);
    s.isolationResistance      = (uint16_t)extractIntel(data, 19, 10);
    s.chargeRequest            = (uint8_t)extractIntel(data, 29, 1);
    s.keepWarmRequest          = (uint8_t)extractIntel(data, 30, 1);
    s.bmsState                 = (uint8_t)extractIntel(data, 32, 4);
    s.diLimpRequest            = (uint8_t)extractIntel(data, 36, 1);
    s.okToShipByAir            = (uint8_t)extractIntel(data, 37, 1);
    s.okToShipByLand           = (uint8_t)extractIntel(data, 38, 1);
    s.chgPowerAvailable        = (uint16_t)extractIntel(data, 40, 11);
    s.chargeRetryCount         = (uint8_t)extractIntel(data, 51, 3);
    s.smStateRequest           = (uint8_t)extractIntel(data, 56, 4);
    return s;
}

inline VCLEFT_doorStatus_s parseVCLEFT_doorStatus(const uint8_t *data, uint8_t dlc)
{
    VCLEFT_doorStatus_s s = {};
    if (dlc < 8) return s;
    s.frontLatchStatus       = (uint8_t)extractIntel(data, 0, 4);
    s.rearLatchStatus        = (uint8_t)extractIntel(data, 4, 4);
    s.frontLatchSwitch       = (uint8_t)extractIntel(data, 8, 1);
    s.rearLatchSwitch        = (uint8_t)extractIntel(data, 9, 1);
    s.frontHandlePulled      = (uint8_t)extractIntel(data, 10, 1);
    s.rearHandlePulled       = (uint8_t)extractIntel(data, 11, 1);
    s.frontRelActuatorSwitch = (uint8_t)extractIntel(data, 12, 1);
    s.rearRelActuatorSwitch  = (uint8_t)extractIntel(data, 13, 1);
    s.frontHandlePWM         = (uint8_t)extractIntel(data, 16, 7);
    s.rearHandlePWM          = (uint8_t)extractIntel(data, 24, 7);
    s.frontIntSwitchPressed  = (uint8_t)extractIntel(data, 31, 1);
    s.rearIntSwitchPressed   = (uint8_t)extractIntel(data, 32, 1);
    s.mirrorTiltXPosition    = (uint8_t)extractIntel(data, 33, 8);
    s.mirrorTiltYPosition    = (uint8_t)extractIntel(data, 41, 8);
    s.mirrorState            = (uint8_t)extractIntel(data, 49, 3);
    s.mirrorFoldState        = (uint8_t)extractIntel(data, 52, 3);
    s.mirrorRecallState      = (uint8_t)extractIntel(data, 55, 3);
    s.mirrorHeatState        = (uint8_t)extractIntel(data, 58, 3);
    s.mirrorDipped           = (uint8_t)extractIntel(data, 61, 1);
    s.frontHandlePulledPersist = (uint8_t)extractIntel(data, 62, 1);
    return s;
}

inline VCRIGHT_doorStatus_s parseVCRIGHT_doorStatus(const uint8_t *data, uint8_t dlc)
{
    VCRIGHT_doorStatus_s s = {};
    if (dlc < 8) return s;
    s.frontLatchStatus       = (uint8_t)extractIntel(data, 0, 4);
    s.rearLatchStatus        = (uint8_t)extractIntel(data, 4, 4);
    s.frontLatchSwitch       = (uint8_t)extractIntel(data, 8, 1);
    s.rearLatchSwitch        = (uint8_t)extractIntel(data, 9, 1);
    s.frontHandlePulled      = (uint8_t)extractIntel(data, 10, 1);
    s.rearHandlePulled       = (uint8_t)extractIntel(data, 11, 1);
    s.frontRelActuatorSwitch = (uint8_t)extractIntel(data, 12, 1);
    s.rearRelActuatorSwitch  = (uint8_t)extractIntel(data, 13, 1);
    s.frontHandlePWM         = (uint8_t)extractIntel(data, 14, 7);
    s.rearHandlePWM          = (uint8_t)extractIntel(data, 21, 7);
    s.frontHandlePulledPersist = (uint8_t)extractIntel(data, 30, 1);
    s.frontIntSwitchPressed  = (uint8_t)extractIntel(data, 31, 1);
    s.rearIntSwitchPressed   = (uint8_t)extractIntel(data, 32, 1);
    s.mirrorTiltXPosition    = (uint8_t)extractIntel(data, 33, 8);
    s.mirrorTiltYPosition    = (uint8_t)extractIntel(data, 41, 8);
    s.mirrorState            = (uint8_t)extractIntel(data, 49, 3);
    s.mirrorFoldState        = (uint8_t)extractIntel(data, 52, 3);
    s.mirrorRecallState      = (uint8_t)extractIntel(data, 60, 3);
    s.trunkLatchStatus       = (uint8_t)extractIntel(data, 56, 4);
    s.mirrorDipped           = (uint8_t)extractIntel(data, 63, 1);
    return s;
}
