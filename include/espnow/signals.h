#pragma once

#include "protocol.h"
#include "../can_helpers.h"

// Current signal state for broadcast — defined in broadcast.h
extern EspNowCanDataPkt espnowCurData;
extern unsigned long lastCanSignalMs;

// Update the ESP-NOW signal state from any CAN frame
// Uses unified signal parsers from can_helpers.h
static void espnowUpdateFromFrame(const CanFrame &frame)
{
    lastCanSignalMs = millis();
    if (frame.dlc < 4) return;

    if (frame.id == 280 || frame.id == 0x118)
    {
        auto s = parseDI_systemStatus(frame.data, frame.dlc);
        if (frame.dlc < 7) return;
        espnowCurData.gear                = s.gear;
        espnowCurData.accelPedalPos       = s.accelPedalPos;
        espnowCurData.regenLight          = s.regenLight;
        espnowCurData.brakePedalState     = s.brakePedalState;
        espnowCurData.systemState         = s.systemState;
        espnowCurData.tractionControlMode = s.tractionControlMode;
        espnowCurData.epbRequest          = s.epbRequest;
        espnowCurData.trackModeState      = s.trackModeState;
        espnowCurData.immobilizerState    = s.immobilizerState;
        espnowCurData.proximity           = s.proximity;
        espnowCurData.keepAliveRequest    = s.keepAliveRequest;
        return;
    }

    if (frame.id == 599 || frame.id == 0x257)
    {
        auto s = parseDI_speed(frame.data, frame.dlc);
        if (frame.dlc < 5) return;
        espnowCurData.vehicleSpeed  = s.vehicleSpeed;
        espnowCurData.uiSpeed       = s.uiSpeed;
        espnowCurData.uiSpeedUnits  = s.uiSpeedUnits;
        return;
    }

    if (frame.id == 905 || frame.id == 0x389)
    {
        auto s = parseDAS_status2(frame.data, frame.dlc);
        if (frame.dlc < 7) return;
        espnowCurData.longCollisionWarning   = s.longCollisionWarning;
        espnowCurData.lssState               = s.lssState;
        espnowCurData.driverInteractionLevel = s.driverInteractionLevel;
        uint16_t accLimit = s.accSpeedLimit * 0.4f + 0.5f;
        espnowCurData.accSpeedLimit          = accLimit > 255 ? 255 : (uint8_t)accLimit;
        return;
    }

    if (frame.id == 263 || frame.id == 0x107)
    {
        auto s = parseIBST_status(frame.data, frame.dlc);
        if (frame.dlc < 3) return;
        espnowCurData.driverBrakeApply = s.driverBrakeApply;
        espnowCurData.brakeRodTravel   = s.brakeRodTravel;
        espnowCurData.internalState    = s.internalState;
        espnowCurData.iBoosterStatus   = s.iBoosterStatus;
        return;
    }

    if (frame.id == 1013 || frame.id == 0x3F5)
    {
        auto s = parseVCFRONT_lighting(frame.data, frame.dlc);
        if (frame.dlc < 8) return;
        espnowCurData.lowBeamLeftStatus      = s.lowBeamLeftStatus;
        espnowCurData.lowBeamRightStatus     = s.lowBeamRightStatus;
        espnowCurData.highBeamLeftStatus     = s.highBeamLeftStatus;
        espnowCurData.highBeamRightStatus    = s.highBeamRightStatus;
        espnowCurData.drlLeftStatus          = s.drlLeftStatus;
        espnowCurData.drlRightStatus         = s.drlRightStatus;
        espnowCurData.fogLeftStatus          = s.fogLeftStatus;
        espnowCurData.fogRightStatus         = s.fogRightStatus;
        espnowCurData.sideRepeaterLeftStatus = s.sideRepeaterLeftStatus;
        espnowCurData.sideRepeaterRightStatus= s.sideRepeaterRightStatus;
        espnowCurData.turnSignalLeftStatus   = s.turnSignalLeftStatus;
        espnowCurData.turnSignalRightStatus  = s.turnSignalRightStatus;
        espnowCurData.parkLeftStatus         = s.parkLeftStatus;
        espnowCurData.parkRightStatus        = s.parkRightStatus;
        espnowCurData.hazardLightRequest     = s.hazardLightRequest;
        espnowCurData.indicatorLeftRequest   = s.indicatorLeftRequest;
        espnowCurData.indicatorRightRequest  = s.indicatorRightRequest;
        return;
    }

    if (frame.id == 994 || frame.id == 0x3E2)
    {
        auto s = parseE32_brakeLight(frame.data, frame.dlc);
        if (frame.dlc < 1) return;
        espnowCurData.brakeLight = s.brakeLight;
        return;
    }

    if (frame.id == 258 || frame.id == 0x102)
    {
        auto s = parseVCLEFT_doorStatus(frame.data, frame.dlc);
        if (frame.dlc < 8) return;
        espnowCurData.doorOpenBits &= ~0x03;
        if (s.frontLatchStatus == 1) espnowCurData.doorOpenBits |= 0x01;
        if (s.rearLatchStatus == 1)  espnowCurData.doorOpenBits |= 0x02;
        return;
    }

    if (frame.id == 259 || frame.id == 0x103)
    {
        auto s = parseVCRIGHT_doorStatus(frame.data, frame.dlc);
        if (frame.dlc < 8) return;
        espnowCurData.doorOpenBits &= ~0x0C;
        if (s.frontLatchStatus == 1) espnowCurData.doorOpenBits |= 0x04;
        if (s.rearLatchStatus == 1)  espnowCurData.doorOpenBits |= 0x08;
        return;
    }

    // 0x212 — ID212BMS_status (BMS 状态, 休眠备用条件)
    if (frame.id == 530 || frame.id == 0x212)
    {
        auto s = parseID212BMS_status(frame.data, frame.dlc);
        if (frame.dlc < 8) return;
        espnowCurData.bmsState        = s.bmsState;
        espnowCurData.bmsHvState      = s.hvState;
        espnowCurData.bmsChargeStatus = s.uiChargeStatus;
        // BMS 备选: HV_DOWN/HV_GOING_DOWN + BMS_STANDBY → 锁车
        // 但 UI_powerOff 优先级更高（无论充电状态都能判定）
        espnowCurData.vehicleLocked = espnowCurData.uiPowerOff ||
            (((s.hvState == 0 || s.hvState == 2) && s.bmsState == 0) ? 1 : 0);
        return;
    }

    // 0x273 — ID273UI_vehicleControl (主休眠信号: UI_powerOff)
    if (frame.id == 627 || frame.id == 0x273)
    {
        auto s = parseID273UI_vehicleControl(frame.data, frame.dlc);
        if (frame.dlc < 8) return;
        espnowCurData.uiPowerOff = s.powerOff;
        // UI_powerOff=1 → 车辆下电，立即标记休眠
        if (s.powerOff) {
            espnowCurData.vehicleLocked = 1;
        } else {
            // 未下电但 BMS 状态已就绪 → 用 BMS 条件判断
            espnowCurData.vehicleLocked =
                (espnowCurData.bmsHvState == 0 || espnowCurData.bmsHvState == 2)
                && espnowCurData.bmsState == 0;
        }
        return;
    }
}
