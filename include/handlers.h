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
    Shared<bool> APActive{false};
    Shared<bool> Parked{true};
    Shared<bool> Summoning{false};
    Shared<int> gatewayAutopilot{-1};
    Shared<bool> enablePrint{true};
    Shared<uint32_t> frameCount{0};
    Shared<int> speedOffset{0};
    Shared<uint32_t> last921Ms{0};
    Shared<uint32_t> last280Ms{0};
    Shared<uint32_t> last390Ms{0};
    Shared<uint32_t> last1016Ms{0};
    Shared<uint32_t> last1021Ms{0};
    Shared<int> dasAutopilotStatus{-1};

    unsigned long lastSummonActivityMs = 0;
    bool sprSeen = false;
    bool lastAca = false;

    void (*onFrame)(const CanFrame &) = nullptr;

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

    void recomputeSummoning()
    {
        Summoning = lastAca && sprSeen;
    }

    void updateSummonFrom1016(const CanFrame &frame)
    {
        if (frame.dlc < 4)
            return;
        uint8_t spr = static_cast<uint8_t>((frame.data[3] >> 4) & 0x0F);
        if (spr != 0)
            sprSeen = true;
        recomputeSummoning();
    }

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

    virtual void handleMessage(const CanFrame &frame) = 0;
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

    void handleMessage(const CanFrame &frame) override
    {
        if (onFrame)
            onFrame(frame);
        updateGateFrameDiagnostics(frame);
        if (frame.id == 69)
        {
            return;
        }
        if (frame.id == 280)
        {
            if (frame.dlc < 3)
                return;
            {
                uint8_t diGear = readDIGear(frame);
                Parked = isVehicleParked(diGear);
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
            if (enablePrint)
            {
                char buf[LogRingBuffer::kMaxMsgLen];
                snprintf(buf, sizeof(buf), "LegacyHandler: mux=%u", (unsigned int)index);
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

    void handleMessage(const CanFrame &frame) override
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
                clearSummonOnParkIfAcaInactive(difGear);
            }
            return;
        }
        if (frame.id == 1016)
        {
            if (frame.dlc < 6)
                return;
            updateSummonFrom1016(frame);
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
            if (enablePrint)
            {
                char buf[LogRingBuffer::kMaxMsgLen];
                snprintf(buf, sizeof(buf), "HW3Handler: mux=%u", (unsigned int)index);
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

struct NagHandler : public CarManagerBase
{
    Shared<bool> nagKillerActive{true};

    const uint32_t *filterIds() const override
    {
        static constexpr uint32_t ids[] = {880};
        return ids;
    }
    uint8_t filterIdCount() const override { return 1; }

    void handleMessage(const CanFrame &frame) override
    {
    }
};

struct HW4Handler : public CarManagerBase
{
    const uint32_t *filterIds() const override
    {
        static constexpr uint32_t ids[] = {280, 390, 921, 1016, 1021, 2047};
        return ids;
    }
    uint8_t filterIdCount() const override { return 6; }

    void handleMessage(const CanFrame &frame) override
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
        if (frame.id == 1016)
        {
            if (frame.dlc < 6)
                return;
            updateSummonFrom1016(frame);
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
            if (enablePrint)
            {
                char buf[LogRingBuffer::kMaxMsgLen];
                snprintf(buf, sizeof(buf), "HW4Handler: mux=%u", (unsigned int)index);
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
