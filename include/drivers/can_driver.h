#pragma once

#include "../can_frame_types.h"
#include <stddef.h>
#include <stdio.h>

struct CanDriver
{
    void (*onSendFrame)(const CanFrame &, bool ok) = nullptr;

    virtual bool init() = 0;
    virtual void setFilters(const uint32_t *ids, uint8_t count) = 0;
    virtual bool enableInterrupt(void (*onReady)()) = 0;
    virtual bool read(CanFrame &frame) = 0;
    virtual bool send(const CanFrame &frame) = 0;

    virtual void diagnosticsJson(char *out, size_t outLen) const
    {
        if (!out || outLen == 0)
            return;
        snprintf(out, outLen, "{\"type\":\"generic\"}");
    }

    virtual void diagnosticsSummary(char *out, size_t outLen) const
    {
        if (!out || outLen == 0)
            return;
        snprintf(out, outLen, "CAN driver diagnostics unavailable");
    }

    virtual ~CanDriver() = default;
};
