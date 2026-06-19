#pragma once

#include <memory>
#include "can_frame_types.h"
#include "drivers/can_driver.h"
#include "can_helpers.h"
#include "handlers.h"

#ifndef NATIVE_BUILD
#ifdef ESP_PLATFORM
#include "platform/espidf_runtime.h"
#else
#include <Arduino.h>
#endif
#endif

#ifdef NATIVE_BUILD
// Stubs so CanFrameBatcher tests can compile without Arduino
static void delay(unsigned long) {}
static void pinMode(int, int) {}
static void digitalWrite(int, int) {}
inline unsigned long millis() { return 0; }
class SerialStub
{
public:
    void begin(int) {}
    void print(const char *) {}
    void println(const char *) {}
    bool operator!() const { return true; }
};
static SerialStub Serial;
#ifndef OUTPUT
#define OUTPUT 1
#endif
#ifndef HIGH
#define HIGH 1
#endif
#ifndef LOW
#define LOW 0
#endif
#endif

#ifndef PIN_LED
#define PIN_LED 2
#endif

using SelectedHandler = MyCanHandler;

// ── AppContext: encapsulated global state ──
struct AppContext
{
    std::unique_ptr<CanDriver> driver;
    std::unique_ptr<SelectedHandler> handler;
    SelectedHandler *activeHandler = nullptr;

    static AppContext &instance()
    {
        static AppContext ctx;
        return ctx;
    }

    SelectedHandler *current()
    {
        return activeHandler ? activeHandler : handler.get();
    }
};

static volatile bool frameReady = true;
static void canISR() { frameReady = true; }

#if defined(ESP32_DASHBOARD) && !defined(NATIVE_BUILD)
#include "web/dashboard.h"
#endif

// ── CanFrameBatcher: deduplicate by ID before processing ──
struct CanFrameBatcher
{
    static constexpr uint8_t kMaxBatch = 48;

    CanFrame batch[kMaxBatch];
    uint32_t ids[kMaxBatch];
    uint8_t count = 0;
    uint32_t totalRead = 0;

    void add(const CanFrame &frame)
    {
        totalRead++;

        for (uint8_t i = 0; i < count; i++)
        {
            if (ids[i] == frame.id)
            {
                batch[i] = frame;
                return;
            }
        }
        if (count < kMaxBatch)
        {
            batch[count] = frame;
            ids[count] = frame.id;
            count++;
        }
    }

    void process(SelectedHandler *h)
    {
        h->frameCount += totalRead;
        for (uint8_t i = 0; i < count; i++)
            h->handleMessage(batch[i]);
    }
};

template <typename Driver>
static void appSetup(std::unique_ptr<Driver> drv, const char *readyMsg)
{
    auto &ctx = AppContext::instance();
    ctx.handler = std::make_unique<SelectedHandler>();
    ctx.activeHandler = ctx.handler.get();
    delay(1500);
    Serial.begin(115200);
    unsigned long t0 = millis();
    while (!Serial && millis() - t0 < 1000)
    {
    }

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, HIGH);

    ctx.driver = std::move(drv);
    bool canInitOk = ctx.driver->init();
    if (!canInitOk)
    {
        Serial.println("CAN init failed");
    }
    char canDiag[640];
    ctx.driver->diagnosticsSummary(canDiag, sizeof(canDiag));
    Serial.print("[CAN] ");
    Serial.println(canDiag);

    ctx.driver->setFilters(ctx.handler->filterIds(), ctx.handler->filterIdCount());
    ctx.driver->diagnosticsSummary(canDiag, sizeof(canDiag));
    Serial.print("[CAN] after filters: ");
    Serial.println(canDiag);

    if constexpr (Driver::kSupportsISR)
    {
        ctx.driver->enableInterrupt(canISR);
    }

    Serial.println(readyMsg);

#if defined(ESP32_DASHBOARD) && !defined(NATIVE_BUILD)
    delay(2000);
#endif
}

template <typename Driver>
static void appLoop()
{
#if defined(ESP32_DASHBOARD) && !defined(NATIVE_BUILD)
    if (Update.isRunning())
    {
        delay(1);
        return;
    }
#endif

    if constexpr (Driver::kSupportsISR)
    {
        if (!frameReady)
            return;
        frameReady = false;
    }

    auto &ctx = AppContext::instance();
    SelectedHandler *h = ctx.current();

    CanFrameBatcher batcher;
    CanFrame frame;

    while (ctx.driver->read(frame))
    {
        if (frame.bus == CAN_BUS_ANY)
            frame.bus = CAN_BUS_DEFAULT;
        batcher.add(frame);
    }

    digitalWrite(PIN_LED, LOW);
    batcher.process(h);
    digitalWrite(PIN_LED, HIGH);
}
