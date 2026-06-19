#pragma once

#include "protocol.h"
#include "signals.h"
#include "../shared_types.h"
#include <esp_now.h>
#include <esp_wifi.h>

extern EspNowDiscoveredDev espnowDiscovered[ESPNOW_MAX_DISCOVERED];
extern int espnowDiscoveredCount;
extern uint8_t espnowPairedMac[6];
extern bool espnowHasPaired;
extern bool espnowInitialized;
extern bool espnowCanBroadcastEnabled;

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

static void espnowSendCb(const esp_now_send_info_t *, esp_now_send_status_t)
{
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
