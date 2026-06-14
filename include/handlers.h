#pragma once

#include <memory>
#include <algorithm>
#include "can_frame_types.h"
#include "drivers/can_driver.h"
#include "can_helpers.h"
#include "shared_types.h"
#include "log_buffer.h"

#ifndef NATIVE_BUILD
#ifdef ESP_PLATFORM
#include "platform/espidf_runtime.h"
#else
#include <Arduino.h>
#endif
#endif

inline LogRingBuffer logRing;

struct CarManagerBase
{
    Shared<int> speedProfile{1};
    Shared<bool> speedProfileAuto{true};
    Shared<bool> ADEnabled{false};
    Shared<bool> APActive{false};
    // 默认 Parked=true，以便当DI休眠时（例如车辆锁闭且哨兵模式开启，
    // CAN ID 280未广播），AP注入门在模块启动时立即打开。
    // 第一个带有行驶档位（R/N/D）的DI_systemStatus帧将其翻转为false；
    // 如果280从未到达，则车辆处于休眠/驻车状态，门按设计保持打开。
    Shared<bool> Parked{true};
    Shared<bool> Summoning{false};
    Shared<int> gatewayAutopilot{-1};
    Shared<bool> enablePrint{true};
    Shared<uint32_t> frameCount{0};
    Shared<uint32_t> framesSent{0};
    Shared<int> speedOffset{0};
    Shared<uint32_t> last921Ms{0};
    Shared<uint32_t> last280Ms{0};
    Shared<uint32_t> last390Ms{0};
    Shared<uint32_t> last1016Ms{0};
    Shared<uint32_t> last1021Ms{0};
    Shared<int> dasAutopilotStatus{-1};

    unsigned long lastSummonActivityMs = 0;
    // 召唤与AP/TACC鉴别状态。仅ACA（DI_autonomyControlActive）
    // 在AP、TACC和智能召唤期间设置，因此它不能作为唯一的门控信号。
    // 只有当我们也在当前自主驾驶会话期间观察到UI_selfParkRequest变为非零时，
    // 我们才将ACA视为"召唤激活"。ACA下降沿清除sprSeen，
    // 这样下一个ACA上升沿（例如用户在完成召唤后启用TACC）不会错误地保持门打开。
    bool sprSeen = false;
    bool lastAca = false;

    void (*onFrame)(const CanFrame &) = nullptr;
    void (*onSend)(uint8_t mux, bool ok) = nullptr;
    bool (*checkAD)() = nullptr;
    bool (*checkNag)() = nullptr;
    bool (*checkSummon)() = nullptr;
    bool (*checkIsa)() = nullptr;
    bool (*checkEvd)() = nullptr;

    bool injectionGateOpen() const
    {
        return (bool)APActive || (bool)Parked || (bool)Summoning;
    }

    static uint32_t diagnosticMillis()
    {
#ifndef NATIVE_BUILD
        return millis();
#else
        return 0;
#endif
    }

    void updateGateFrameDiagnostics(const CanFrame &frame)
    {
        uint32_t now = diagnosticMillis();
        if (frame.id == 921)
        {
            last921Ms = now;
            if (frame.dlc >= 1)
                dasAutopilotStatus = readDASAutopilotStatus(frame);
        }
        else if (frame.id == 280)
            last280Ms = now;
        else if (frame.id == 390)
            last390Ms = now;
        else if (frame.id == 1016)
            last1016Ms = now;
        else if (frame.id == 1021)
            last1021Ms = now;
    }

    // 根据当前sprSeen + lastAca状态重新计算Summoning。召唤
    // 需要同时满足：ACA位当前已设置且我们在当前自主驾驶会话中至少看到
    // 一个非零的UI_selfParkRequest命令。
    // 这排除了纯TACC（ACA=1，无spr）和AP后的ACA尾部
    // （ACA闪烁但无新的spr）锁住门。
    void recomputeSummoning()
    {
        Summoning = lastAca && sprSeen;
    }

    // 从UI_driverAssistControl（CAN ID 1016）更新召唤状态。
    // Tesla DBC：UI_selfParkRequest位于字节3的第4-7位（4=预备, 5=暂停,
    // 7/8=自动召唤前进/后退, 11=智能召唤, 0=无）。记录在当前自主驾驶会话期间
    // 已发出召唤命令。
    void updateSummonFrom1016(const CanFrame &frame)
    {
        if (frame.dlc < 4)
            return;
        uint8_t spr = static_cast<uint8_t>((frame.data[3] >> 4) & 0x0F);
        if (spr != 0)
            sprSeen = true;
        recomputeSummoning();
    }

    // 从DI_systemStatus（CAN ID 280）更新召唤状态。
    // Tesla DBC：DI_autonomyControlActive位于第50位（字节6第2位）。
    // 当DI由AP、TACC、智能召唤等驱动时保持高电平。
    // ACA下降沿结束自主驾驶会话并清除sprSeen，
    // 以便后续的TACC接合（再次ACA=1）不会重新锁住门。
    void updateSummonFromDISystemStatus(const CanFrame &frame)
    {
        if (frame.dlc < 7)
            return;
        bool aca = (frame.data[6] & 0x04) != 0;
        if (lastAca && !aca)
            sprSeen = false;
        lastAca = aca;
        recomputeSummoning();
    }

    // 当观察到车辆处于驻车状态且无活跃的自主驾驶会话时，
    // 强制关闭Summoning并重置sprSeen，以便后续手动P->D切换
    // 正确等待AP。在智能召唤启动期间，
    // DI可以在档位仍为P时报告ACA=1；保持sprSeen锁定，
    // 使其在待处理的驶出驻车切换期间仍然有效。
    void clearSummonOnPark()
    {
        Summoning = false;
        sprSeen = false;
#ifndef NATIVE_BUILD
        lastSummonActivityMs = 0;
#endif
    }

    void clearSummonOnParkIfAcaInactive(uint8_t gear)
    {
        if (gear == 1 && !lastAca)
            clearSummonOnPark();
    }

    bool shouldInjectSpeedProfile() const
    {
#if defined(ESP32_DASHBOARD)
        return !speedProfileAuto;
#else
        return true;
#endif
    }

    virtual void handleMessage(CanFrame &frame, CanDriver &driver) = 0;
    virtual const uint32_t *filterIds() const = 0;
    virtual uint8_t filterIdCount() const = 0;
    virtual ~CarManagerBase() = default;
};

struct LegacyHandler : public CarManagerBase
{
    const uint32_t *filterIds() const override
    {
        static constexpr uint32_t ids[] = {69, 280, 390, 921, 1006};
        return ids;
    }
    uint8_t filterIdCount() const override { return 5; }

    void handleMessage(CanFrame &frame, CanDriver &driver) override
    {
        if (onFrame)
            onFrame(frame);
        updateGateFrameDiagnostics(frame);
        // STW_ACTN_RQ（0x045 = 69）：跟车距离拨杆作为配置文件映射源
        // byte[1]：0x00=位置1, 0x21=位置2, 0x42=位置3, 0x64=位置4, 0x85=位置5, 0xA6=位置6, 0xC8=位置7
        if (frame.id == 69)
        {
            if (frame.dlc < 2)
                return;
            if (!speedProfileAuto)
                return;
            uint8_t pos = frame.data[1] >> 5;
            if (pos <= 1)
                speedProfile = 2;
            else if (pos == 2)
                speedProfile = 1;
            else
                speedProfile = 0;
            return;
        }
        if (frame.id == 280)
        {
            if (frame.dlc < 3)
                return;
            {
                uint8_t diGear = readDIGear(frame);
                Parked = isVehicleParked(diGear);
                // 仅在*明确的*驻车（gear==1）时清除Summoning。
                // SNA（7）和无效（0）可能在档位切换期间短暂出现
                // （例如在召唤切换到倒车档时），否则会在召唤中途关闭门。
                updateSummonFromDISystemStatus(frame);
                clearSummonOnParkIfAcaInactive(diGear);
            }
            return;
        }
        if (frame.id == 390)
        {
            if (frame.dlc < 8)
                return;
            {
                uint8_t difGear = readVehicleGear(frame);
                Parked = isVehicleParked(difGear);
                // 仅在*明确的*驻车（gear==1）时清除Summoning。
                // SNA（7）和无效（0）可能在档位切换期间短暂出现。
                clearSummonOnParkIfAcaInactive(difGear);
            }
            return;
        }
        if (frame.id == 921)
        {
            if (frame.dlc < 1)
                return;
            APActive = isDASAutopilotActive(readDASAutopilotStatus(frame));
            return;
        }
        if (frame.id == 1006)
        {
            if (frame.dlc < 8)
                return;
            auto index = readMuxID(frame);
            if (index == 0)
                ADEnabled = isADSelectedInUI(frame) && (!checkAD || checkAD());
            if (index == 0 && ADEnabled && shouldInjectSpeedProfile() && (!checkAD || checkAD()))
            {
                setSpeedProfileV12V13(frame, speedProfile);
                setBit(frame, 46, true);
                framesSent++;
                driver.send(frame);
                if (onSend)
                    onSend(0, true);
            }
            if (index == 1 && (!checkNag || checkNag()))
            {
#if !defined(ESP32_DASHBOARD)
                setBit(frame, 19, false);
                framesSent++;
                driver.send(frame);
                if (onSend)
                    onSend(1, true);
#endif
            }
            if (index == 0 && enablePrint)
            {
                char buf[LogRingBuffer::kMaxMsgLen];
                snprintf(buf, sizeof(buf), "LegacyHandler: AD: %d, Profile: %d",
                         (bool)ADEnabled, (int)speedProfile);
                logRing.push(buf,
#ifndef NATIVE_BUILD
                             millis()
#else
                             0
#endif
                );
#ifndef NATIVE_BUILD
                Serial.println(buf);
#endif
            }
        }
    }
};

struct HW3Handler : public CarManagerBase
{
    const uint32_t *filterIds() const override
    {
        static constexpr uint32_t ids[] = {280, 390, 921, 1016, 1021, 2047};
        return ids;
    }
    uint8_t filterIdCount() const override { return 6; }

    void handleMessage(CanFrame &frame, CanDriver &driver) override
    {
        if (onFrame)
            onFrame(frame);
        updateGateFrameDiagnostics(frame);
        if (frame.id == 280)
        {
            if (frame.dlc < 3)
                return;
            {
                uint8_t diGear = readDIGear(frame);
                Parked = isVehicleParked(diGear);
                // 仅在*明确的*驻车（gear==1）时清除Summoning。
                // SNA（7）和无效（0）可能在档位切换期间短暂出现
                // （例如在召唤切换到倒车档时），否则会在召唤中途关闭门。
                updateSummonFromDISystemStatus(frame);
                clearSummonOnParkIfAcaInactive(diGear);
            }
            return;
        }
        if (frame.id == 390)
        {
            if (frame.dlc < 8)
                return;
            {
                uint8_t difGear = readVehicleGear(frame);
                Parked = isVehicleParked(difGear);
                // 仅在*明确的*驻车（gear==1）时清除Summoning。
                // SNA（7）和无效（0）可能在档位切换期间短暂出现。
                clearSummonOnParkIfAcaInactive(difGear);
            }
            return;
        }
        if (frame.id == 1016)
        {
            if (frame.dlc < 6)
                return;
            updateSummonFrom1016(frame);
            if (!speedProfileAuto)
                return;
            uint8_t followDistance = (frame.data[5] & 0b11100000) >> 5;
            switch (followDistance)
            {
            case 1:
                speedProfile = 2;
                break;
            case 2:
                speedProfile = 1;
                break;
            case 3:
                speedProfile = 0;
                break;
            default:
                break;
            }
            return;
        }
        if (frame.id == 921)
        {
            if (frame.dlc < 1)
                return;
            APActive = isDASAutopilotActive(readDASAutopilotStatus(frame));
            return;
        }
        if (frame.id == 2047)
        {
            if (frame.dlc < 6)
                return;
            if (readMuxID(frame) != 2)
                return;

            uint8_t next = readGTWAutopilot(frame);
            int prev = gatewayAutopilot;
            gatewayAutopilot = next;

            if (enablePrint && prev != next)
            {
                char buf[LogRingBuffer::kMaxMsgLen];
                snprintf(buf, sizeof(buf), "HW3Handler: GTW_autopilot: %d -> %u (%s)",
                         prev, (unsigned int)next, describeGTWAutopilot(next));
                logRing.push(buf,
#ifndef NATIVE_BUILD
                             millis()
#else
                             0
#endif
                );
#ifndef NATIVE_BUILD
                Serial.println(buf);
#endif
            }
            return;
        }
        if (frame.id == 1021)
        {
            if (frame.dlc < 8)
                return;
            auto index = readMuxID(frame);
            if (index == 0)
                ADEnabled = isADSelectedInUI(frame) && (!checkAD || checkAD());
            if (index == 0 && ADEnabled && (!checkAD || checkAD()))
            {
                speedOffset = std::max(std::min(((uint8_t)((frame.data[3] >> 1) & 0x3F) - 30) * 5, 100), 0);
#if defined(ESP32_DASHBOARD)
                if (shouldInjectSpeedProfile())
                {
                    setSpeedProfileV12V13(frame, speedProfile);
                    setBit(frame, 46, true);
                    framesSent++;
                    driver.send(frame);
                    if (onSend)
                        onSend(0, true);
                }
#else
                setBit(frame, 46, true);
                framesSent++;
                driver.send(frame);
                if (onSend)
                    onSend(0, true);
#endif
            }
            if (index == 1)
            {
#if !defined(ESP32_DASHBOARD)
                bool modified = false;
#if defined(ENHANCED_AUTOPILOT)
                if (enhancedAutopilotRuntime && enhancedAutopilotInjectionAllowed(injectionGateOpen()))
                {
                    setBit(frame, 19, false);
                    setBit(frame, 46, true);
                    modified = true;
                }
#else
                // 无仪表盘：鸣叫抑制始终开启（RP2040, M4）
                setBit(frame, 19, false);
                modified = true;
#endif
                if (modified)
                {
                    framesSent++;
                    driver.send(frame);
                    if (onSend)
                        onSend(1, true);
                }
#endif
            }
            if (index == 0 && enablePrint)
            {
                char buf[LogRingBuffer::kMaxMsgLen];
                snprintf(buf, sizeof(buf), "HW3Handler: AD: %d, Profile: %d, Offset: %d",
                         (bool)ADEnabled, (int)speedProfile, (int)speedOffset);
                logRing.push(buf,
#ifndef NATIVE_BUILD
                             millis()
#else
                             0
#endif
                );
#ifndef NATIVE_BUILD
                Serial.println(buf);
#endif
            }
        }
    }
};

/**
 * NagHandler — 自动辅助转向鸣叫抑制（计数器+1回显方法）
 *
 * 复现中国TSL6P模块的行为：
 * - 监听CAN 880（0x370）= EPAS3P_sysStatus
 * - 当handsOnLevel = 0（将会触发鸣叫）时：
 *   1. 复制真实帧
 *   2. 设置字节3 = 0xB6（固定torsionBarTorque = 1.80 Nm）
 *   3. 设置字节4 |= 0x40（handsOnLevel = 1）
 *   4. 递增计数器（字节6低半字节 + 1）
 *   5. 重新计算校验和（字节7）
 * - 具有相同计数器的真实EPAS帧稍后到达 -> 作为重复帧被拒绝
 *
 * 测试通过：Model Y Performance 2022 HW3，基础版自动辅助转向
 * 总线：X179 pin 2/3（CAN总线4）
 *
 * 使用构建标志启用：-D NAG_KILLER
 */
struct NagHandler : public CarManagerBase
{
    Shared<bool> nagKillerActive{true};
    Shared<uint32_t> nagEchoCount{0};

    const uint32_t *filterIds() const override
    {
        static constexpr uint32_t ids[] = {880};
        return ids;
    }
    uint8_t filterIdCount() const override { return 1; }

    void handleMessage(CanFrame &frame, CanDriver &driver) override
    {
        if (frame.id != 880 || frame.dlc < 8)
            return;

        uint8_t handsOn = (frame.data[4] >> 6) & 0x03;

        if (!nagKillerActive || !nagKillerRuntime || handsOn != 0)
            return;

        CanFrame echo;
        echo.id = 880;
        echo.dlc = 8;

        echo.data[0] = frame.data[0];
        echo.data[1] = frame.data[1];
        echo.data[2] = (frame.data[2] & 0xF0) | 0x08;
        echo.data[5] = frame.data[5];

        // 固定扭矩 = 1.80 Nm（tRaw = 0x08B6）
        echo.data[3] = 0xB6;

        // handsOnLevel 设置为 1
        echo.data[4] = frame.data[4] | 0x40;

        // 计数器 + 1
        uint8_t cnt = (frame.data[6] & 0x0F);
        cnt = (cnt + 1) & 0x0F;
        echo.data[6] = (frame.data[6] & 0xF0) | cnt;

        // 校验和：sum(byte0..byte6) + 0x73
        uint16_t sum = echo.data[0] + echo.data[1] + echo.data[2] + echo.data[3] + echo.data[4] + echo.data[5] + echo.data[6];
        echo.data[7] = static_cast<uint8_t>((sum + 0x73) & 0xFF);

        framesSent++;
        nagEchoCount++;
        driver.send(echo);

        if (enablePrint && (nagEchoCount % 500 == 1))
        {
            char buf[LogRingBuffer::kMaxMsgLen];
            snprintf(buf, sizeof(buf), "NagHandler: echo=%u",
                     (unsigned int)(uint32_t)nagEchoCount);
            logRing.push(buf,
#ifndef NATIVE_BUILD
                         millis()
#else
                         0
#endif
            );
#ifndef NATIVE_BUILD
            Serial.println(buf);
#endif
        }
    }
};

struct HW4Handler : public CarManagerBase
{
    const uint32_t *filterIds() const override
    {
#if defined(ISA_SPEED_CHIME_SUPPRESS) && !defined(ESP32_DASHBOARD)
        static constexpr uint32_t ids[] = {280, 390, 921, 1016, 1021, 2047};
        return ids;
    }
    uint8_t filterIdCount() const override { return 6; }
#else
        static constexpr uint32_t ids[] = {280, 390, 921, 1016, 1021, 2047};
        return ids;
    }
    uint8_t filterIdCount() const override { return 6; }
#endif

    void handleMessage(CanFrame &frame, CanDriver &driver) override
    {
        if (onFrame)
            onFrame(frame);
        updateGateFrameDiagnostics(frame);
        if (frame.id == 280)
        {
            if (frame.dlc < 3)
                return;
            {
                uint8_t diGear = readDIGear(frame);
                Parked = isVehicleParked(diGear);
                // 仅在*明确的*驻车（gear==1）时清除Summoning。
                // SNA（7）和无效（0）可能在档位切换期间短暂出现
                // （例如在召唤切换到倒车档时），否则会在召唤中途关闭门。
                updateSummonFromDISystemStatus(frame);
                clearSummonOnParkIfAcaInactive(diGear);
            }
            return;
        }
        if (frame.id == 390)
        {
            if (frame.dlc < 8)
                return;
            {
                uint8_t difGear = readVehicleGear(frame);
                Parked = isVehicleParked(difGear);
                // 仅在*明确的*驻车（gear==1）时清除Summoning。
                // SNA（7）和无效（0）可能在档位切换期间短暂出现。
                clearSummonOnParkIfAcaInactive(difGear);
            }
            return;
        }
        if (frame.id == 921)
        {
            if (frame.dlc < 1)
                return;
            APActive = isDASAutopilotActive(readDASAutopilotStatus(frame));
        }
#if defined(ISA_SPEED_CHIME_SUPPRESS) && !defined(ESP32_DASHBOARD)
        if (isaSpeedChimeSuppressRuntime && frame.id == 921)
        {
            if (frame.dlc < 8)
                return;
            if (!isaSpeedChimeSuppressRuntime)
                return;
            frame.data[1] |= 0x20;
            uint8_t sum = 0;
            for (int i = 0; i < 7; i++)
                sum += frame.data[i];
            sum += (921 & 0xFF) + (921 >> 8);
            frame.data[7] = sum & 0xFF;
            framesSent++;
            driver.send(frame);
            if (onSend)
                onSend(0, true);
            return;
        }
#endif
        if (frame.id == 1016)
        {
            if (frame.dlc < 6)
                return;
            updateSummonFrom1016(frame);
            if (!speedProfileAuto)
                return;
            auto fd = (frame.data[5] & 0b11100000) >> 5;
            switch (fd)
            {
            case 1:
                speedProfile = 3;
                break;
            case 2:
                speedProfile = 2;
                break;
            case 3:
                speedProfile = 1;
                break;
            case 4:
                speedProfile = 0;
                break;
            case 5:
                speedProfile = 4;
                break;
            }
        }
        if (frame.id == 2047)
        {
            if (frame.dlc < 6)
                return;
            if (readMuxID(frame) != 2)
                return;

            uint8_t next = readGTWAutopilot(frame);
            int prev = gatewayAutopilot;
            gatewayAutopilot = next;

            if (enablePrint && prev != next)
            {
                char buf[LogRingBuffer::kMaxMsgLen];
                snprintf(buf, sizeof(buf), "HW4Handler: GTW_autopilot: %d -> %u (%s)",
                         prev, (unsigned int)next, describeGTWAutopilot(next));
                logRing.push(buf,
#ifndef NATIVE_BUILD
                             millis()
#else
                             0
#endif
                );
#ifndef NATIVE_BUILD
                Serial.println(buf);
#endif
            }
            return;
        }
        if (frame.id == 1021)
        {
            if (frame.dlc < 8)
                return;
            auto index = readMuxID(frame);
            if (index == 0)
                ADEnabled = isADSelectedInUI(frame) && (!checkAD || checkAD());
            if (index == 0 && ADEnabled && (!checkAD || checkAD()))
            {
#if !defined(ESP32_DASHBOARD)
                setBit(frame, 46, true);
                setBit(frame, 60, true);
#if defined(EMERGENCY_VEHICLE_DETECTION) && !defined(ESP32_DASHBOARD)
                if (emergencyVehicleDetectionRuntime)
                    setBit(frame, 59, true);
#endif
                framesSent++;
                driver.send(frame);
                if (onSend)
                    onSend(0, true);
#endif
            }
            if (index == 2 && ADEnabled && !speedProfileAuto && (!checkAD || checkAD()))
            {
                setSpeedProfileHW4(frame, speedProfile);
            }
            if (index == 1)
            {
#if !defined(ESP32_DASHBOARD)
                bool modified = false;
#if defined(ENHANCED_AUTOPILOT)
                if (enhancedAutopilotRuntime && enhancedAutopilotInjectionAllowed(injectionGateOpen()))
                {
                    setBit(frame, 19, false);
                    setBit(frame, 47, true);
                    modified = true;
                }
#else
                // 无仪表盘：鸣叫抑制始终开启（RP2040, M4）
                setBit(frame, 19, false);
                setBit(frame, 47, true);
                modified = true;
#endif
                if (modified)
                {
                    framesSent++;
                    driver.send(frame);
                    if (onSend)
                        onSend(1, true);
                }
#endif
            }
            if (index == 0 && enablePrint)
            {
                char buf[LogRingBuffer::kMaxMsgLen];
                snprintf(buf, sizeof(buf), "HW4Handler: AD: %d, Profile: %d",
                         (bool)ADEnabled, (int)speedProfile);
                logRing.push(buf,
#ifndef NATIVE_BUILD
                             millis()
#else
                             0
#endif
                );
#ifndef NATIVE_BUILD
                Serial.println(buf);
#endif
            }
        }
    }
};
