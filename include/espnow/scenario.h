#pragma once

#include "protocol.h"
#include "signals.h"

struct EspNowScenarioStep
{
    unsigned long durationMs;
    uint8_t gear;
    uint8_t accel;
    uint16_t speed;
    uint8_t brakeApply;
};

static const EspNowScenarioStep espnowDefaultScenario[] = {
    {2000, 1, 0,   0,   0},
    {3000, 4, 40,  400, 0},
    {2000, 4, 25,  500, 0},
    {2500, 4, 0,   300, 2},
    {2000, 4, 20,  350, 0},
    {1000, 1, 0,   0,   0},
};
static constexpr int espnowScenarioStepCount =
    sizeof(espnowDefaultScenario) / sizeof(espnowDefaultScenario[0]);

static bool espnowScenarioActive = false;
static int espnowScenarioStep = 0;
static unsigned long espnowScenarioStepStarted = 0;
static uint16_t espnowScenarioPrevSpeed = 0;

static void espnowStartScenario()
{
    espnowScenarioActive = true;
    espnowScenarioStep = 0;
    espnowScenarioStepStarted = millis();
    espnowScenarioPrevSpeed = 0;
}

static void espnowTickScenario()
{
    if (!espnowScenarioActive)
        return;
    unsigned long now = millis();
    const EspNowScenarioStep &step = espnowDefaultScenario[espnowScenarioStep];
    unsigned long elapsed = now - espnowScenarioStepStarted;

    if (elapsed >= step.durationMs)
    {
        espnowScenarioPrevSpeed = step.speed;
        espnowScenarioStep++;
        if (espnowScenarioStep >= espnowScenarioStepCount)
        {
            espnowScenarioActive = false;
            return;
        }
        espnowScenarioStepStarted = now;
        elapsed = 0;
    }

    const EspNowScenarioStep &curStep = espnowDefaultScenario[espnowScenarioStep];
    uint16_t speed;
    if (curStep.durationMs == 0)
        speed = curStep.speed;
    else
    {
        unsigned long t = (elapsed < curStep.durationMs) ? elapsed : curStep.durationMs;
        speed = espnowScenarioPrevSpeed
            + (static_cast<int>(curStep.speed) - static_cast<int>(espnowScenarioPrevSpeed))
              * t / curStep.durationMs;
    }

    espnowCurData.gear = curStep.gear;
    espnowCurData.accelPedalPos = curStep.accel;
    espnowCurData.vehicleSpeed = speed;
    espnowCurData.driverBrakeApply = curStep.brakeApply;
}
