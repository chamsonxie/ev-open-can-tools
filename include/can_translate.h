#pragma once
#include "can_frame_types.h"
#include "can_helpers.h"
#include <cstdio>
#include <cstring>

// ── Named helper functions ──

inline const char *lightStatusName(uint8_t v)
{
    switch (v)
    {
    case 0: return "关"; case 1: return "开"; case 2: return "故障"; case 3: return "无效";
    default: return "?";
    }
}

inline const char *gearName(uint8_t g)
{
    switch (g)
    {
    case 0: return "无效"; case 1: return "P"; case 2: return "R";
    case 3: return "N"; case 4: return "D"; case 7: return "SNA";
    default: return "未知";
    }
}

inline const char *brakeApplyName(uint8_t v)
{
    switch (v)
    {
    case 0: return "未初始化"; case 1: return "未制动";
    case 2: return "制动中"; case 3: return "故障";
    default: return "未知";
    }
}

inline const char *reqName(uint8_t v)
{
    switch (v)
    {
    case 0: return "关"; case 1: return "低有效"; case 2: return "高有效";
    default: return "?";
    }
}

inline const char *lssStateName(uint8_t v)
{
    switch (v)
    {
    case 0: return "故障"; case 1: return "LDW"; case 2: return "LKA";
    case 3: return "ELK"; case 4: return "监控"; case 5: return "盲区";
    case 6: return "中止"; case 7: return "关闭";
    default: return "?";
    }
}

inline const char *longCollisionName(uint8_t v)
{
    switch (v)
    {
    case 0: return "无"; case 1: return "前方车辆"; case 2: return "行人";
    case 7: return "切入车辆"; default: return "?";
    }
}

// ── formatCanTranslation: only the 6 selected CAN IDs ──

inline bool formatCanTranslation(uint32_t id, const uint8_t *data, uint8_t dlc, char *out, size_t outLen)
{
    if (!out || outLen == 0) return false;
    out[0] = '\0';

    // ── 0x118 (280) DI_systemStatus ──
    // DBC: gear=21|3@1+, accelPedalPos=32|8@1+, regenLight=26|1@1+, brakePedalState=19|2@1+
    if (id == 280 || id == 0x118)
    {
        if (dlc < 5) return false;
        uint8_t gear = (data[2] >> 5) & 0x07;
        float accel = data[4] * 0.4f;
        uint8_t regen = (data[3] >> 2) & 0x01;
        uint8_t brake = (data[2] >> 3) & 0x03;
        char tmp[96];
        snprintf(tmp, sizeof(tmp), "档位:%s 油门:%.0f%% 动能回收:%s 制动:%s",
                 gearName(gear), accel, regen ? "回收中" : "关闭",
                 brake == 1 ? "踩下" : (brake == 0 ? "松开" : "无效"));
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ── 0x129 (297) SCCM_steeringAngleSensor ──
    // DBC: steeringAngle=16|14@1+, steeringAngleSpeed=32|14@1+
    if (id == 297 || id == 0x129)
    {
        if (dlc < 6) return false;
        float angle = extractIntel(data, 16, 14) * 0.1f - 819.2f;
        float speed = extractIntel(data, 32, 14) * 0.5f - 4096.0f;
        char tmp[64];
        snprintf(tmp, sizeof(tmp), "方向盘:%.1f° 角速度:%.0f°/s", angle, speed);
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ── 0x257 (599) DI_speed ──
    // DBC: vehicleSpeed=12|12@1+ (0.08,-40) [-40|285] "kph"
    //      uiSpeed=24|8@1+ (1,0) [0|254]
    //      uiSpeedUnits=32|1@1+ (1,0) [0|1]
    if (id == 599 || id == 0x257)
    {
        if (dlc < 5) return false;
        uint64_t rawVeh = extractIntel(data, 12, 12);
        float kph = rawVeh * 0.08f - 40.0f;
        uint8_t uiSpeed = data[3];
        uint8_t units = data[4] & 0x01;
        char tmp[64];
        if (rawVeh == 4095)
            snprintf(tmp, sizeof(tmp), "车速:无效 显示速度:%d %s", uiSpeed, units ? "km/h" : "mph");
        else
            snprintf(tmp, sizeof(tmp), "车速:%.1fkm/h 显示:%d %s", kph, uiSpeed, units ? "km/h" : "mph");
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ── 0x389 (905) DAS_status2 ──
    // DBC: longCollisionWarning=48|4@1+, lssState=31|3@1+, driverInteractionLevel=38|2@1+
    //      accSpeedLimit=0|10@1+ (0.4,0) [0|204.6] "mph"
    if (id == 905 || id == 0x389)
    {
        if (dlc < 7) return false;
        uint8_t lcw = (uint8_t)extractIntel(data, 48, 4);
        uint8_t lss = (uint8_t)extractIntel(data, 31, 3);
        uint8_t dil = (uint8_t)extractIntel(data, 38, 2);
        uint64_t rawSpd = extractIntel(data, 0, 10);
        float accLimit = rawSpd * 0.4f;
        char tmp[96];
        snprintf(tmp, sizeof(tmp), "碰撞预警:%s 车道辅助:%s 交互:%s ACC限速:%.0fmph",
                 longCollisionName(lcw), lssStateName(lss),
                 dil == 0 ? "交互中" : (dil == 1 ? "未交互" : "持续未交互"),
                 accLimit);
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ── 0x39D (925) IBST_status ──
    // DBC: driverBrakeApply=16|2@1+, brakeRodTravel=21|12@1+ (0.015625,-5)
    if (id == 925 || id == 0x39D)
    {
        if (dlc < 3) return false;
        uint8_t brake = (uint8_t)extractIntel(data, 16, 2);
        uint64_t rawRod = extractIntel(data, 21, 12);
        float rod = rawRod * 0.015625f - 5.0f;
        char tmp[64];
        snprintf(tmp, sizeof(tmp), "制动:%s 踏板行程:%.1fmm",
                 brakeApplyName(brake), rod);
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ── 0x3F5 (1013) VCFRONT_lighting ──
    // DBC: indicatorLeftRequest=0|2@1+, indicatorRightRequest=2|2@1+
    //      lowBeamLeftStatus=28|2@1+, lowBeamRightStatus=30|2@1+
    //      highBeamLeftStatus=32|2@1+, highBeamRightStatus=34|2@1+
    //      drlLeftStatus=36|2@1+, drlRightStatus=38|2@1+
    //      parkLeftStatus=54|2@1+, parkRightStatus=56|2@1+
    if (id == 1013 || id == 0x3F5)
    {
        if (dlc < 8) return false;
        uint8_t indL = data[0] & 0x03;
        uint8_t indR = (data[0] >> 2) & 0x03;
        uint8_t lowL = (data[3] >> 4) & 0x03;
        uint8_t lowR = (data[3] >> 6) & 0x03;
        uint8_t highL = data[4] & 0x03;
        uint8_t highR = (data[4] >> 2) & 0x03;
        uint8_t drlL = (data[4] >> 4) & 0x03;
        uint8_t drlR = (data[4] >> 6) & 0x03;
        uint8_t parkL = (data[6] >> 6) & 0x03;
        uint8_t parkR = data[7] & 0x03;
        char tmp[128];
        snprintf(tmp, sizeof(tmp), "转向L:%s/R:%s 近光:%s/%s 远光:%s/%s DRL:%s/%s 驻车:%s/%s",
                 reqName(indL), reqName(indR),
                 lightStatusName(lowL), lightStatusName(lowR),
                 lightStatusName(highL), lightStatusName(highR),
                 lightStatusName(drlL), lightStatusName(drlR),
                 lightStatusName(parkL), lightStatusName(parkR));
        strlcpy(out, tmp, outLen);
        return true;
    }

    return false;
}
