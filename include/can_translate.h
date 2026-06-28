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

inline const char *tractionModeName(uint8_t v)
{
    switch (v)
    {
    case 0: return "正常"; case 1: return "防滑起步"; case 2: return "开发模式1";
    case 3: return "开发模式2"; case 4: return "圆环模式"; case 5: return "马力机模式";
    default: return "?";
    }
}

inline const char *systemStateName(uint8_t v)
{
    switch (v)
    {
    case 0: return "不可用"; case 1: return "空闲"; case 2: return "待机";
    case 3: return "故障"; case 4: return "中止"; case 5: return "使能";
    default: return "?";
    }
}

inline const char *immobilizerName(uint8_t v)
{
    switch (v)
    {
    case 0: return "无效"; case 1: return "未锁止"; case 2: return "锁止";
    case 3: return "读钥匙"; case 4: return "已认证"; case 5: return "紧急模式";
    case 6: return "故障"; default: return "?";
    }
}

inline const char *epbRequestName(uint8_t v)
{
    switch (v)
    {
    case 0: return "无"; case 1: return "驻车"; case 2: return "释放";
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

inline const char *robStateName(uint8_t v)
{
    switch (v)
    {
    case 0: return "禁止"; case 1: return "测量中"; case 2: return "激活"; case 3: return "无地图";
    default: return "?";
    }
}

inline const char *radarTelemetryName(uint8_t v)
{
    switch (v)
    {
    case 0: return "空闲"; case 1: return "正常"; case 2: return "紧急";
    default: return "?";
    }
}

inline const char *pmmObstacleName(uint8_t v)
{
    switch (v)
    {
    case 0: return "无"; case 1: return "后方即将"; case 2: return "前方即将";
    case 5: return "前方碰撞"; default: return "?";
    }
}

inline const char *ibstInternalStateName(uint8_t v)
{
    switch (v)
    {
    case 0: return "无"; case 1: return "预检"; case 2: return "本地刹车";
    case 3: return "外部刹车"; case 4: return "诊断"; case 5: return "过渡到空闲";
    case 6: return "后检"; default: return "?";
    }
}

inline const char *iBoosterStatusName(uint8_t v)
{
    switch (v)
    {
    case 0: return "关"; case 1: return "初始化"; case 2: return "故障";
    case 3: return "诊断"; case 4: return "自检"; case 5: return "就绪";
    case 6: return "作动中"; default: return "?";
    }
}

inline const char *latchStatusName(uint8_t v)
{
    switch (v)
    {
    case 0: return "SNA"; case 1: return "开"; case 2: return "关";
    case 3: return "关紧中"; case 4: return "开锁中"; case 5: return "微开";
    case 6: return "超时"; case 7: return "默认"; case 8: return "故障";
    default: return "?";
    }
}

inline const char *mirrorStateName(uint8_t v)
{
    switch (v)
    {
    case 0: return "空闲"; case 1: return "X倾斜"; case 2: return "Y倾斜";
    case 3: return "折叠/展开"; case 4: return "记忆调出";
    default: return "?";
    }
}

inline const char *mirrorFoldName(uint8_t v)
{
    switch (v)
    {
    case 0: return "未知"; case 1: return "折叠"; case 2: return "展开";
    case 3: return "折叠中"; case 4: return "展开中";
    default: return "?";
    }
}

inline const char *mirrorRecallName(uint8_t v)
{
    switch (v)
    {
    case 0: return "空闲"; case 1: return "调用1"; case 2: return "调用2";
    case 3: return "调用3"; case 4: return "保存中"; case 5: return "调用中";
    default: return "?";
    }
}

inline const char *mirrorHeatName(uint8_t v)
{
    switch (v)
    {
    case 0: return "SNA"; case 1: return "开"; case 2: return "关";
    case 3: return "关不可用"; case 4: return "故障";
    default: return "?";
    }
}

// ── formatCanTranslation: all DBC signals for 6 CAN IDs ──
// Uses the unified signal parsers from can_helpers.h to eliminate bit-extraction duplication.

inline bool formatCanTranslation(uint32_t id, const uint8_t *data, uint8_t dlc, char *out, size_t outLen)
{
    if (!out || outLen == 0) return false;
    out[0] = '\0';

    if (id == 280 || id == 0x118)
    {
        auto s = parseDI_systemStatus(data, dlc);
        if (dlc < 7) return false;
        char tmp[256];
        snprintf(tmp, sizeof(tmp), "%s 油门:%.0f%% 动能回收:%s 制动:%s 牵引力:%s EPB:%s 保持:%s 接近:%s 防盗:%s 系统状态:%s 赛道:%s",
                 gearName(s.gear),
                 s.accelPedalPos == 255 ? -1.0f : s.accelPedalPos * 0.4f,
                 s.regenLight ? "回收" : "关",
                 s.brakePedalState == 1 ? "踩下" : (s.brakePedalState == 0 ? "松开" : "无效"),
                 tractionModeName(s.tractionControlMode),
                 epbRequestName(s.epbRequest),
                 s.keepAliveRequest ? "请求" : "无",
                 s.proximity ? "触发" : "未触发",
                 immobilizerName(s.immobilizerState),
                 systemStateName(s.systemState),
                 s.trackModeState == 0 ? "不可用" : (s.trackModeState == 1 ? "可用" : "开启"));
        strlcpy(out, tmp, outLen);
        return true;
    }

    if (id == 599 || id == 0x257)
    {
        auto s = parseDI_speed(data, dlc);
        if (dlc < 5) return false;
        float kph = s.vehicleSpeed == 4095 ? -1.0f : s.vehicleSpeed * 0.08f - 40.0f;
        char tmp[128];
        if (s.vehicleSpeed == 4095)
            snprintf(tmp, sizeof(tmp), "车速:无效 显示速度:%d %s", s.uiSpeed, s.uiSpeedUnits ? "km/h" : "mph");
        else
            snprintf(tmp, sizeof(tmp), "车速:%.1fkm/h 显示:%d %s", kph, s.uiSpeed, s.uiSpeedUnits ? "km/h" : "mph");
        strlcpy(out, tmp, outLen);
        return true;
    }

    if (id == 905 || id == 0x389)
    {
        auto s = parseDAS_status2(data, dlc);
        if (dlc < 7) return false;
        float ramp = s.ppOffsetDesiredRamp == 128 ? 0.0f : (s.ppOffsetDesiredRamp * 0.01f - 1.28f);
        float accLimit = s.accSpeedLimit == 1023 ? -1.0f : s.accSpeedLimit * 0.4f;
        char tmp[256];
        snprintf(tmp, sizeof(tmp),
                 "碰撞:%s 车道:%s 交互:%s ACC限速:%.0fmph 坡道偏移:%.2fm "
                 "后障:%s 雷达:%s ACC报告:%d 障碍物:%s "
                 "PM摄像头:%d 系统:%d 雷达:%d 超声:%d 激活失败:%d 日志:%s",
                 longCollisionName(s.longCollisionWarning), lssStateName(s.lssState),
                 s.driverInteractionLevel == 0 ? "交互中" : (s.driverInteractionLevel == 1 ? "未交互" : "持续未交互"),
                 accLimit < 0 ? 0.0f : accLimit, ramp,
                 robStateName(s.robState), radarTelemetryName(s.radarTelemetry), s.accReport,
                 pmmObstacleName(s.pmmObstacleSeverity),
                 s.pmmCameraFaultReason, s.pmmSysFaultReason, s.pmmRadarFaultReason, s.pmmUltrasonicsFaultReason,
                 s.activationFailureStatus,
                 s.pmmLoggingRequest ? "请求" : "无");
        strlcpy(out, tmp, outLen);
        return true;
    }

    if (id == 263 || id == 0x107)
    {
        auto s = parseIBST_status(data, dlc);
        if (dlc < 3) return false;
        float rod = s.brakeRodTravel * 0.015625f - 5.0f;
        char tmp[128];
        snprintf(tmp, sizeof(tmp), "制动:%s 踏板:%.1fmm 内部:%s iBooster:%s",
                 brakeApplyName(s.driverBrakeApply), rod,
                 ibstInternalStateName(s.internalState), iBoosterStatusName(s.iBoosterStatus));
        strlcpy(out, tmp, outLen);
        return true;
    }

    if (id == 1013 || id == 0x3F5)
    {
        auto s = parseVCFRONT_lighting(data, dlc);
        if (dlc < 8) return false;
        char tmp[256];
        snprintf(tmp, sizeof(tmp),
                 "转向指示L:%s/R:%s 近光:%s/%s 远光:%s/%s DRL:%s/%s "
                 "雾灯:%s/%s 侧灯:%s/%s 转弯灯:%s/%s 驻车:%s/%s 危险:%s",
                 reqName(s.indicatorLeftRequest), reqName(s.indicatorRightRequest),
                 lightStatusName(s.lowBeamLeftStatus), lightStatusName(s.lowBeamRightStatus),
                 lightStatusName(s.highBeamLeftStatus), lightStatusName(s.highBeamRightStatus),
                 lightStatusName(s.drlLeftStatus), lightStatusName(s.drlRightStatus),
                 lightStatusName(s.fogLeftStatus), lightStatusName(s.fogRightStatus),
                 lightStatusName(s.sideRepeaterLeftStatus), lightStatusName(s.sideRepeaterRightStatus),
                 lightStatusName(s.turnSignalLeftStatus), lightStatusName(s.turnSignalRightStatus),
                 lightStatusName(s.parkLeftStatus), lightStatusName(s.parkRightStatus),
                 s.hazardLightRequest == 0 ? "关" : (s.hazardLightRequest == 1 ? "按钮" : "激活"));
        strlcpy(out, tmp, outLen);
        return true;
    }

    if (id == 994 || id == 0x3E2)
    {
        auto s = parseE32_brakeLight(data, dlc);
        if (dlc < 1) return false;
        char tmp[64];
        snprintf(tmp, sizeof(tmp), "刹车灯:%s", s.brakeLight ? "亮" : "灭");
        strlcpy(out, tmp, outLen);
        return true;
    }

    if (id == 258 || id == 0x102)
    {
        auto s = parseVCLEFT_doorStatus(data, dlc);
        if (dlc < 8) return false;
        char tmp[256];
        snprintf(tmp, sizeof(tmp),
                 "左前门:%s 左后门:%s 前把手拉:%s 后把手拉:%s "
                 "前内开:%s 后内开:%s 前把PWM:%d%% 后把PWM:%d%% "
                 "镜折叠:%s 镜状态:%s 镜加热:%s 镜防眩:%s "
                 "镜倾X:%.2fV Y:%.2fV",
                 latchStatusName(s.frontLatchStatus), latchStatusName(s.rearLatchStatus),
                 s.frontHandlePulled ? "是" : "否", s.rearHandlePulled ? "是" : "否",
                 s.frontIntSwitchPressed ? "按" : "松", s.rearIntSwitchPressed ? "按" : "松",
                 s.frontHandlePWM, s.rearHandlePWM,
                 mirrorFoldName(s.mirrorFoldState), mirrorStateName(s.mirrorState),
                 mirrorHeatName(s.mirrorHeatState), s.mirrorDipped ? "开" : "关",
                 s.mirrorTiltXPosition * 0.02f, s.mirrorTiltYPosition * 0.02f);
        strlcpy(out, tmp, outLen);
        return true;
    }

    if (id == 259 || id == 0x103)
    {
        auto s = parseVCRIGHT_doorStatus(data, dlc);
        if (dlc < 8) return false;
        char tmp[256];
        snprintf(tmp, sizeof(tmp),
                 "右前门:%s 右后门:%s 尾门:%s 前把手拉:%s 后把手拉:%s "
                 "前内开:%s 后内开:%s 前把PWM:%d%% 后把PWM:%d%% "
                 "镜折叠:%s 镜状态:%s 镜防眩:%s "
                 "镜倾X:%.2fV Y:%.2fV",
                 latchStatusName(s.frontLatchStatus), latchStatusName(s.rearLatchStatus),
                 latchStatusName(s.trunkLatchStatus),
                 s.frontHandlePulled ? "是" : "否", s.rearHandlePulled ? "是" : "否",
                 s.frontIntSwitchPressed ? "按" : "松", s.rearIntSwitchPressed ? "按" : "松",
                 s.frontHandlePWM, s.rearHandlePWM,
                 mirrorFoldName(s.mirrorFoldState), mirrorStateName(s.mirrorState),
                 s.mirrorDipped ? "开" : "关",
                 s.mirrorTiltXPosition * 0.02f, s.mirrorTiltYPosition * 0.02f);
        strlcpy(out, tmp, outLen);
        return true;
    }

    return false;
}
