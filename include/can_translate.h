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

inline const char *blindSpotName(uint8_t s)
{
    switch (s)
    {
    case 0: return "无警告"; case 1: return "1级"; case 2: return "2级";
    case 3: return "无效"; default: return "?";
    }
}

inline const char *laneDepName(uint8_t v)
{
    switch (v)
    {
    case 0: return "无"; case 1: return "左"; case 2: return "右";
    case 3: return "左严重"; case 4: return "右严重"; default: return "?";
    }
}

inline const char *sideCollName(uint8_t v)
{
    switch (v)
    {
    case 0: return "无"; case 1: return "左"; case 2: return "右"; case 3: return "左右";
    default: return "?";
    }
}

inline const char *fcwName(uint8_t v)
{
    switch (v)
    {
    case 0: return "无"; case 1: return "预警"; case 3: return "无效";
    default: return "?";
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

inline const char *hazardReqName(uint8_t v)
{
    switch (v)
    {
    case 0: return "无"; case 1: return "按钮"; case 2: return "闭锁";
    case 3: return "解锁"; case 4: return "误锁"; case 5: return "碰撞";
    case 6: return "报警"; case 7: return "DAS"; case 8: return "诊断";
    default: return "?";
    }
}

// ── formatCanTranslation: only the 7 selected CAN IDs ──

inline bool formatCanTranslation(uint32_t id, const uint8_t *data, uint8_t dlc, char *out, size_t outLen)
{
    if (!out || outLen == 0) return false;
    out[0] = '\0';

    // ── 0x118 (280) DI_systemStatus ──
    if (id == 280 || id == 0x118)
    {
        if (dlc < 5) return false;
        uint8_t gear = (data[2] >> 5) & 0x07;
        float accel = data[4] * 0.4f;
        uint8_t regen = (data[3] >> 2) & 0x01;
        char tmp[80];
        snprintf(tmp, sizeof(tmp), "档位:%s 油门:%.0f%% 动能回收:%s",
                 gearName(gear), accel, regen ? "回收中" : "关闭");
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ── 0x145 (325) ESP_status ──
    // data[4] bit1/2: brake switch (02→04踩下), data[5]: 踏板行程(1E基线, 重踩可达4F)
    if (id == 325 || id == 0x145)
    {
        if (dlc < 6) return false;
        uint8_t pedal = data[5];
        bool brakeOn = (data[4] & 0x06) != 0 || pedal > 0x1E;
        char tmp[64];
        if (brakeOn)
            snprintf(tmp, sizeof(tmp), "制动:踩下 踏板值:%d(0x%02X)", pedal, pedal);
        else
            snprintf(tmp, sizeof(tmp), "制动:未踩 待命值:%d(0x%02X)", pedal, pedal);
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ── 0x155 (341) ESP_B ──
    if (id == 341 || id == 0x155)
    {
        if (dlc < 6) return false;
        uint16_t rawSpeed = (uint16_t)extractIntel(data, 42, 10);
        float kph = rawSpeed == 1023 ? -1 : rawSpeed * 0.5f;
        bool still = extractIntel(data, 41, 1) != 0;
        char tmp[48];
        if (kph >= 0)
            snprintf(tmp, sizeof(tmp), "车速:%.0fkm/h %s", kph, still ? "静止" : "");
        else
            snprintf(tmp, sizeof(tmp), "车速:无效");
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ── 0x129 (297) SCCM_steeringAngleSensor ──
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

    // ── 0x311 (785) UI_warning ──
    if (id == 785 || id == 0x311)
    {
        if (dlc < 7) return false;
        bool buckle = extractMotorola(data, 13, 1) != 0;
        uint8_t leftBlink = (uint8_t)extractMotorola(data, 25, 2);
        uint8_t rightBlink = (uint8_t)extractIntel(data, 26, 2);
        bool door = extractMotorola(data, 28, 1) != 0;
        bool hiBeam = extractMotorola(data, 50, 1) != 0;
        char tmp[80];
        snprintf(tmp, sizeof(tmp), "门:%s 安全带:%s 左转:%s 右转:%s 远光:%s",
                 door ? "开" : "关", buckle ? "扣" : "未扣",
                 leftBlink ? "闪烁" : "关", rightBlink ? "闪烁" : "关",
                 hiBeam ? "开" : "关");
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ── 0x39B (923) DAS_status ──
    if (id == 923 || id == 0x39B)
    {
        if (dlc < 5) return false;
        uint8_t bsL = (uint8_t)extractIntel(data, 4, 2);
        uint8_t bsR = (uint8_t)extractIntel(data, 6, 2);
        uint8_t fcw = (uint8_t)extractIntel(data, 22, 2);
        uint8_t scw = (uint8_t)extractIntel(data, 32, 2);
        uint8_t ldw = (uint8_t)extractIntel(data, 37, 3);
        uint8_t ssw = (uint8_t)extractIntel(data, 13, 1);
        char tmp[96];
        snprintf(tmp, sizeof(tmp), "盲区:左%s/右%s 前碰:%s 侧碰:%s 车道偏离:%s 超速警告:%s",
                 blindSpotName(bsL), blindSpotName(bsR),
                 fcwName(fcw), sideCollName(scw), laneDepName(ldw),
                 ssw ? "抑制" : "正常");
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ── 0x39D (925) IBST_status ──
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
