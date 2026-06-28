#pragma once

#include "can_frame_types.h"
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

// ── CRTP base: no virtual methods on the hot path ──
template <typename Derived>
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

    // CRTP dispatch — zero-overhead, no vtable
    void handleMessage(const CanFrame &frame)
    {
        static_cast<Derived *>(this)->handleMessage(frame);
    }

    const uint32_t *filterIds() const
    {
        return static_cast<const Derived *>(this)->filterIds();
    }

    uint8_t filterIdCount() const
    {
        return static_cast<const Derived *>(this)->filterIdCount();
    }
};

struct MyCanHandler : public CarManagerBase<MyCanHandler>
{
    Shared<bool> filterEnabled{true};

    static constexpr uint32_t kFilterIds[] = {
        0x118, 0x257, 0x389, 0x117, 0x3F5, 0x3E2, 0x102};

    const uint32_t *filterIds() const
    {
        return kFilterIds;
    }
    uint8_t filterIdCount() const
    {
        return filterEnabled ? (sizeof(kFilterIds) / sizeof(kFilterIds[0])) : 0;
    }

    void handleMessage(const CanFrame &frame)
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
