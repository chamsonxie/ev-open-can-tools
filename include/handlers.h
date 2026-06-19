#pragma once

#include <memory>
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
    Shared<bool> Parked{true};
    Shared<bool> enablePrint{true};
    Shared<uint32_t> frameCount{0};

    void (*onFrame)(const CanFrame &) = nullptr;

    static uint32_t diagnosticMillis()
    {
#ifndef NATIVE_BUILD
        return millis();
#else
        return 0;
#endif
    }

    virtual void handleMessage(const CanFrame &frame) = 0;
    virtual const uint32_t *filterIds() const = 0;
    virtual uint8_t filterIdCount() const = 0;
    virtual ~CarManagerBase() = default;
};

struct MyCanHandler : public CarManagerBase
{
    static constexpr uint32_t kFilterIds[] = {
        0x118, 0x257, 0x389, 0x39D, 0x3F5
    };

    const uint32_t *filterIds() const override
    {
        return kFilterIds;
    }
    uint8_t filterIdCount() const override { return 5; }

    void handleMessage(const CanFrame &frame) override
    {
        if (onFrame)
            onFrame(frame);

        if (frame.id == 280 && frame.dlc >= 3)
        {
            uint8_t gear = readDIGear(frame);
            Parked = isVehicleParked(gear);
        }
    }
};
