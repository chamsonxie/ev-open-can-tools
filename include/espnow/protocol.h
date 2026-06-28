#pragma once

#include <cstdint>
#include <cstring>
#include <cstdio>

static constexpr uint8_t ESPNOW_BROADCAST_MAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static constexpr const char *ESPNOW_PASSPHRASE = "chamsonxie";
static constexpr size_t ESPNOW_PASSPHRASE_LEN = 10;
static constexpr int ESPNOW_CHANNEL = 1;
static constexpr unsigned long ESPNOW_CAN_BROADCAST_MS = 100;
static constexpr int ESPNOW_MAX_DISCOVERED = 16;

enum EspNowPktType : uint8_t
{
    ESPNOW_PKT_DISCOVERY = 0x00,
    ESPNOW_PKT_CAN_DATA = 0x01,
};

struct __attribute__((packed)) EspNowDiscoveryPkt
{
    uint8_t type;
    uint8_t passphrase[10];
    uint8_t reserved[5];
};

// ═══════════════════════════════════════════════════════════════
// ESP-NOW CAN Data Packet — signals from 8 selected CAN IDs
// Total size: 49 bytes (well within ESP-NOW 250-byte limit)
// ═══════════════════════════════════════════════════════════════
struct __attribute__((packed)) EspNowCanDataPkt
{
    uint8_t type;
    uint32_t seq;

    // 0x118 DI_systemStatus
    uint8_t gear;
    uint8_t accelPedalPos;
    uint8_t regenLight;
    uint8_t brakePedalState;
    uint8_t systemState;
    uint8_t tractionControlMode;
    uint8_t epbRequest;
    uint8_t trackModeState;
    uint8_t immobilizerState;
    uint8_t proximity;
    uint8_t keepAliveRequest;

    // 0x257 DI_speed
    uint16_t vehicleSpeed;
    uint8_t uiSpeed;
    uint8_t uiSpeedUnits;

    // 0x107 IBST_status
    int16_t brakeRodTravel;
    uint8_t driverBrakeApply;
    uint8_t internalState;
    uint8_t iBoosterStatus;

    // 0x389 DAS_status2
    uint8_t longCollisionWarning;
    uint8_t lssState;
    uint8_t driverInteractionLevel;
    uint8_t accSpeedLimit;

    // 0x3F5 VCFRONT_lighting
    uint8_t lowBeamLeftStatus;
    uint8_t lowBeamRightStatus;
    uint8_t highBeamLeftStatus;
    uint8_t highBeamRightStatus;
    uint8_t drlLeftStatus;
    uint8_t drlRightStatus;
    uint8_t turnSignalLeftStatus;
    uint8_t turnSignalRightStatus;
    uint8_t parkLeftStatus;
    uint8_t parkRightStatus;
    uint8_t fogLeftStatus;
    uint8_t fogRightStatus;
    uint8_t sideRepeaterLeftStatus;
    uint8_t sideRepeaterRightStatus;
    uint8_t hazardLightRequest;
    uint8_t indicatorLeftRequest;
    uint8_t indicatorRightRequest;

    // 0x3E2 brake light
    uint8_t brakeLight;

    // 0x102/0x103 door status — bit0=左前, bit1=左后, bit2=右前, bit3=右后
    uint8_t doorOpenBits;

    // 0x212 VCLEFT_status — byte[2] powerState: 2=off, 0x21=awake
    uint8_t vehicleLocked;

    uint8_t chksum;
};

static_assert(sizeof(EspNowCanDataPkt) == 50, "EspNowCanDataPkt size mismatch");

struct EspNowDiscoveredDev
{
    uint8_t mac[6];
    int8_t rssi;
    unsigned long lastSeenMs;
    bool paired;
};
