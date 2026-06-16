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

struct __attribute__((packed)) EspNowCanDataPkt
{
    uint8_t type;
    uint32_t seq;
    uint16_t speed;
    uint8_t throttle;
    uint8_t brake;
    uint8_t turnLeft;
    uint8_t turnRight;
    uint8_t chksum;
};

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

static uint16_t espnowCanSpeed = 0;
static uint8_t espnowCanThrottle = 0;
static uint8_t espnowCanBrake = 0;
static uint8_t espnowCanTurnLeft = 0;
static uint8_t espnowCanTurnRight = 0;

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

static void espnowSetCanData(uint16_t speed, uint8_t throttle, uint8_t brake,
                              uint8_t turnLeft, uint8_t turnRight)
{
    espnowCanSpeed = speed;
    espnowCanThrottle = throttle;
    espnowCanBrake = brake;
    espnowCanTurnLeft = turnLeft;
    espnowCanTurnRight = turnRight;
}

static void espnowBroadcastCanData()
{
    if (!espnowInitialized || !espnowCanBroadcastEnabled)
        return;
    unsigned long now = millis();
    if (now - espnowLastBroadcastMs < ESPNOW_CAN_BROADCAST_MS)
        return;
    espnowLastBroadcastMs = now;

    EspNowCanDataPkt pkt;
    memset(&pkt, 0, sizeof(pkt));
    pkt.type = ESPNOW_PKT_CAN_DATA;
    pkt.seq = espnowSeq++;
    pkt.speed = espnowCanSpeed;
    pkt.throttle = espnowCanThrottle;
    pkt.brake = espnowCanBrake;
    pkt.turnLeft = espnowCanTurnLeft;
    pkt.turnRight = espnowCanTurnRight;

    const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&pkt);
    pkt.chksum = 0;
    for (size_t i = 0; i < sizeof(pkt) - 1; i++)
        pkt.chksum ^= bytes[i];

    esp_now_send(ESPNOW_BROADCAST_MAC, bytes, sizeof(pkt));
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
