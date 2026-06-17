#pragma once

#include <cstdint>
#include <cstring>
#include <cstdio>
#include "esp_now.h"
#include "esp_wifi.h"

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
// ESP-NOW CAN Data Packet — contains all signals from my-dbc-sign.csv
// ═══════════════════════════════════════════════════════════════
// Total size: 53 bytes (well within ESP-NOW 250-byte limit)
//
// Field layout:
//   Offset  Size  Description
//   ──────  ────  ───────────
//   0       1     type (0x01 = CAN_DATA)
//   1       4     seq (monotonic counter)
//   5       2     vehicleSpeed (raw*0.5=km/h, 1023=SNA)  [0x155 ESP_B]
//   7       1     standstill (0=no, 1=yes)                [0x155 ESP_B]
//   8       1     gear (0=INV,1=P,2=R,3=N,4=D,7=SNA)     [0x118 DI_systemStatus]
//   9       1     accelPedalPos (raw*0.4=%)               [0x118 DI_systemStatus]
//   10      1     regenLight (0=off, 1=on)                [0x118 DI_systemStatus]
//   11      2     steeringAngle (raw*0.1-819.2=deg)       [0x129 SCCM_steeringAngleSensor]
//   13      2     steeringAngleSpeed (raw*0.5-4096=deg/s) [0x129 SCCM_steeringAngleSensor]
//   15      1     buckleStatus (0=unbuckled, 1=buckled)   [0x311 UI_warning]
//   16      1     leftBlinkerBlinking (0/1/2)             [0x311 UI_warning]
//   17      1     rightBlinkerBlinking (0/1/2)            [0x311 UI_warning]
//   18      1     anyDoorOpen (0=closed, 1=open)          [0x311 UI_warning]
//   19      1     uiHighBeam (0=off, 1=on)                [0x311 UI_warning]
//   20      1     laneDepartureWarning (0=none,1=left..)   [0x39B DAS_status]
//   21      1     sideCollisionWarning (0=none,1=left..)   [0x39B DAS_status]
//   22      1     forwardCollisionWarning (0=none,1=warn)  [0x39B DAS_status]
//   23      1     suppressSpeedWarning (0=no, 1=yes)      [0x39B DAS_status]
//   24      1     blindSpotRearRight (0=none,1=1级,2=2级)  [0x39B DAS_status]
//   25      1     blindSpotRearLeft  (0=none,1=1级,2=2级)  [0x39B DAS_status]
//   26      2     brakeRodTravel (scaled -5~47mm)          [0x39D IBST_status]
//   28      1     driverBrakeApply (0=uninit,1=no,2=yes)   [0x39D IBST_status]
//   29      1     highBeamSwitchActive                     [0x3F5 VCFRONT_lighting]
//   30      1     parkRightStatus                          [0x3F5 VCFRONT_lighting]
//   31      1     parkLeftStatus                           [0x3F5 VCFRONT_lighting]
//   32      1     turnSignalRightStatus                    [0x3F5 VCFRONT_lighting]
//   33      1     turnSignalLeftStatus                     [0x3F5 VCFRONT_lighting]
//   34      1     sideRepeaterRightStatus                  [0x3F5 VCFRONT_lighting]
//   35      1     sideRepeaterLeftStatus                   [0x3F5 VCFRONT_lighting]
//   36      1     fogRightStatus                           [0x3F5 VCFRONT_lighting]
//   37      1     fogLeftStatus                            [0x3F5 VCFRONT_lighting]
//   38      1     drlRightStatus                           [0x3F5 VCFRONT_lighting]
//   39      1     drlLeftStatus                            [0x3F5 VCFRONT_lighting]
//   40      1     highBeamRightStatus                      [0x3F5 VCFRONT_lighting]
//   41      1     highBeamLeftStatus                       [0x3F5 VCFRONT_lighting]
//   42      1     lowBeamRightStatus                       [0x3F5 VCFRONT_lighting]
//   43      1     lowBeamLeftStatus                        [0x3F5 VCFRONT_lighting]
//   44      1     hazardSwitchBacklight                    [0x3F5 VCFRONT_lighting]
//   45      1     seeYouHomeLightingReq                    [0x3F5 VCFRONT_lighting]
//   46      1     approachLightingRequest                  [0x3F5 VCFRONT_lighting]
//   47      1     switchLightingBrightness                 [0x3F5 VCFRONT_lighting]
//   48      1     ambientLightingBrightness                [0x3F5 VCFRONT_lighting]
//   49      1     hazardLightRequest                       [0x3F5 VCFRONT_lighting]
//   50      1     indicatorRightRequest                    [0x3F5 VCFRONT_lighting]
//   51      1     indicatorLeftRequest                     [0x3F5 VCFRONT_lighting]
//   52      1     chksum (XOR of bytes 0..51)
//   ──────  ────
//   53      bytes total
// ═══════════════════════════════════════════════════════════════
struct __attribute__((packed)) EspNowCanDataPkt
{
    uint8_t type;       // 0 = ESP_NOW_PKT_CAN_DATA
    uint32_t seq;       // sequence counter

    // 0x155 ESP_B
    uint16_t vehicleSpeed;      // raw*0.5=km/h, 1023=SNA
    uint8_t standstill;         // 0=not, 1=standstill

    // 0x118 DI_systemStatus
    uint8_t gear;               // 0=INV,1=P,2=R,3=N,4=D,7=SNA
    uint8_t accelPedalPos;      // raw*0.4=%
    uint8_t regenLight;         // 0=off,1=on

    // 0x129 SCCM_steeringAngleSensor
    int16_t steeringAngle;       // raw*0.1-819.2=deg
    int16_t steeringAngleSpeed;  // raw*0.5-4096=deg/s

    // 0x311 UI_warning
    uint8_t buckleStatus;
    uint8_t leftBlinkerBlinking;
    uint8_t rightBlinkerBlinking;
    uint8_t anyDoorOpen;
    uint8_t uiHighBeam;

    // 0x39B DAS_status
    uint8_t laneDepartureWarning;
    uint8_t sideCollisionWarning;
    uint8_t forwardCollisionWarning;
    uint8_t suppressSpeedWarning;
    uint8_t blindSpotRearRight;
    uint8_t blindSpotRearLeft;

    // 0x39D IBST_status
    int16_t brakeRodTravel;
    uint8_t driverBrakeApply;

    // 0x3F5 VCFRONT_lighting
    uint8_t highBeamSwitchActive;
    uint8_t parkRightStatus;
    uint8_t parkLeftStatus;
    uint8_t turnSignalRightStatus;
    uint8_t turnSignalLeftStatus;
    uint8_t sideRepeaterRightStatus;
    uint8_t sideRepeaterLeftStatus;
    uint8_t fogRightStatus;
    uint8_t fogLeftStatus;
    uint8_t drlRightStatus;
    uint8_t drlLeftStatus;
    uint8_t highBeamRightStatus;
    uint8_t highBeamLeftStatus;
    uint8_t lowBeamRightStatus;
    uint8_t lowBeamLeftStatus;
    uint8_t hazardSwitchBacklight;
    uint8_t seeYouHomeLightingReq;
    uint8_t approachLightingRequest;
    uint8_t switchLightingBrightness;
    uint8_t ambientLightingBrightness;
    uint8_t hazardLightRequest;
    uint8_t indicatorRightRequest;
    uint8_t indicatorLeftRequest;

    uint8_t chksum;
};

static_assert(sizeof(EspNowCanDataPkt) == 53, "EspNowCanDataPkt size mismatch");

struct EspNowDiscoveredDev
{
    uint8_t mac[6];
    int8_t rssi;
    unsigned long lastSeenMs;
    bool paired;
};

static bool espnowInitialized = false;
static bool espnowScanning = false;
static bool espnowCanBroadcastEnabled = true;
static uint32_t espnowSeq = 0;
static unsigned long espnowLastBroadcastMs = 0;
static EspNowDiscoveredDev espnowDiscovered[ESPNOW_MAX_DISCOVERED] = {};
static int espnowDiscoveredCount = 0;
static uint8_t espnowPairedMac[6] = {0};
static bool espnowHasPaired = false;

// Current signal state for broadcast
static EspNowCanDataPkt espnowCurData = {};

static void espnowMacToStr(const uint8_t *mac, char *out)
{
    sprintf(out, "%02X:%02X:%02X:%02X:%02X:%02X",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

static bool espnowMacEq(const uint8_t *a, const uint8_t *b)
{
    return memcmp(a, b, 6) == 0;
}

static int espnowAddDiscovered(const uint8_t *mac, int8_t rssi)
{
    for (int i = 0; i < espnowDiscoveredCount; i++)
    {
        if (espnowMacEq(espnowDiscovered[i].mac, mac))
        {
            espnowDiscovered[i].lastSeenMs = millis();
            espnowDiscovered[i].rssi = rssi;
            return i;
        }
    }
    if (espnowDiscoveredCount < ESPNOW_MAX_DISCOVERED)
    {
        int idx = espnowDiscoveredCount++;
        memcpy(espnowDiscovered[idx].mac, mac, 6);
        espnowDiscovered[idx].rssi = rssi;
        espnowDiscovered[idx].lastSeenMs = millis();
        espnowDiscovered[idx].paired = false;
        return idx;
    }
    unsigned long oldest = espnowDiscovered[0].lastSeenMs;
    int oldestIdx = 0;
    for (int i = 1; i < ESPNOW_MAX_DISCOVERED; i++)
    {
        if (!espnowDiscovered[i].paired && espnowDiscovered[i].lastSeenMs < oldest)
        {
            oldest = espnowDiscovered[i].lastSeenMs;
            oldestIdx = i;
        }
    }
    memcpy(espnowDiscovered[oldestIdx].mac, mac, 6);
    espnowDiscovered[oldestIdx].rssi = rssi;
    espnowDiscovered[oldestIdx].lastSeenMs = millis();
    return oldestIdx;
}

static void espnowRecvCb(const esp_now_recv_info_t *recvInfo, const uint8_t *data, int len)
{
    if (!data || len < 1)
        return;
    uint8_t pktType = data[0];
    if (pktType == ESPNOW_PKT_DISCOVERY && len >= 11)
    {
        if (memcmp(data + 1, ESPNOW_PASSPHRASE, ESPNOW_PASSPHRASE_LEN) == 0)
        {
            espnowAddDiscovered(recvInfo->src_addr, recvInfo->rx_ctrl->rssi);
        }
    }
}

static void espnowSendCb(const esp_now_send_info_t *tx_info, esp_now_send_status_t status)
{
    (void)tx_info;
    (void)status;
}

static bool espnowInit()
{
    if (espnowInitialized)
        return true;
    esp_err_t ret = esp_now_init();
    if (ret != ESP_OK)
    {
        Serial.printf("[ESPNOW] Init failed: %d\n", ret);
        return false;
    }
    esp_now_register_recv_cb(espnowRecvCb);
    esp_now_register_send_cb(espnowSendCb);
    esp_now_peer_info_t peer = {};
    peer.channel = ESPNOW_CHANNEL;
    peer.ifidx = WIFI_IF_AP;
    peer.encrypt = false;
    memcpy(peer.peer_addr, ESPNOW_BROADCAST_MAC, 6);
    esp_now_add_peer(&peer);
    espnowInitialized = true;
    Serial.println("[ESPNOW] Initialized");
    return true;
}

// Update the ESP-NOW signal state from any CAN frame
static void espnowUpdateFromFrame(const CanFrame &frame)
{
    if (frame.dlc < 4) return;

    // 0x155 ESP_B: speed + standstill
    if (frame.id == 341 || frame.id == 0x155)
    {
        if (frame.dlc < 6) return;
        espnowCurData.vehicleSpeed = (uint16_t)extractIntel(frame.data, 42, 10);
        espnowCurData.standstill = (uint8_t)extractIntel(frame.data, 41, 1);
        return;
    }

    // 0x118 DI_systemStatus: gear, accel, regen
    if (frame.id == 280 || frame.id == 0x118)
    {
        if (frame.dlc < 5) return;
        espnowCurData.gear = (frame.data[2] >> 5) & 0x07;
        espnowCurData.accelPedalPos = frame.data[4];
        espnowCurData.regenLight = (frame.data[3] >> 2) & 0x01;
        return;
    }

    // 0x129 SCCM_steeringAngleSensor
    if (frame.id == 297 || frame.id == 0x129)
    {
        if (frame.dlc < 6) return;
        espnowCurData.steeringAngle = (int16_t)extractIntel(frame.data, 16, 14);
        espnowCurData.steeringAngleSpeed = (int16_t)extractIntel(frame.data, 32, 14);
        return;
    }

    // 0x311 UI_warning
    if (frame.id == 785 || frame.id == 0x311)
    {
        if (frame.dlc < 7) return;
        espnowCurData.buckleStatus = (uint8_t)extractMotorola(frame.data, 13, 1);
        espnowCurData.leftBlinkerBlinking = (uint8_t)extractMotorola(frame.data, 25, 2);
        espnowCurData.rightBlinkerBlinking = (uint8_t)extractIntel(frame.data, 26, 2);
        espnowCurData.anyDoorOpen = (uint8_t)extractMotorola(frame.data, 28, 1);
        espnowCurData.uiHighBeam = (uint8_t)extractMotorola(frame.data, 50, 1);
        return;
    }

    // 0x39B DAS_status
    if (frame.id == 923 || frame.id == 0x39B)
    {
        if (frame.dlc < 5) return;
        espnowCurData.blindSpotRearLeft = (uint8_t)extractIntel(frame.data, 4, 2);
        espnowCurData.blindSpotRearRight = (uint8_t)extractIntel(frame.data, 6, 2);
        espnowCurData.suppressSpeedWarning = (uint8_t)extractIntel(frame.data, 13, 1);
        espnowCurData.forwardCollisionWarning = (uint8_t)extractIntel(frame.data, 22, 2);
        espnowCurData.sideCollisionWarning = (uint8_t)extractIntel(frame.data, 32, 2);
        espnowCurData.laneDepartureWarning = (uint8_t)extractIntel(frame.data, 37, 3);
        return;
    }

    // 0x39D IBST_status
    if (frame.id == 925 || frame.id == 0x39D)
    {
        if (frame.dlc < 3) return;
        espnowCurData.driverBrakeApply = (uint8_t)extractIntel(frame.data, 16, 2);
        espnowCurData.brakeRodTravel = (int16_t)extractIntel(frame.data, 21, 12);
        return;
    }

    // 0x3F5 VCFRONT_lighting
    if (frame.id == 1013 || frame.id == 0x3F5)
    {
        if (frame.dlc < 8) return;
        espnowCurData.indicatorLeftRequest = frame.data[0] & 0x03;
        espnowCurData.indicatorRightRequest = (frame.data[0] >> 2) & 0x03;
        espnowCurData.hazardLightRequest = (frame.data[0] >> 4) & 0x0F;
        espnowCurData.ambientLightingBrightness = frame.data[1];
        espnowCurData.switchLightingBrightness = frame.data[2];
        espnowCurData.approachLightingRequest = (frame.data[3] >> 1) & 0x01;
        espnowCurData.seeYouHomeLightingReq = (frame.data[3] >> 2) & 0x01;
        espnowCurData.hazardSwitchBacklight = (frame.data[3] >> 3) & 0x01;
        espnowCurData.lowBeamLeftStatus = (frame.data[3] >> 4) & 0x03;
        espnowCurData.lowBeamRightStatus = (frame.data[3] >> 6) & 0x03;
        espnowCurData.highBeamLeftStatus = frame.data[4] & 0x03;
        espnowCurData.highBeamRightStatus = (frame.data[4] >> 2) & 0x03;
        espnowCurData.drlLeftStatus = (frame.data[4] >> 4) & 0x03;
        espnowCurData.drlRightStatus = (frame.data[4] >> 6) & 0x03;
        espnowCurData.fogLeftStatus = frame.data[5] & 0x03;
        espnowCurData.fogRightStatus = (frame.data[5] >> 2) & 0x03;
        espnowCurData.sideRepeaterLeftStatus = (frame.data[5] >> 6) & 0x03;
        espnowCurData.sideRepeaterRightStatus = frame.data[6] & 0x03;
        espnowCurData.turnSignalLeftStatus = (frame.data[6] >> 2) & 0x03;
        espnowCurData.turnSignalRightStatus = (frame.data[6] >> 4) & 0x03;
        espnowCurData.parkLeftStatus = (frame.data[6] >> 6) & 0x03;
        espnowCurData.parkRightStatus = frame.data[7] & 0x03;
        espnowCurData.highBeamSwitchActive = (frame.data[7] >> 2) & 0x01;
        return;
    }
}

static void espnowTickScenario();

static void espnowBroadcastCanData()
{
    if (!espnowInitialized || !espnowCanBroadcastEnabled)
        return;
    unsigned long now = millis();
    if (now - espnowLastBroadcastMs < ESPNOW_CAN_BROADCAST_MS)
        return;
    espnowLastBroadcastMs = now;

    espnowTickScenario();

    EspNowCanDataPkt pkt;
    memcpy(&pkt, &espnowCurData, sizeof(pkt));
    pkt.type = ESPNOW_PKT_CAN_DATA;
    pkt.seq = espnowSeq++;
    pkt.chksum = 0;

    const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&pkt);
    for (size_t i = 0; i < sizeof(pkt) - 1; i++)
        pkt.chksum ^= bytes[i];

    esp_now_send(ESPNOW_BROADCAST_MAC, reinterpret_cast<const uint8_t *>(&pkt), sizeof(pkt));
}

static void espnowSendTestPackets()
{
    if (!espnowInitialized)
        return;
    EspNowCanDataPkt saved = espnowCurData;
    espnowCurData = {};
    espnowCurData.type = ESPNOW_PKT_CAN_DATA;

    static const uint16_t testSpeeds[] = {200, 400, 800, 1200, 0};
    static const uint8_t testAccels[] = {15, 30, 50, 70, 0};
    for (int i = 0; i < 5; i++)
    {
        espnowCurData.vehicleSpeed = testSpeeds[i];
        espnowCurData.accelPedalPos = testAccels[i];
        espnowCurData.gear = (i == 0) ? 1 : 4;
        espnowCurData.leftBlinkerBlinking = (i % 3 == 1) ? 1 : 0;
        espnowCurData.rightBlinkerBlinking = (i % 3 == 2) ? 1 : 0;

        EspNowCanDataPkt pkt = espnowCurData;
        pkt.seq = espnowSeq++;
        pkt.chksum = 0;
        const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&pkt);
        for (size_t j = 0; j < sizeof(pkt) - 1; j++)
            pkt.chksum ^= bytes[j];
        esp_now_send(ESPNOW_BROADCAST_MAC, bytes, sizeof(pkt));
    }
    espnowCurData = saved;
}

// ── Scenario simulation ──
struct EspNowScenarioStep
{
    unsigned long durationMs;
    uint16_t speed;
    uint8_t accel;
    uint8_t gear;
    uint8_t brake;
    uint8_t turnLeft;
    uint8_t turnRight;
    uint8_t doorOpen;
};

static const EspNowScenarioStep espnowDefaultScenario[] = {
    {2000, 0, 0, 1, 0, 0, 0, 0},      // Step 0: Parked
    {1500, 0, 0, 1, 0, 0, 0, 0},      // Step 1: Still parked
    {3000, 400, 40, 4, 0, 0, 0, 0},   // Step 2: Drive, accelerate
    {2000, 500, 25, 4, 0, 0, 0, 0},   // Step 3: Cruise
    {2500, 350, 20, 4, 0, 1, 0, 0},   // Step 4: Left blinker
    {2000, 150, 10, 4, 0, 0, 1, 0},   // Step 5: Right blinker
    {1000, 0, 0, 1, 0, 0, 0, 0},      // Step 6: Stop
};
static constexpr int espnowScenarioStepCount = sizeof(espnowDefaultScenario) / sizeof(espnowDefaultScenario[0]);

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
        speed = espnowScenarioPrevSpeed + (static_cast<int>(curStep.speed) - static_cast<int>(espnowScenarioPrevSpeed)) * t / curStep.durationMs;
    }

    espnowCurData.vehicleSpeed = speed;
    espnowCurData.accelPedalPos = curStep.accel;
    espnowCurData.gear = curStep.gear;
    espnowCurData.leftBlinkerBlinking = curStep.turnLeft;
    espnowCurData.rightBlinkerBlinking = curStep.turnRight;
    espnowCurData.anyDoorOpen = curStep.doorOpen;
}

static bool espnowPairDevice(const uint8_t *mac)
{
    int idx = -1;
    for (int i = 0; i < espnowDiscoveredCount; i++)
    {
        if (espnowMacEq(espnowDiscovered[i].mac, mac))
        {
            idx = i;
            break;
        }
    }
    if (idx < 0)
        return false;

    if (espnowHasPaired && !espnowMacEq(espnowPairedMac, mac))
    {
        esp_now_del_peer(espnowPairedMac);
        for (int i = 0; i < espnowDiscoveredCount; i++)
        {
            if (espnowMacEq(espnowDiscovered[i].mac, espnowPairedMac))
                espnowDiscovered[i].paired = false;
        }
    }

    esp_now_peer_info_t peer = {};
    peer.channel = ESPNOW_CHANNEL;
    peer.ifidx = WIFI_IF_AP;
    peer.encrypt = false;
    memcpy(peer.peer_addr, mac, 6);
    esp_now_del_peer(mac);
    esp_err_t ret = esp_now_add_peer(&peer);
    if (ret != ESP_OK)
        return false;

    memcpy(espnowPairedMac, mac, 6);
    espnowHasPaired = true;
    espnowDiscovered[idx].paired = true;
    espnowCanBroadcastEnabled = true;

    return true;
}

static void espnowUnpairDevice()
{
    if (!espnowHasPaired)
        return;
    esp_now_del_peer(espnowPairedMac);
    for (int i = 0; i < espnowDiscoveredCount; i++)
    {
        if (espnowMacEq(espnowDiscovered[i].mac, espnowPairedMac))
            espnowDiscovered[i].paired = false;
    }
    memset(espnowPairedMac, 0, 6);
    espnowHasPaired = false;
    espnowCanBroadcastEnabled = true;
}

static String espnowGetDiscoveredMac(int i)
{
    if (i < 0 || i >= espnowDiscoveredCount)
        return "";
    char macStr[18];
    espnowMacToStr(espnowDiscovered[i].mac, macStr);
    return String(macStr);
}

static int espnowGetDiscoveredRssi(int i)
{
    if (i < 0 || i >= espnowDiscoveredCount)
        return 0;
    return espnowDiscovered[i].rssi;
}

static bool espnowGetDiscoveredPaired(int i)
{
    if (i < 0 || i >= espnowDiscoveredCount)
        return false;
    return espnowDiscovered[i].paired;
}
