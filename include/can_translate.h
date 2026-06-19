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

// ── formatCanTranslation: all DBC signals for 5 CAN IDs ──

inline bool formatCanTranslation(uint32_t id, const uint8_t *data, uint8_t dlc, char *out, size_t outLen)
{
    if (!out || outLen == 0) return false;
    out[0] = '\0';

    // ── 0x118 (280) DI_systemStatus ──
    // DBC: gear(21|3), accelPedalPos(32|8), regenLight(26|1), brakePedalState(19|2)
    //      trackModeState(48|2), keepAliveRequest(47|1), proximity(46|1), epbRequest(44|2)
    //      tractionControlMode(40|3), immobilizerState(27|3), systemState(16|3)
    if (id == 280 || id == 0x118)
    {
        if (dlc < 7) return false;
        uint8_t gear     = (data[2] >> 5) & 0x07;
        uint8_t accel    = data[4];
        uint8_t regen    = (data[3] >> 2) & 0x01;
        uint8_t brake    = (data[2] >> 3) & 0x03;
        uint8_t track    = (data[6] >> 0) & 0x03;
        uint8_t keepAwake = (data[5] >> 7) & 0x01;
        uint8_t prox     = (data[5] >> 6) & 0x01;
        uint8_t epb      = (data[5] >> 4) & 0x03;
        uint8_t tcm      = (data[5] >> 0) & 0x07;
        uint8_t immo     = (data[3] >> 3) & 0x07;
        uint8_t sysState = (data[2] >> 0) & 0x07;
        char tmp[256];
        snprintf(tmp, sizeof(tmp), "%s 油门:%.0f%% 动能回收:%s 制动:%s 牵引力:%s EPB:%s 保持:%s 接近:%s 防盗:%s 系统状态:%s 赛道:%s",
                 gearName(gear),
                 accel == 255 ? -1.0f : accel * 0.4f,
                 regen ? "回收" : "关",
                 brake == 1 ? "踩下" : (brake == 0 ? "松开" : "无效"),
                 tractionModeName(tcm),
                 epbRequestName(epb),
                 keepAwake ? "请求" : "无",
                 prox ? "触发" : "未触发",
                 immobilizerName(immo),
                 systemStateName(sysState),
                 track == 0 ? "不可用" : (track == 1 ? "可用" : "开启"));
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ── 0x257 (599) DI_speed ──
    // DBC: vehicleSpeed(12|12), uiSpeed(24|8), uiSpeedUnits(32|1)
    if (id == 599 || id == 0x257)
    {
        if (dlc < 5) return false;
        uint64_t rawVeh = extractIntel(data, 12, 12);
        float kph = rawVeh == 4095 ? -1.0f : rawVeh * 0.08f - 40.0f;
        uint8_t uiSpeed = data[3];
        uint8_t units   = (data[4] >> 0) & 0x01;
        char tmp[128];
        if (rawVeh == 4095)
            snprintf(tmp, sizeof(tmp), "车速:无效 显示速度:%d %s", uiSpeed, units ? "km/h" : "mph");
        else
            snprintf(tmp, sizeof(tmp), "车速:%.1fkm/h 显示:%d %s", kph, uiSpeed, units ? "km/h" : "mph");
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ── 0x389 (905) DAS_status2 ──
    // DBC: 17 signals total
    // longCollisionWarning(48|4), ppOffsetDesiredRamp(40|8), driverInteractionLevel(38|2)
    // robState(36|2), radarTelemetry(34|2), lssState(31|3), ACC_report(26|5)
    // pmmCameraFaultReason(24|2), pmmSysFaultReason(21|3), pmmRadarFaultReason(19|2)
    // pmmUltrasonicsFaultReason(16|3), activationFailureStatus(14|2)
    // pmmLoggingRequest(13|1), pmmObstacleSeverity(10|3), accSpeedLimit(0|10)
    if (id == 905 || id == 0x389)
    {
        if (dlc < 7) return false;
        uint8_t lcw   = (uint8_t)extractIntel(data, 48, 4);
        int16_t ppOff = (int16_t)extractIntel(data, 40, 8);
        float ramp    = ppOff == 128 ? 0.0f : (ppOff * 0.01f - 1.28f);
        uint8_t dil   = (uint8_t)extractIntel(data, 38, 2);
        uint8_t rob   = (uint8_t)extractIntel(data, 36, 2);
        uint8_t radar = (uint8_t)extractIntel(data, 34, 2);
        uint8_t lss   = (uint8_t)extractIntel(data, 31, 3);
        uint8_t accRpt= (uint8_t)extractIntel(data, 26, 5);
        uint8_t camFlt= (uint8_t)extractIntel(data, 24, 2);
        uint8_t sysFlt= (uint8_t)extractIntel(data, 21, 3);
        uint8_t radFlt= (uint8_t)extractIntel(data, 19, 2);
        uint8_t ultFlt= (uint8_t)extractIntel(data, 16, 3);
        uint8_t actFail=(uint8_t)extractIntel(data, 14, 2);
        uint8_t logReq= (uint8_t)extractIntel(data, 13, 1);
        uint8_t obs   = (uint8_t)extractIntel(data, 10, 3);
        uint64_t rawSpd = extractIntel(data, 0, 10);
        float accLimit  = rawSpd == 1023 ? -1.0f : rawSpd * 0.4f;
        char tmp[256];
        snprintf(tmp, sizeof(tmp),
                 "碰撞:%s 车道:%s 交互:%s ACC限速:%.0fmph 坡道偏移:%.2fm "
                 "后障:%s 雷达:%s ACC报告:%d 障碍物:%s "
                 "PM摄像头:%d 系统:%d 雷达:%d 超声:%d 激活失败:%d 日志:%s",
                 longCollisionName(lcw), lssStateName(lss),
                 dil == 0 ? "交互中" : (dil == 1 ? "未交互" : "持续未交互"),
                 accLimit < 0 ? 0.0f : accLimit, ramp,
                 robStateName(rob), radarTelemetryName(radar), accRpt,
                 pmmObstacleName(obs),
                 camFlt, sysFlt, radFlt, ultFlt, actFail,
                 logReq ? "请求" : "无");
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ── 0x39D (925) IBST_status ──
    // DBC: sInputRodDriver(21|12), internalState(18|3), driverBrakeApply(16|2)
    //      iBoosterStatus(12|3)
    if (id == 925 || id == 0x39D)
    {
        if (dlc < 3) return false;
        uint8_t  brake= (uint8_t)extractIntel(data, 16, 2);
        uint64_t rawRod= extractIntel(data, 21, 12);
        float rod     = rawRod * 0.015625f - 5.0f;
        uint8_t intSt = (uint8_t)extractIntel(data, 18, 3);
        uint8_t ibst  = (uint8_t)extractIntel(data, 12, 3);
        char tmp[128];
        snprintf(tmp, sizeof(tmp), "制动:%s 踏板:%.1fmm 内部:%s iBooster:%s",
                 brakeApplyName(brake), rod,
                 ibstInternalStateName(intSt), iBoosterStatusName(ibst));
        strlcpy(out, tmp, outLen);
        return true;
    }

    // ── 0x3F5 (1013) VCFRONT_lighting ──
    // DBC: 17 lighting fields
    if (id == 1013 || id == 0x3F5)
    {
        if (dlc < 8) return false;
        uint8_t indL = data[0] & 0x03;
        uint8_t indR = (data[0] >> 2) & 0x03;
        uint8_t lowL = (data[3] >> 4) & 0x03;
        uint8_t lowR = (data[3] >> 6) & 0x03;
        uint8_t highL = data[4] & 0x03;
        uint8_t highR = (data[4] >> 2) & 0x03;
        uint8_t drlL  = (data[4] >> 4) & 0x03;
        uint8_t drlR  = (data[4] >> 6) & 0x03;
        uint8_t fogL  = (data[5] >> 0) & 0x03;
        uint8_t fogR  = (data[5] >> 2) & 0x03;
        uint8_t srL   = (data[5] >> 6) & 0x03;
        uint8_t srR   = (data[6] >> 0) & 0x03;
        uint8_t turnL = (data[6] >> 2) & 0x03;
        uint8_t turnR = (data[6] >> 4) & 0x03;
        uint8_t parkL = (data[6] >> 6) & 0x03;
        uint8_t parkR = data[7] & 0x03;
        uint8_t haz   = (data[0] >> 4) & 0x0F;
        char tmp[256];
        snprintf(tmp, sizeof(tmp),
                 "转向指示L:%s/R:%s 近光:%s/%s 远光:%s/%s DRL:%s/%s "
                 "雾灯:%s/%s 侧灯:%s/%s 转弯灯:%s/%s 驻车:%s/%s 危险:%s",
                 reqName(indL), reqName(indR),
                 lightStatusName(lowL), lightStatusName(lowR),
                 lightStatusName(highL), lightStatusName(highR),
                 lightStatusName(drlL), lightStatusName(drlR),
                 lightStatusName(fogL), lightStatusName(fogR),
                 lightStatusName(srL), lightStatusName(srR),
                 lightStatusName(turnL), lightStatusName(turnR),
                 lightStatusName(parkL), lightStatusName(parkR),
                 haz == 0 ? "关" : (haz == 1 ? "按钮" : "激活"));
        strlcpy(out, tmp, outLen);
        return true;
    }

    return false;
}
