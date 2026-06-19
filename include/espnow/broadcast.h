#pragma once

#include "protocol.h"
#include "signals.h"
#include "scenario.h"
#include "discovery.h"
#include <esp_now.h>

// ── Global state definitions ──

EspNowCanDataPkt espnowCurData = {};

EspNowDiscoveredDev espnowDiscovered[ESPNOW_MAX_DISCOVERED] = {};
int espnowDiscoveredCount = 0;
uint8_t espnowPairedMac[6] = {0};
bool espnowHasPaired = false;
bool espnowInitialized = false;
static bool espnowScanning = false;
bool espnowCanBroadcastEnabled = true;
static uint32_t espnowSeq = 0;
static unsigned long espnowLastBroadcastMs = 0;

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

    esp_err_t sendRet = esp_now_send(ESPNOW_BROADCAST_MAC,
                                     reinterpret_cast<const uint8_t *>(&pkt), sizeof(pkt));
    if (sendRet != ESP_OK)
        Serial.printf("[ESPNOW] send failed: %d\n", sendRet);
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
