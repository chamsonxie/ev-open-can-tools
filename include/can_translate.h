#pragma once
#include "can_frame_types.h"
#include "can_helpers.h"
#include <cstdio>
#include <cstring>

// Intel (little-endian) signal extraction: bits numbered linearly from LSB of byte 0
inline uint64_t extractIntel(const uint8_t* data, int start, int len)
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

// Motorola (big-endian) signal extraction: bits numbered MSB-first within each byte
inline uint64_t extractMotorola(const uint8_t* data, int start, int len)
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

inline const char* latchStateName(uint8_t v)
{
    switch (v)
    {
    case 0: return "SNA"; case 1: return "开"; case 2: return "关";
    case 3: return "关中"; case 4: return "开中"; case 5: return "微开";
    case 6: return "超时"; case 7: return "默认"; case 8: return "故障";
    default: return "?";
    }
}

inline const char* mirrorFoldName(uint8_t v)
{
    switch (v)
    {
    case 0: return "未知"; case 1: return "折叠";
    case 2: return "展开"; case 3: return "折叠中"; case 4: return "展开中";
    default: return "?";
    }
}

inline const char* turnStalkName(uint8_t v)
{
    switch (v)
    {
    case 0: return "无"; case 1: return "上1"; case 2: return "上2";
    case 3: return "下1"; case 4: return "下2"; case 5: return "无效";
    default: return "?";
    }
}

inline const char* highBeamName(uint8_t v)
{
    switch (v)
    {
    case 0: return "无"; case 1: return "拉"; case 2: return "推"; case 3: return "无效";
    default: return "?";
    }
}

inline const char* wiperBtnName(uint8_t v)
{
    switch (v)
    {
    case 0: return "无"; case 1: return "1段"; case 2: return "2段"; case 3: return "无效";
    default: return "?";
    }
}

inline const char* powerStateName(uint8_t v)
{
    switch (v)
    {
    case 0: return "关"; case 1: return "调节"; case 2: return "附件"; case 3: return "行驶";
    default: return "?";
    }
}

inline const char* defrostName(uint8_t v)
{
    switch (v)
    {
    case 0: return "无效"; case 1: return "开"; case 2: return "关";
    case 3: return "关不可用"; case 4: return "故障";
    default: return "?";
    }
}

inline const char* rightStalkName(uint8_t v)
{
    switch (v)
    {
    case 0: return "无"; case 1: return "上1"; case 2: return "上2";
    case 3: return "下1"; case 4: return "下2"; case 5: return "初始化";
    case 6: return "无效";
    default: return "?";
    }
}

inline const char* parkBtnName(uint8_t v)
{
    switch (v)
    {
    case 0: return "无"; case 1: return "按"; case 2: return "初始化"; case 3: return "无效";
    default: return "?";
    }
}

inline const char* steerValidName(uint8_t v)
{
    switch (v)
    {
    case 0: return "无效"; case 1: return "有效";
    case 2: return "初始化"; case 3: return "无效";
    default: return "?";
    }
}

inline const char* steerStatusName(uint8_t v)
{
    switch (v)
    {
    case 0: return "OK"; case 1: return "初始化";
    case 2: return "错误"; case 3: return "初始化错误";
    default: return "?";
    }
}

inline const char* switchName(uint8_t v)
{
    switch (v)
    {
    case 0: return "无效"; case 1: return "关"; case 2: return "开"; case 3: return "故障";
    default: return "?";
    }
}

inline const char* scrollName(uint8_t v)
{
    switch (v)
    {
    case 0: return "无"; case 1: return "上"; case 2: return "下"; case 3: return "无效";
    default: return "?";
    }
}

inline const char* dasAccStateName(uint8_t s)
{
    switch (s)
    {
    case 0: return "取消"; case 1: return "摄像头盲区取消"; case 2: return "雷达盲区取消";
    case 3: return "保持"; case 4: return "ACC开启"; case 5: return "APC倒车";
    case 6: return "APC前进"; case 7: return "APC完成"; case 8: return "APC中止";
    case 9: return "APC暂停"; case 10: return "APC驻车完成"; case 11: return "APC开始自驻车";
    case 12: return "路径不清取消"; case 13: return "静默取消"; case 14: return "标定取消";
    case 15: return "故障"; default: return "未知";
    }
}

inline const char* dasAutopilotStateName(uint8_t s)
{
    switch (s)
    {
    case 0: return "禁用"; case 1: return "不可用"; case 2: return "可用";
    case 3: return "激活(标准)"; case 4: return "激活(受限)"; case 5: return "激活(导航)";
    case 6: return "激活(FSD)"; case 7: return "保留"; case 8: return "中止中";
    case 9: return "已中止"; case 14: return "故障"; case 15: return "无效";
    default: return "未知";
    }
}

inline const char* dasBlindSpotName(uint8_t s)
{
    switch (s)
    {
    case 0: return "无警告"; case 1: return "一级警告"; case 2: return "二级警告";
    case 3: return "无效"; default: return "未知";
    }
}

inline const char* ibstBrakeName(uint8_t s)
{
    switch (s)
    {
    case 0: return "未初始化"; case 1: return "未制动";
    case 2: return "制动中"; case 3: return "故障";
    default: return "未知";
    }
}

inline const char* ibstStatusName(uint8_t s)
{
    switch (s)
    {
    case 0: return "关闭"; case 1: return "初始化"; case 2: return "故障";
    case 3: return "诊断"; case 4: return "自检通过"; case 5: return "就绪";
    case 6: return "作动中"; default: return "未知";
    }
}

inline const char* epasHandsOnName(uint8_t s)
{
    switch (s)
    {
    case 0: return "LEVEL_0(手在)"; case 1: return "LEVEL_1(轻握)";
    case 2: return "LEVEL_2(脱离)"; case 3: return "LEVEL_3(严重)";
    default: return "未知";
    }
}

inline const char* eacStatusName(uint8_t s)
{
    switch (s)
    {
    case 0: return "禁止"; case 1: return "可用"; case 2: return "激活";
    case 3: return "故障"; case 4: return "车道保持"; case 5: return "紧急车道保持";
    case 7: return "无效"; default: return "未知";
    }
}

inline const char* lightStatusName(uint8_t v)
{
    switch (v)
    {
    case 0: return "关"; case 1: return "开"; case 2: return "故障"; case 3: return "无效";
    default: return "?";
    }
}

inline const char* drvBrkName(uint8_t v)
{
    switch (v)
    {
    case 0: return "未初始化"; case 1: return "未踩"; case 2: return "踩下"; case 3: return "故障";
    default: return "?";
    }
}

// ============================================================
// formatCanTranslation — 对照 tesla_model3_vehicle.dbc 完整实现
// ============================================================
inline bool formatCanTranslation(uint32_t id, const uint8_t* data, uint8_t dlc, char* out, size_t outLen)
{
    if (!out || outLen == 0) return false;
    out[0] = '\0';

    // ---- 0x102 (258) VCLEFT_doorStatus [VEH] ----
    if (id == 0x102)
    {
        if (dlc < 8) return false;
        uint8_t fls = data[0] & 0x0F;
        uint8_t rls = (data[0] >> 4) & 0x0F;
        uint8_t fhp = (data[1] >> 2) & 0x01;
        uint8_t rhp = (data[1] >> 3) & 0x01;
        uint8_t mfs = (data[6] >> 4) & 0x07;
        uint8_t mst = (data[6] >> 1) & 0x07;
        char tmp[96];
        snprintf(tmp, sizeof(tmp), "前门:%s 后门:%s 前拉:%s 后拉:%s 后视镜:%s",
                 latchStateName(fls), latchStateName(rls),
                 fhp?"是":"否", rhp?"是":"否",
                 mirrorFoldName(mfs));
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x103 (259) VCRIGHT_doorStatus [VEH] ----
    if (id == 0x103)
    {
        if (dlc < 8) return false;
        uint8_t fls = data[0] & 0x0F;
        uint8_t rls = (data[0] >> 4) & 0x0F;
        uint8_t fhp = (data[1] >> 2) & 0x01;
        uint8_t rhp = (data[1] >> 3) & 0x01;
        uint8_t tls = (data[7] >> 0) & 0x0F;
        uint8_t mfs = (data[6] >> 4) & 0x07;
        uint8_t mst = (data[6] >> 1) & 0x07;
        char tmp[96];
        snprintf(tmp, sizeof(tmp), "前门:%s 后门:%s 前拉:%s 后拉:%s 尾箱:%s",
                 latchStateName(fls), latchStateName(rls),
                 fhp?"是":"否", rhp?"是":"否",
                 latchStateName(tls));
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x108 (264) DI_torque [PARTY] ----
    if (id == 0x108)
    {
        if (dlc < 6) return false;
        int64_t torqueActual = extractSigned(extractIntel(data, 27, 13), 13) * 2;
        int64_t torqueCmd = extractSigned(extractIntel(data, 12, 13), 13) * 2;
        int64_t axleSpeed = extractSigned(extractIntel(data, 40, 16), 16);
        float rpm = axleSpeed * 0.1f;
        char tmp[96];
        snprintf(tmp, sizeof(tmp), "扭矩实际：%ld Nm，扭矩指令：%ld Nm，转速：%.0f RPM",
                 (long)torqueActual, (long)torqueCmd, rpm);
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x116 DI_torque2 (车速/刹车/档位) ----
    if (id == 0x116)
    {
        if (dlc < 6) return false;
        uint16_t raw = ((uint16_t)data[2] << 4) | (data[3] >> 4);
        float mph = raw * 0.05f - 25.0f;
        float kph = mph * 1.60934f;
        uint8_t gr = (data[1] >> 4) & 0x07;
        const char* g = (gr==1?"P":gr==2?"R":gr==3?"N":gr==4?"D":gr==7?"SNA":gr==0?"无效":"未知");
        char tmp[96];
        if (mph > -10 && mph < 200)
            snprintf(tmp, sizeof(tmp), "车速：%.1f km/h (%.1f mph)，档位：%s", kph, mph, g);
        else
            snprintf(tmp, sizeof(tmp), "车速：(无效)，档位：%s", g);
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x118 (280) DI_systemStatus [VEH] ----
    if (id == 280 || id == 0x118)
    {
        if (dlc < 3) return false;
        uint8_t b2 = data[2], b4 = data[4], b6 = data[6];
        uint8_t gr = (b2 >> 5) & 0x07;
        const char* g = (gr==1?"P":gr==2?"R":gr==3?"N":gr==4?"D":gr==7?"SNA":gr==0?"无效":"未知");
        bool parked = (gr==0 || gr==1 || gr==7);
        float acc = b4 * 0.4f;
        uint8_t br = (b2 >> 3) & 0x03;
        const char* bs = (br==0?"OFF":br==1?"ON":"INVALID");
        bool aca = (b6 & 0x04) != 0;
        char tmp[128];
        snprintf(tmp, sizeof(tmp), "档位：%s（%s），油门：%.1f%%，刹车：%s，ACA：%s",
                 g, parked?"驻车":"非驻车", acc, bs, aca?"是":"否");
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x129 (297) SCCM_steeringAngleSensor [VEH] ----
    if (id == 0x129)
    {
        if (dlc < 7) return false;
        float angle = extractIntel(data, 16, 14) * 0.1f - 819.2f;
        float speed = extractIntel(data, 32, 14) * 0.5f - 4096.0f;
        uint8_t valid = (uint8_t)extractIntel(data, 30, 2);
        uint8_t status = (uint8_t)extractIntel(data, 14, 2);
        char tmp[80];
        snprintf(tmp, sizeof(tmp), "方向盘:%.1f° 角速度:%.0f°/s %s",
                 angle, speed, steerValidName(valid));
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x145 (325) ESP_status [PARTY] ----
    if (id == 0x145)
    {
        if (dlc < 4) return false;
        uint8_t driverBrake = (uint8_t)extractIntel(data, 29, 2);
        uint8_t brakeApply = (uint8_t)extractIntel(data, 31, 1);
        char tmp[48];
        snprintf(tmp, sizeof(tmp), "制动:%s ESP主动制动:%s",
                 drvBrkName(driverBrake), brakeApply?"是":"否");
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x175 (373) ESP_wheelSpeeds [CH] ----
    if (id == 0x175)
    {
        if (dlc < 7) return false;
        float frL = extractIntel(data, 0, 13) * 0.04f;
        float frR = extractIntel(data, 13, 13) * 0.04f;
        float reL = extractIntel(data, 26, 13) * 0.04f;
        float reR = extractIntel(data, 39, 13) * 0.04f;
        char tmp[96];
        snprintf(tmp, sizeof(tmp), "轮速：左前%.0f 右前%.0f 左后%.0f 右后%.0f km/h", frL, frR, reL, reR);
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x229 (553) SCCM_rightStalk [VEH] ----
    if (id == 0x229)
    {
        if (dlc < 3) return false;
        uint8_t st = (data[1] >> 4) & 0x07;
        uint8_t pb = (data[2] >> 0) & 0x03;
        char tmp[48];
        snprintf(tmp, sizeof(tmp), "右拨杆:%s 驻车按钮:%s",
                 rightStalkName(st), parkBtnName(pb));
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x238 (568) STW_ACTN_RQ [STW] ----
    if (id == 0x238)
    {
        if (dlc < 7) return false;
        uint8_t spdCtrl = data[0] & 0x3F;
        uint8_t turnLvr = (data[2] >> 0) & 0x03;
        uint8_t hiBmLvr = (data[2] >> 2) & 0x03;
        uint8_t wprWash = (data[2] >> 4) & 0x03;
        uint8_t hrnSw = (data[3] >> 6) & 0x03;
        uint8_t wprPos = (data[6] >> 0) & 0x07;
        const char* spd = spdCtrl==1?"前":spdCtrl==2?"后":spdCtrl==4?"上2":spdCtrl==8?"下2":spdCtrl==16?"上1":spdCtrl==32?"下1":"无";
        char tmp[96];
        snprintf(tmp, sizeof(tmp), "巡航:%s 转向:%s 远光:%s 雨刷:%u 喇叭:%s",
                 spd, turnLvr==1?"左":turnLvr==2?"右":"无",
                 hiBmLvr==1?"闪":hiBmLvr==2?"开":"无",
                 wprPos, hrnSw==1?"按":"无");
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x249 (585) SCCM_leftStalk [VEH] ----
    if (id == 0x249)
    {
        if (dlc < 3) return false;
        uint8_t turn = (data[2] >> 0) & 0x07;
        uint8_t hb = (data[1] >> 4) & 0x03;
        uint8_t ww = (data[1] >> 6) & 0x03;
        char tmp[48];
        snprintf(tmp, sizeof(tmp), "转向灯:%s 远光:%s 雨刷:%s",
                 turnStalkName(turn), highBeamName(hb), wiperBtnName(ww));
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x257 (599) DI_speed [PARTY] ----
    if (id == 0x257)
    {
        if (dlc < 5) return false;
        float kph = extractIntel(data, 12, 12) * 0.08f - 40.0f;
        uint8_t uiSpeed = (uint8_t)extractIntel(data, 24, 8);
        bool isKph = extractIntel(data, 32, 1) != 0;
        char tmp[80];
        if (kph > -30 && kph < 300)
            snprintf(tmp, sizeof(tmp), "车速：%.1f %s (UI:%u)", kph, isKph?"km/h":"mph", uiSpeed);
        else
            snprintf(tmp, sizeof(tmp), "车速：(无效)");
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x2B9 (697) DAS_control [PARTY] ----
    if (id == 0x2B9)
    {
        if (dlc < 7) return false;
        uint8_t accState = (uint8_t)extractIntel(data, 12, 4);
        float setSpeed = extractIntel(data, 0, 12) * 0.1f;
        uint64_t rawAccelMax = extractIntel(data, 44, 9);
        float accelMax = rawAccelMax == 511 ? -99 : rawAccelMax * 0.04f - 15.0f;
        uint64_t rawAccelMin = extractIntel(data, 35, 9);
        float accelMin = rawAccelMin == 511 ? -99 : rawAccelMin * 0.04f - 15.0f;
        char tmp[128];
        snprintf(tmp, sizeof(tmp), "ACC:%s 设定速度:%.0f km/h 加减速:%.2f~%.2f m/s²",
                 dasAccStateName(accState), setSpeed, accelMin, accelMax);
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x343 (835) VCRIGHT_status [VEH] ----
    if (id == 0x343)
    {
        if (dlc < 7) return false;
        uint8_t pwr = (data[1] >> 0) & 0x03;
        uint8_t defrost = (data[1] >> 3) & 0x07;
        bool ota = (data[2] >> 1) & 0x01;
        uint16_t vbatRaw = (uint16_t)extractIntel(data, 28, 12);
        float vbat = vbatRaw * 0.005443676098f;
        uint16_t tempRaw = (uint16_t)extractIntel(data, 52, 11);
        float temp = tempRaw * 0.125f - 40.0f;
        char tmp[96];
        snprintf(tmp, sizeof(tmp), "电源:%s 除霜:%s OTA:%s 电压:%.2fV 温度:%.0f°C",
                 powerStateName(pwr), defrostName(defrost),
                 ota?"中":"无", vbat, temp);
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x370 (880) EPAS3S_sysStatus [PARTY] (Motorola @0+) ----
    if (id == 0x370)
    {
        if (dlc < 7) return false;
        uint8_t handsOn = (uint8_t)extractMotorola(data, 39, 2);
        uint64_t rawSas = extractMotorola(data, 37, 14);
        float sas = rawSas * 0.1f - 819.2f;
        uint8_t eac = (uint8_t)extractMotorola(data, 55, 3);
        char tmp[96];
        snprintf(tmp, sizeof(tmp), "转向:%s EAC:%s 方向盘角度:%.1f°",
                 epasHandsOnName(handsOn), eacStatusName(eac), sas);
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x389 (905) DAS_status2 [PARTY] ----
    if (id == 0x389)
    {
        if (dlc < 7) return false;
        uint8_t fcw = (uint8_t)extractIntel(data, 48, 4);
        uint8_t lss = (uint8_t)extractIntel(data, 31, 3);
        uint16_t limit = (uint16_t)extractIntel(data, 0, 10);
        float speedLimit = limit == 1023 ? -1 : limit * 0.4f;
        char tmp[80];
        snprintf(tmp, sizeof(tmp), "FCW:%s LSS:%s 限速:%.0f mph",
                 fcw?"预警":"无", lss==0?"故障":lss==1?"LDW":lss==2?"LKA":"其他",
                 speedLimit);
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x39B (923) DAS_status [PARTY] ----
    if (id == 0x39B)
    {
        if (dlc < 6) return false;
        uint8_t apState = (uint8_t)extractIntel(data, 0, 4);
        uint8_t bsL = (uint8_t)extractIntel(data, 4, 2);
        uint8_t bsR = (uint8_t)extractIntel(data, 6, 2);
        uint8_t fcw = (uint8_t)extractIntel(data, 22, 2);
        uint8_t handsOn = (uint8_t)extractIntel(data, 42, 4);
        char tmp[128];
        snprintf(tmp, sizeof(tmp), "AP:%s 盲区:左%s/右%s FCW:%s 手握:%s",
                 dasAutopilotStateName(apState),
                 dasBlindSpotName(bsL), dasBlindSpotName(bsR),
                 fcw==1?"预警":fcw==3?"无效":"无",
                 handsOn==0?"无需":handsOn==8?"暂停":"需要");
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x39D (925) IBST_status [PARTY] ----
    if (id == 0x39D)
    {
        if (dlc < 3) return false;
        uint8_t brake = (uint8_t)extractIntel(data, 16, 2);
        uint8_t status = (uint8_t)extractIntel(data, 12, 3);
        char tmp[64];
        snprintf(tmp, sizeof(tmp), "iBooster:%s 制动:%s", ibstStatusName(status), ibstBrakeName(brake));
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x3C2 (962) VCLEFT_switchStatus [VEH] (Mux) ----
    if (id == 0x3C2)
    {
        if (dlc < 8) return false;
        uint8_t mux = data[0] & 0x03;
        if (mux == 0)
        {
            bool brake = (data[7] >> 4) & 0x01;
            bool horn = (data[0] >> 2) & 0x01;
            bool hazard = (data[0] >> 3) & 0x01;
            const char* occ = switchName((data[6] >> 2) & 0x03);
            char tmp[64];
            snprintf(tmp, sizeof(tmp), "刹车:%s 喇叭:%s 双闪:%s 占位:%s",
                     brake?"踩":"松", horn?"按":"无", hazard?"开":"关", occ);
            strlcpy(out, tmp, outLen);
            return true;
        }
        if (mux == 1)
        {
            uint8_t leftPress = (data[1] >> 5) & 0x03;
            int8_t leftScroll = (int8_t)extractSigned(extractIntel(data, 16, 6), 6);
            uint8_t rightPress = (data[1] >> 2) & 0x03;
            int8_t rightScroll = (int8_t)extractSigned(extractIntel(data, 24, 6), 6);
            char tmp[80];
            snprintf(tmp, sizeof(tmp), "左滚轮:%+d(%s) 右滚轮:%+d(%s)",
                     leftScroll, scrollName(leftPress),
                     rightScroll, scrollName(rightPress));
            strlcpy(out, tmp, outLen);
            return true;
        }
        char tmp[32];
        snprintf(tmp, sizeof(tmp), "mux=%u", mux);
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x3E9 (1001) DAS_bodyControls [VEH] ----
    if (id == 0x3E9)
    {
        if (dlc < 4) return false;
        uint8_t headlight = data[0] & 0x03;
        uint8_t turnInd = (data[1] >> 0) & 0x03;
        uint8_t wiper = (data[0] >> 4) & 0x0F;
        bool apActive = (data[3] & 0x01) != 0;
        const char* hl = headlight==1?"开":headlight==0?"关":"无效";
        const char* ti = turnInd==1?"左":turnInd==2?"右":"无";
        char tmp[96];
        snprintf(tmp, sizeof(tmp), "大灯:%s 转向:%s 雨刷:%u AP:%s",
                 hl, ti, wiper, apActive?"是":"否");
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x3F5 (1013) VCFRONT_lighting [VEH] ----
    // 信号定义:
    //   VCFRONT_indicatorLeftRequest  : 0|2@1+ — 字节0 bit0-1
    //   VCFRONT_indicatorRightRequest : 2|2@1+ — 字节0 bit2-3
    // VAL_: 0=OFF, 1=ACTIVE_LOW, 2=ACTIVE_HIGH
    // 你观察: 左转 01↔02, 右转 04↔08, 不亮 00
    if (id == 0x3F5)
    {
        if (dlc < 7) return false;
        uint8_t leftReq  = data[0] & 0x03;
        uint8_t rightReq = (data[0] >> 2) & 0x03;
        uint8_t drlL = (data[4] >> 4) & 0x03;
        uint8_t drlR = (data[4] >> 6) & 0x03;
        uint8_t lowL = (data[3] >> 4) & 0x03;
        uint8_t lowR = (data[3] >> 6) & 0x03;
        uint8_t highL = (data[4] >> 0) & 0x03;
        uint8_t highR = (data[4] >> 2) & 0x03;
        char tmp[128];
        snprintf(tmp, sizeof(tmp), "左转:%s 右转:%s 近光:%s/%s 远光:%s/%s DRL:%s/%s",
                 leftReq?"亮":"关", rightReq?"亮":"关",
                 lightStatusName(lowL), lightStatusName(lowR),
                 lightStatusName(highL), lightStatusName(highR),
                 lightStatusName(drlL), lightStatusName(drlR));
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x3F8 (1016) / 0x3FD (1021) UI_selfParkRequest ----
    if (id == 1016 || id == 0x3F8)
    {
        if (dlc < 6) return false;
        uint8_t spr = (data[3] >> 4) & 0x0F;
        char tmp[80]; snprintf(tmp, sizeof(tmp), "召唤请求：%s（spr=%u）", spr?"是":"否", spr);
        strlcpy(out, tmp, outLen);
        return true;
    }
    if (id == 1021 || id == 0x3FD)
    {
        if (dlc < 1) return false;
        uint8_t mux = data[0] & 0x07;
        char tmp[32]; snprintf(tmp, sizeof(tmp), "UI_selfPark mux=%u", mux);
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ---- 0x7FF (2047) GTW_systemStatus mux2 AP ----
    if (id == 2047 || id == 0x7FF)
    {
        if (dlc < 6) return false;
        uint8_t mux = data[0] & 0x07;
        if (mux == 2)
        {
            uint8_t ap = (data[5] >> 2) & 0x07;
            const char* apn = (ap==0?"NONE":ap==1?"HIGHWAY":ap==2?"ENHANCED":ap==3?"SELF_DRIVING":ap==4?"BASIC":"未知");
            char tmp[80]; snprintf(tmp, sizeof(tmp), "AP状态：%s", apn);
            strlcpy(out, tmp, outLen);
            return true;
        }
        char tmp[32]; snprintf(tmp, sizeof(tmp), "GTW mux=%u", mux);
        strlcpy(out, tmp, outLen);
        return true;
    }

    return false;
}
