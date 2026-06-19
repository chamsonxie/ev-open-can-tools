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

#ifndef PIN_LED
#define PIN_LED 2
#endif

using SelectedHandler = MyCanHandler;

static std::unique_ptr<CanDriver> appDriver;
static std::unique_ptr<CarManagerBase> appHandler;
static CarManagerBase *appActiveHandler = nullptr;

static volatile bool frameReady = true;
static void canISR() { frameReady = true; }

#if defined(ESP32_DASHBOARD) && !defined(NATIVE_BUILD)
#include "web/dashboard.h"
#endif

template <typename Driver>
static void appSetup(std::unique_ptr<Driver> drv, const char *readyMsg)
{
    appHandler = std::make_unique<SelectedHandler>();
    appActiveHandler = appHandler.get();
    delay(1500);
    Serial.begin(115200);
    unsigned long t0 = millis();
    while (!Serial && millis() - t0 < 1000)
    {
    }

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, HIGH);

    appDriver = std::move(drv);
    bool canInitOk = appDriver->init();
    if (!canInitOk)
    {
        Serial.println("CAN init failed");
    }
    char canDiag[640];
    appDriver->diagnosticsSummary(canDiag, sizeof(canDiag));
    Serial.print("[CAN] ");
    Serial.println(canDiag);

    appDriver->setFilters(appHandler->filterIds(), appHandler->filterIdCount());
    appDriver->diagnosticsSummary(canDiag, sizeof(canDiag));
    Serial.print("[CAN] after filters: ");
    Serial.println(canDiag);

    if constexpr (Driver::kSupportsISR)
    {
        appDriver->enableInterrupt(canISR);
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

    CanFrame frame;
    CarManagerBase *h = appActiveHandler ? appActiveHandler : appHandler.get();

    CanFrame batch[48];
    uint32_t batchIds[48];
    uint8_t batchCount = 0;
    uint32_t totalRead = 0;

    while (appDriver->read(frame))
    {
        if (frame.bus == CAN_BUS_ANY)
            frame.bus = CAN_BUS_DEFAULT;
        totalRead++;

        bool dup = false;
        for (uint8_t i = 0; i < batchCount; i++)
        {
            if (batchIds[i] == frame.id)
            {
                batch[i] = frame;
                dup = true;
                break;
            }
        }
        if (!dup && batchCount < 48)
        {
            batch[batchCount] = frame;
            batchIds[batchCount] = frame.id;
            batchCount++;
        }
    }

    h->frameCount += totalRead;
    digitalWrite(PIN_LED, LOW);
    for (uint8_t i = 0; i < batchCount; i++)
        h->handleMessage(batch[i]);
    digitalWrite(PIN_LED, HIGH);
}
