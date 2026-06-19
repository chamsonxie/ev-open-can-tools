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
// ESP-NOW CAN Data Packet — signals from 6 selected CAN IDs
// ═══════════════════════════════════════════════════════════════
// Total size: 41 bytes (well within ESP-NOW 250-byte limit)
//
// Field layout:
//   Offset  Size  Description
//   ──────  ────  ───────────
//   0       1     type (0x01 = CAN_DATA)
//   1       4     seq (monotonic counter)
//   5       1     gear (0=INV,1=P,2=R,3=N,4=D,7=SNA)     [0x118 DI_systemStatus]
//   6       1     accelPedalPos (raw*0.4=%)               [0x118 DI_systemStatus]
//   7       1     regenLight (0=off, 1=on)                [0x118 DI_systemStatus]
//   8       1     brakePedalState (0=release,1=pressed)   [0x118 DI_systemStatus]
//   9       2     steeringAngle (raw*0.1-819.2=deg)       [0x129 SCCM_steeringAngleSensor]
//   11      2     steeringAngleSpeed (raw*0.5-4096=deg/s) [0x129 SCCM_steeringAngleSensor]
//   13      2     vehicleSpeed (raw*0.08-40=km/h)         [0x257 DI_speed]
//   15      1     uiSpeed (raw*1=display speed)           [0x257 DI_speed]
//   16      2     brakeRodTravel (scaled -5~47mm)          [0x39D IBST_status]
//   18      1     driverBrakeApply (0=uninit,1=no,2=yes)  [0x39D IBST_status]
//   19      4     0x389 signals (longCollisionWarning, lssState, driverInteractionLevel, accSpeedLimit)
//   23      1     lowBeamLeftStatus                       [0x3F5 VCFRONT_lighting]
//   24      1     lowBeamRightStatus                      [0x3F5 VCFRONT_lighting]
//   25      1     highBeamLeftStatus                      [0x3F5 VCFRONT_lighting]
//   26      1     highBeamRightStatus                     [0x3F5 VCFRONT_lighting]
//   27      1     drlLeftStatus                           [0x3F5 VCFRONT_lighting]
//   28      1     drlRightStatus                          [0x3F5 VCFRONT_lighting]
//   29      1     turnSignalLeftStatus                    [0x3F5 VCFRONT_lighting]
//   30      1     turnSignalRightStatus                   [0x3F5 VCFRONT_lighting]
//   31      1     parkLeftStatus                          [0x3F5 VCFRONT_lighting]
//   32      1     parkRightStatus                         [0x3F5 VCFRONT_lighting]
//   33      1     fogLeftStatus                           [0x3F5 VCFRONT_lighting]
//   34      1     fogRightStatus                          [0x3F5 VCFRONT_lighting]
//   35      1     sideRepeaterLeftStatus                  [0x3F5 VCFRONT_lighting]
//   36      1     sideRepeaterRightStatus                 [0x3F5 VCFRONT_lighting]
//   37      1     hazardLightRequest                      [0x3F5 VCFRONT_lighting]
//   38      1     indicatorLeftRequest                    [0x3F5 VCFRONT_lighting]
//   39      1     indicatorRightRequest                   [0x3F5 VCFRONT_lighting]
//   40      1     chksum (XOR of bytes 0..39)
//   ──────  ────
//   41      bytes total
// ═══════════════════════════════════════════════════════════════
struct __attribute__((packed)) EspNowCanDataPkt
{
    uint8_t type;       // 0 = ESP_NOW_PKT_CAN_DATA
    uint32_t seq;       // sequence counter

    // 0x118 DI_systemStatus
    uint8_t gear;               // 0=INV,1=P,2=R,3=N,4=D,7=SNA
    uint8_t accelPedalPos;      // raw*0.4=%
    uint8_t regenLight;         // 0=off,1=on
    uint8_t brakePedalState;    // 0=release,1=pressed,2=invalid

    // 0x129 SCCM_steeringAngleSensor
    int16_t steeringAngle;       // raw*0.1-819.2=deg
    int16_t steeringAngleSpeed;  // raw*0.5-4096=deg/s

    // 0x257 DI_speed
    uint16_t vehicleSpeed;       // raw*0.08-40=km/h, 4095=SNA
    uint8_t uiSpeed;             // raw*1=display speed, 255=SNA

    // 0x39D IBST_status
    int16_t brakeRodTravel;      // raw*0.015625-5.0=mm
    uint8_t driverBrakeApply;    // 0=uninit,1=no,2=yes,3=fault

    // 0x389 DAS_status2
    uint8_t longCollisionWarning;      // 4 bits (stored as is)
    uint8_t lssState;                  // 3 bits (stored as is)
    uint8_t driverInteractionLevel;    // 2 bits (stored as is)
    uint8_t accSpeedLimit;             // 0-255 (raw*0.4=mph, DBC uses 10 bits but 8 is sufficient)

    // 0x3F5 VCFRONT_lighting
    uint8_t lowBeamLeftStatus;      // 0=off,1=on,2=fault,3=SNA
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

    uint8_t chksum;
};

static_assert(sizeof(EspNowCanDataPkt) == 41, "EspNowCanDataPkt size mismatch");

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

    // 0x118 DI_systemStatus: gear, accelPedalPos, regenLight, brakePedalState
    // DBC: gear=21|3@1+, accelPedalPos=32|8@1+, regenLight=26|1@1+, brakePedalState=19|2@1+
    if (frame.id == 280 || frame.id == 0x118)
    {
        if (frame.dlc < 5) return;
        espnowCurData.gear            = (frame.data[2] >> 5) & 0x07;
        espnowCurData.accelPedalPos   = frame.data[4];
        espnowCurData.regenLight      = (frame.data[3] >> 2) & 0x01;
        espnowCurData.brakePedalState = (frame.data[2] >> 3) & 0x03;
        return;
    }

    // 0x129 SCCM_steeringAngleSensor: steeringAngle, steeringAngleSpeed
    // DBC: steeringAngle=16|14@1+, steeringAngleSpeed=32|14@1+
    if (frame.id == 297 || frame.id == 0x129)
    {
        if (frame.dlc < 6) return;
        espnowCurData.steeringAngle      = (int16_t)extractIntel(frame.data, 16, 14);
        espnowCurData.steeringAngleSpeed = (int16_t)extractIntel(frame.data, 32, 14);
        return;
    }

    // 0x257 DI_speed: vehicleSpeed, uiSpeed
    // DBC: vehicleSpeed=12|12@1+ (0.08,-40), uiSpeed=24|8@1+
    if (frame.id == 599 || frame.id == 0x257)
    {
        if (frame.dlc < 5) return;
        espnowCurData.vehicleSpeed = (uint16_t)extractIntel(frame.data, 12, 12);
        espnowCurData.uiSpeed      = frame.data[3];
        return;
    }

    // 0x389 DAS_status2: longCollisionWarning, lssState, driverInteractionLevel, accSpeedLimit
    // DBC: longCollisionWarning=48|4@1+, lssState=31|3@1+, driverInteractionLevel=38|2@1+, accSpeedLimit=0|10@1+
    if (frame.id == 905 || frame.id == 0x389)
    {
        if (frame.dlc < 7) return;
        espnowCurData.longCollisionWarning   = (uint8_t)extractIntel(frame.data, 48, 4);
        espnowCurData.lssState               = (uint8_t)extractIntel(frame.data, 31, 3);
        espnowCurData.driverInteractionLevel = (uint8_t)extractIntel(frame.data, 38, 2);
        uint64_t raw = extractIntel(frame.data, 0, 10);
        uint16_t accLimit = raw * 0.4f + 0.5f;
        espnowCurData.accSpeedLimit          = accLimit > 255 ? 255 : (uint8_t)accLimit;
        return;
    }

    // 0x39D IBST_status: driverBrakeApply, brakeRodTravel
    // DBC: driverBrakeApply=16|2@1+, brakeRodTravel=21|12@1+
    if (frame.id == 925 || frame.id == 0x39D)
    {
        if (frame.dlc < 3) return;
        espnowCurData.driverBrakeApply = (uint8_t)extractIntel(frame.data, 16, 2);
        espnowCurData.brakeRodTravel   = (int16_t)extractIntel(frame.data, 21, 12);
        return;
    }

    // 0x3F5 VCFRONT_lighting
    // DBC: indicatorLeftRequest=0|2@1+, indicatorRightRequest=2|2@1+, hazardLightRequest=4|4@1+
    //      lowBeamLeftStatus=28|2@1+, lowBeamRightStatus=30|2@1+, highBeamLeftStatus=32|2@1+
    //      highBeamRightStatus=34|2@1+, drlLeftStatus=36|2@1+, drlRightStatus=38|2@1+
    //      turnSignalLeftStatus=50|2@1+, turnSignalRightStatus=52|2@1+
    //      parkLeftStatus=54|2@1+, parkRightStatus=56|2@1+
    //      fogLeftStatus=40|2@1+, fogRightStatus=42|2@1+
    //      sideRepeaterLeftStatus=46|2@1+, sideRepeaterRightStatus=48|2@1+
    if (frame.id == 1013 || frame.id == 0x3F5)
    {
        if (frame.dlc < 8) return;
        espnowCurData.lowBeamLeftStatus      = (frame.data[3] >> 4) & 0x03;
        espnowCurData.lowBeamRightStatus     = (frame.data[3] >> 6) & 0x03;
        espnowCurData.highBeamLeftStatus     = frame.data[4] & 0x03;
        espnowCurData.highBeamRightStatus    = (frame.data[4] >> 2) & 0x03;
        espnowCurData.drlLeftStatus          = (frame.data[4] >> 4) & 0x03;
        espnowCurData.drlRightStatus         = (frame.data[4] >> 6) & 0x03;
        espnowCurData.fogLeftStatus          = frame.data[5] & 0x03;
        espnowCurData.fogRightStatus         = (frame.data[5] >> 2) & 0x03;
        espnowCurData.sideRepeaterLeftStatus = (frame.data[5] >> 6) & 0x03;
        espnowCurData.sideRepeaterRightStatus= frame.data[6] & 0x03;
        espnowCurData.turnSignalLeftStatus   = (frame.data[6] >> 2) & 0x03;
        espnowCurData.turnSignalRightStatus  = (frame.data[6] >> 4) & 0x03;
        espnowCurData.parkLeftStatus         = (frame.data[6] >> 6) & 0x03;
        espnowCurData.parkRightStatus        = frame.data[7] & 0x03;
        espnowCurData.hazardLightRequest     = (frame.data[0] >> 4) & 0x0F;
        espnowCurData.indicatorLeftRequest   = frame.data[0] & 0x03;
        espnowCurData.indicatorRightRequest  = (frame.data[0] >> 2) & 0x03;
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
    espnowCurData.gear = (i == 0) ? 1 : 4;
    espnowCurData.accelPedalPos = testAccels[i];
    espnowCurData.vehicleSpeed = testSpeeds[i];
    espnowCurData.driverBrakeApply = (i == 3) ? 2 : 0;

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
    uint8_t gear;
    uint8_t accel;
    uint16_t speed;
    uint8_t brakeApply;
};

static const EspNowScenarioStep espnowDefaultScenario[] = {
    {2000, 1, 0,   0,   0},   // Parked
    {3000, 4, 40,  400, 0},   // Drive, accelerate
    {2000, 4, 25,  500, 0},   // Cruise
    {2500, 4, 0,   300, 2},   // Braking
    {2000, 4, 20,  350, 0},   // Release brake, cruise
    {1000, 1, 0,   0,   0},   // Stop, park
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

    espnowCurData.gear = curStep.gear;
    espnowCurData.accelPedalPos = curStep.accel;
    espnowCurData.vehicleSpeed = speed;
    espnowCurData.driverBrakeApply = curStep.brakeApply;
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
