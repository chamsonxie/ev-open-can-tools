#pragma once

#if defined(ESP32_DASHBOARD) && !defined(NATIVE_BUILD)

#ifdef ESP_PLATFORM
#include "platform/espidf_runtime.h"
#else
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ArduinoOTA.h>
#include <Update.h>
#endif
#include <esp_task_wdt.h>
#ifndef ESP_PLATFORM
#include <Preferences.h>
#include <SPIFFS.h>
#endif
#include <ArduinoJson.h>
#include "handlers.h"
#include "can_helpers.h"
#if defined(DRIVER_ESP32_EXT_MCP2515)
#include "drivers/esp32_mcp2515_driver.h"
#endif
#include "web/mcp2515_dashboard_ui.h"
#include "can_translate.h"
#include "espnow_broadcast.h"

#ifndef DASH_SSID
#error "Define -DDASH_SSID in build_flags (e.g. -DDASH_SSID=\\\"ADUnlock-1234\\\")"
#endif
#ifndef DASH_PASS
#error "Define -DDASH_PASS in build_flags (min 8 chars)"
#endif
#ifndef DASH_OTA_PASS
#error "Define -DDASH_OTA_PASS in build_flags"
#endif
#ifndef DASH_OTA_USER
#error "Define -DDASH_OTA_USER in build_flags"
#endif

static_assert(sizeof(DASH_SSID) > 1 && sizeof(DASH_SSID) <= 33, "DASH_SSID must be 1-32 bytes");
static_assert(sizeof(DASH_PASS) >= 9 && sizeof(DASH_PASS) <= 65, "DASH_PASS must be 8-64 bytes");

#if defined(DRIVER_TWAI)
#ifndef TWAI_TX_PIN
#define TWAI_TX_PIN GPIO_NUM_5
#endif
#ifndef TWAI_RX_PIN
#define TWAI_RX_PIN GPIO_NUM_4
#endif
#endif

#define PREFS_NS "ADunlock"

static Preferences prefs;

static CarManagerBase *dashHandler = nullptr;
static CanDriver *dashDriver = nullptr;
#if defined(DRIVER_ESP32_EXT_MCP2515)
static MCP2515 *dashMcp = nullptr;
#endif

static unsigned long rxCount = 0;
static unsigned long lastFrameMs = 0;
static unsigned long startMs = 0;
static bool canOnline = false;

static unsigned long fpsFrames = 0;
static unsigned long fpsLastMs = 0;
static float fps = 0.0f;

#if defined(DRIVER_ESP32_EXT_MCP2515)
static uint8_t mcpEflg = 0;
#else
static const uint8_t mcpEflg = 0;
#endif

// WiFi AP
static char apSSID[33] = "";
static char apPass[65] = "";
static bool apHidden = false;
static constexpr size_t kDashMaxSsidLen = 32;
static constexpr size_t kDashMinApPassLen = 8;
static constexpr size_t kDashMaxPassLen = 64;
static constexpr int kDashApChannel = 1;
static constexpr int kDashApMaxConn = 4;

// WiFi STA
static char staSSID[33] = "";
static char staPass[65] = "";
static bool staConnected = false;
static bool staConnectAttemptActive = false;
static bool staStaticIP = false;

// Multi-SSID
static constexpr uint8_t kDashMaxWifiNetworks = 4;
struct DashWifiNetwork
{
    char ssid[33];
    char pass[65];
    bool useStatic;
    char ip[16];
    char gw[16];
    char mask[16];
    char dns[16];
};
static DashWifiNetwork wifiNetworks[kDashMaxWifiNetworks] = {};
static uint8_t wifiNetworkCount = 0;
static int8_t wifiActiveSlot = -1;
static int8_t wifiNextRotateSlot = 0;
static bool updateBetaChannel = false;
static bool autoUpdateEnabled = false;
static bool autoUpdateDone = false;
static unsigned long autoUpdateEligibleAt = 0;
static unsigned long staConnectStartedAt = 0;
static unsigned long staRetryAt = 0;
static constexpr unsigned long kDashStaBootDelayMs = 5000;
static constexpr unsigned long kDashStaConnectTimeoutMs = 25000;
static constexpr unsigned long kDashStaRetryMs = 120000;
static IPAddress staIP(0, 0, 0, 0);
static IPAddress staGW(0, 0, 0, 0);
static IPAddress staMask(255, 255, 255, 0);
static IPAddress staDNS(0, 0, 0, 0);

static String dashWifiKey(uint8_t slot, const char *sub)
{
    String k = "w";
    k += slot;
    k += sub;
    return k;
}
static void dashClearWifiNetwork(DashWifiNetwork &n)
{
    n.ssid[0] = 0;
    n.pass[0] = 0;
    n.useStatic = false;
    n.ip[0] = 0;
    n.gw[0] = 0;
    n.mask[0] = 0;
    n.dns[0] = 0;
}
static void dashRotateAndConnect();
static void dashApplyFilters();

// CAN recorder
#ifndef REC_CAP
#define REC_CAP 2000
#endif
struct RecFrame
{
    unsigned long ts;
    uint32_t id;
    uint8_t dlc;
    uint8_t data[8];
};
static RecFrame recBuf[REC_CAP];
static volatile bool recActive = false;
static volatile int recCount = 0;
static bool recSaved = false;

// CAN sniffer ring buffer
#define SNIFFER_CAP 30
struct SniffFrame
{
    unsigned long ts;
    uint32_t id;
    uint8_t dlc;
    uint8_t data[8];
};
static SniffFrame sniffBuf[SNIFFER_CAP];
static int sniffHead = 0;
static int sniffCount = 0;

static const char *decodeCanId(uint32_t id)
{
    switch (id)
    {
    case 0x118:
        return "DI_systemStatus (档位/油门/动能回收)";
    case 0x155:
        return "ESP_B (车速/静止状态)";
    case 0x129:
        return "SCCM_steeringAngleSensor (方向盘转角)";
    case 0x311:
        return "UI_warning (安全带/转向灯/车门/远光)";
    case 0x39B:
        return "DAS_status (盲区/碰撞预警/车道偏离)";
    case 0x39D:
        return "IBST_status (制动踏板行程)";
    case 0x3F5:
        return "VCFRONT_lighting (前部灯光)";
    default:
        return "";
    }
}

// Sniff whitelist: only the 7 selected CAN IDs
static constexpr uint32_t kDashboardSniffIds[] = {
    0x118, // DI_systemStatus: 档位/油门/动能回收
    0x155, // ESP_B: 车速/静止状态
    0x129, // SCCM_steeringAngleSensor: 方向盘转角
    0x311, // UI_warning: 安全带/转向灯/车门
    0x39B, // DAS_status: 盲区/碰撞预警
    0x39D, // IBST_status: 制动踏板
    0x3F5, // VCFRONT_lighting: 前部灯光
};
static constexpr uint8_t kDashboardSniffIdCount = sizeof(kDashboardSniffIds) / sizeof(kDashboardSniffIds[0]);

// CAN ID collector — tracks every observed CAN ID and its receive count
#define CAN_ID_COLLECTOR_MAX 500
#define CAN_ID_COLLECTOR_COUNT_MAX 9999
struct CanIdEntry
{
    uint32_t id;
    uint16_t count;
};
static CanIdEntry canIdEntries[CAN_ID_COLLECTOR_MAX];
static int canIdEntryCount = 0;

static void canIdCollectorPush(uint32_t id)
{
    for (int i = 0; i < canIdEntryCount; i++)
    {
        if (canIdEntries[i].id == id)
        {
            if (canIdEntries[i].count < CAN_ID_COLLECTOR_COUNT_MAX)
                canIdEntries[i].count++;
            return;
        }
    }
    if (canIdEntryCount < CAN_ID_COLLECTOR_MAX)
    {
        canIdEntries[canIdEntryCount].id = id;
        canIdEntries[canIdEntryCount].count = 1;
        canIdEntryCount++;
    }
}

static void sniffPush(const CanFrame &f)
{
    uint8_t dlc = (f.dlc <= 8) ? f.dlc : 8;
    sniffBuf[sniffHead] = {millis(), f.id, dlc, {}};
    memcpy(sniffBuf[sniffHead].data, f.data, dlc);
    sniffHead = (sniffHead + 1) % SNIFFER_CAP;
    if (sniffCount < SNIFFER_CAP)
        sniffCount++;
}

#define LOG_CAP 80
struct LogEntry
{
    String msg;
    unsigned long seq;
};
static LogEntry logBuf[LOG_CAP];
static int logHead = 0;
static int logCount = 0;
static unsigned long logSeq = 0;

static void dashLog(const String &s)
{
    logBuf[logHead] = {String(millis() / 1000) + "s " + s, ++logSeq};
    logHead = (logHead + 1) % LOG_CAP;
    if (logCount < LOG_CAP)
        logCount++;
    Serial.println(s);
}

// Called for every received CAN frame
static void mcpDashOnFrame(const CanFrame &f)
{
    unsigned long now = millis();
    rxCount++;
    lastFrameMs = now;
    canOnline = true;
    fpsFrames++;
    sniffPush(f);
    canIdCollectorPush(f.id);

    // Update ESP-NOW signal state from this frame
    espnowUpdateFromFrame(f);

    if (recActive)
    {
        int idx = recCount;
        if (idx < REC_CAP)
        {
            uint8_t dlc = (f.dlc <= 8) ? f.dlc : 8;
            recBuf[idx].ts = millis();
            recBuf[idx].id = f.id;
            recBuf[idx].dlc = dlc;
            memcpy(recBuf[idx].data, f.data, dlc);
            recCount = idx + 1;
            if (recCount >= REC_CAP)
                recActive = false;
        }
    }
}

static String jsonEscape(const String &s)
{
    String out;
    out.reserve(s.length() + 8);
    for (unsigned int i = 0; i < s.length(); i++)
    {
        char c = s.charAt(i);
        if (c == '"')
            out += "\\\"";
        else if (c == '\\')
            out += "\\\\";
        else if (c == '\n')
            out += "\\n";
        else if (c == '\r')
            out += "\\r";
        else if (c < 0x20)
            out += ' ';
        else
            out += c;
    }
    return out;
}

static bool dashStaSsidLooksCorrupt(const String &ssid)
{
    return ssid.indexOf("\"ssid\"") >= 0 || ssid.indexOf("{\"") >= 0 ||
           ssid.indexOf("\",\"") >= 0;
}

static void dashSavePrefs()
{
    prefs.begin(PREFS_NS, false);
    prefs.putBool("eprn", dashHandler ? (bool)dashHandler->enablePrint : true);
    prefs.end();
}

static bool dashApPasswordLengthValid(size_t len)
{
    return len >= kDashMinApPassLen && len <= kDashMaxPassLen;
}

static bool dashApConfigValid(const char *ssid, const char *pass)
{
    size_t ssidLen = strlen(ssid);
    size_t passLen = strlen(pass);
    return ssidLen > 0 && ssidLen <= kDashMaxSsidLen && dashApPasswordLengthValid(passLen);
}

static void dashUseDefaultApConfig()
{
    strlcpy(apSSID, DASH_SSID, sizeof(apSSID));
    strlcpy(apPass, DASH_PASS, sizeof(apPass));
    apHidden = false;
}

static bool dashStaConfigLengthValid(const String &ssid, const String &pass)
{
    return ssid.length() <= kDashMaxSsidLen && pass.length() <= kDashMaxPassLen;
}

static void dashLoadPrefs()
{
    prefs.begin(PREFS_NS, false);
    bool ep = prefs.getBool("eprn", true);
    if (dashHandler)
        dashHandler->enablePrint = ep;

    String apSsidPref = prefs.isKey("ap_ssid") ? prefs.getString("ap_ssid", "") : "";
    String apPassPref = prefs.isKey("ap_pass") ? prefs.getString("ap_pass", "") : "";
    bool hasApOverride = apSsidPref.length() > 0 || apPassPref.length() > 0 || prefs.isKey("ap_hidden");
    bool invalidApOverride = apSsidPref.length() > kDashMaxSsidLen ||
                             (apPassPref.length() > 0 && !dashApPasswordLengthValid(apPassPref.length()));
    if (apSsidPref.length() > 0)
        strlcpy(apSSID, apSsidPref.c_str(), sizeof(apSSID));
    else
        strlcpy(apSSID, DASH_SSID, sizeof(apSSID));
    if (apPassPref.length() > 0)
        strlcpy(apPass, apPassPref.c_str(), sizeof(apPass));
    else
        strlcpy(apPass, DASH_PASS, sizeof(apPass));
    apHidden = prefs.getBool("ap_hidden", false);
    if (invalidApOverride || !dashApConfigValid(apSSID, apPass))
    {
        if (hasApOverride)
        {
            prefs.remove("ap_ssid");
            prefs.remove("ap_pass");
            prefs.remove("ap_hidden");
            dashLog("[WIFI] Invalid saved AP config ignored");
        }
        dashUseDefaultApConfig();
    }

    wifiNetworkCount = 0;
    for (uint8_t i = 0; i < kDashMaxWifiNetworks; i++)
        dashClearWifiNetwork(wifiNetworks[i]);

    uint8_t storedCount = prefs.getUChar("wn_cnt", 0);
    if (storedCount > kDashMaxWifiNetworks)
        storedCount = kDashMaxWifiNetworks;

    for (uint8_t i = 0; i < storedCount; i++)
    {
        DashWifiNetwork &n = wifiNetworks[wifiNetworkCount];
        String s = prefs.getString(dashWifiKey(i, "s").c_str(), "");
        String p = prefs.getString(dashWifiKey(i, "p").c_str(), "");
        if (!dashStaConfigLengthValid(s, p) || dashStaSsidLooksCorrupt(s) || s.length() == 0)
            continue;
        strlcpy(n.ssid, s.c_str(), sizeof(n.ssid));
        strlcpy(n.pass, p.c_str(), sizeof(n.pass));
        n.useStatic = prefs.getBool(dashWifiKey(i, "t").c_str(), false);
        if (n.useStatic)
        {
            String ip = prefs.getString(dashWifiKey(i, "i").c_str(), "0.0.0.0");
            String gw = prefs.getString(dashWifiKey(i, "g").c_str(), "0.0.0.0");
            String mk = prefs.getString(dashWifiKey(i, "m").c_str(), "255.255.255.0");
            String dn = prefs.getString(dashWifiKey(i, "d").c_str(), "0.0.0.0");
            strlcpy(n.ip, ip.c_str(), sizeof(n.ip));
            strlcpy(n.gw, gw.c_str(), sizeof(n.gw));
            strlcpy(n.mask, mk.c_str(), sizeof(n.mask));
            strlcpy(n.dns, dn.c_str(), sizeof(n.dns));
        }
        wifiNetworkCount++;
    }

    if (wifiNetworkCount == 0 && prefs.isKey("wifi_ssid"))
    {
        String s = prefs.getString("wifi_ssid", "");
        String p = prefs.getString("wifi_pass", "");
        if (dashStaConfigLengthValid(s, p) && !dashStaSsidLooksCorrupt(s) && s.length() > 0)
        {
            DashWifiNetwork &n = wifiNetworks[0];
            strlcpy(n.ssid, s.c_str(), sizeof(n.ssid));
            strlcpy(n.pass, p.c_str(), sizeof(n.pass));
            n.useStatic = prefs.getBool("wifi_static", false);
            if (n.useStatic)
            {
                strlcpy(n.ip, prefs.getString("wifi_ip", "0.0.0.0").c_str(), sizeof(n.ip));
                strlcpy(n.gw, prefs.getString("wifi_gw", "0.0.0.0").c_str(), sizeof(n.gw));
                strlcpy(n.mask, prefs.getString("wifi_mask", "255.255.255.0").c_str(), sizeof(n.mask));
                strlcpy(n.dns, prefs.getString("wifi_dns", "0.0.0.0").c_str(), sizeof(n.dns));
            }
            wifiNetworkCount = 1;
            prefs.putUChar("wn_cnt", 1);
            prefs.putString(dashWifiKey(0, "s").c_str(), s);
            prefs.putString(dashWifiKey(0, "p").c_str(), p);
            prefs.putBool(dashWifiKey(0, "t").c_str(), n.useStatic);
            if (n.useStatic)
            {
                prefs.putString(dashWifiKey(0, "i").c_str(), String(n.ip));
                prefs.putString(dashWifiKey(0, "g").c_str(), String(n.gw));
                prefs.putString(dashWifiKey(0, "m").c_str(), String(n.mask));
                prefs.putString(dashWifiKey(0, "d").c_str(), String(n.dns));
            }
            dashLog("[WIFI] Migrated legacy STA config to slot 0");
        }
        prefs.remove("wifi_ssid");
        prefs.remove("wifi_pass");
        prefs.remove("wifi_static");
        prefs.remove("wifi_ip");
        prefs.remove("wifi_gw");
        prefs.remove("wifi_mask");
        prefs.remove("wifi_dns");
    }

    if (wifiNetworkCount > 0)
    {
        const DashWifiNetwork &n = wifiNetworks[0];
        strlcpy(staSSID, n.ssid, sizeof(staSSID));
        strlcpy(staPass, n.pass, sizeof(staPass));
        staStaticIP = n.useStatic;
        if (n.useStatic)
        {
            staIP.fromString(n.ip);
            staGW.fromString(n.gw);
            staMask.fromString(n.mask);
            staDNS.fromString(n.dns);
        }
        wifiActiveSlot = 0;
        wifiNextRotateSlot = wifiNetworkCount > 1 ? 1 : 0;
    }
    else
    {
        staSSID[0] = 0;
        staPass[0] = 0;
        staStaticIP = false;
        wifiActiveSlot = -1;
        wifiNextRotateSlot = 0;
    }

    updateBetaChannel = prefs.getBool("update_beta", false);
    autoUpdateEnabled = prefs.getBool("auto_upd", false);
    prefs.end();

    dashLog("[BOOT] Prefs loaded");
}

// MCP2515 hardware filters — only the 7 selected CAN IDs
static void dashApplyFilters()
{
#if defined(DRIVER_ESP32_EXT_MCP2515)
    if (!dashMcp)
        return;
    dashMcp->setConfigMode();

    // Use the 7 sniff IDs as hardware filters
    dashMcp->setFilterMask(MCP2515::MASK0, false, 0x7FF);
    dashMcp->setFilter(MCP2515::RXF0, false, 0x118);
    dashMcp->setFilter(MCP2515::RXF1, false, 0x155);

    dashMcp->setFilterMask(MCP2515::MASK1, false, 0x7FF);
    dashMcp->setFilter(MCP2515::RXF2, false, 0x129);
    dashMcp->setFilter(MCP2515::RXF3, false, 0x311);
    dashMcp->setFilter(MCP2515::RXF4, false, 0x39B);
    dashMcp->setFilter(MCP2515::RXF5, false, 0x3F5);

    dashMcp->setNormalMode();
    dashLog("[CFG] MCP2515 filters: 7 signals (0x118/155/129/311/39B/39D/3F5)");
#endif
}

#if defined(DRIVER_ESP32_EXT_MCP2515)
static unsigned long lastEflgCheckMs = 0;
static void dashCheckBusHealth()
{
    if (!dashMcp)
        return;
    if (millis() - lastEflgCheckMs < 5000)
        return;
    lastEflgCheckMs = millis();
    uint8_t eflg = dashMcp->getErrorFlags();
    mcpEflg = eflg;
    if (eflg & 0x20)
    {
        dashLog("[ERR] MCP2515 BUS-OFF -- recovering");
        dashMcp->reset();
        delay(10);
        dashMcp->setBitrate(CAN_500KBPS, MCP_CRYSTAL_FREQ);
        dashApplyFilters();
        dashLog("[OK] MCP2515 recovered");
    }
}
#else
static void dashCheckBusHealth() {}
#endif

static WebServer server(80);

static void handleRoot()
{
    server.sendHeader("Content-Encoding", "gzip");
    server.sendHeader("Cache-Control", "max-age=3600");
#ifdef ESP_PLATFORM
    server.sendRaw(200, "text/html",
                   reinterpret_cast<const char *>(DASH_HTML_GZ),
                   DASH_HTML_GZ_LEN);
#else
    server.send_P(200, "text/html", reinterpret_cast<const char *>(DASH_HTML_GZ), DASH_HTML_GZ_LEN);
#endif
}

static void handleStatus()
{
    if (canOnline && millis() - lastFrameMs > 10000)
    {
        canOnline = false;
        dashLog("[CAN] Bus OFFLINE (timeout)");
    }
    unsigned long now = millis();
    if (now - fpsLastMs >= 2000)
    {
        fps = fpsFrames * 1000.0f / max(1UL, now - fpsLastMs);
        fpsFrames = 0;
        fpsLastMs = now;
    }

    bool ep = dashHandler ? (bool)dashHandler->enablePrint : true;

    String j = "{\"eprn\":";
    j += ep ? "true" : "false";
    j += ",\"can\":";
    j += canOnline ? "true" : "false";
    j += ",\"rx\":";
    j += rxCount;
    j += ",\"fps\":";
    {
        unsigned long fpsX10 = static_cast<unsigned long>(fps * 10.0f + 0.5f);
        j += String(fpsX10 / 10);
        j += ".";
        j += String(fpsX10 % 10);
    }
    j += ",\"eflg\":";
    j += mcpEflg;
    j += ",\"up\":";
    j += (millis() - startMs) / 1000;
#if defined(HW4)
    j += ",\"hw\":2";
#elif defined(LEGACY)
    j += ",\"hw\":0";
#else
    j += ",\"hw\":1";
#endif
    j += "}";
    server.send(200, "application/json", j);
}

static void handleConfig()
{
    dashSavePrefs();
    server.send(200, "application/json", "{\"ok\":true}");
}

static void handleLoggingConfig()
{
    if (server.hasArg("eprn") && dashHandler)
    {
        bool ep = server.arg("eprn") == "1";
        dashHandler->enablePrint = ep;
        dashLog("[CFG] Logging " + String(ep ? "ON" : "OFF"));
    }
    dashSavePrefs();
    server.send(200, "application/json", "{\"ok\":true}");
}

static void handleFrames()
{
    String j = "{\"frames\":[";
    int start = (sniffCount < SNIFFER_CAP) ? 0 : sniffHead;
    int count = min(sniffCount, SNIFFER_CAP);
    for (int i = 0; i < count; i++)
    {
        int idx = (start + i) % SNIFFER_CAP;
        SniffFrame &f = sniffBuf[idx];
        if (i)
            j += ",";
        j += "{\"ts\":" + String(f.ts) +
             ",\"id\":" + String(f.id) +
             ",\"dlc\":" + String(f.dlc) +
             ",\"data\":[";
        for (int b = 0; b < f.dlc; b++)
        {
            if (b)
                j += ",";
            j += String(f.data[b]);
        }
        char trans[128] = {0};
        formatCanTranslation(f.id, f.data, f.dlc, trans, sizeof(trans));
        j += "],\"name\":\"" + jsonEscape(decodeCanId(f.id)) +
             "\",\"trans\":\"" + jsonEscape(trans) + "\"}";
    }
    j += "]}";
    server.send(200, "application/json", j);
}

static void handleLog()
{
    unsigned long since = 0;
    if (server.hasArg("since"))
        since = strtoul(server.arg("since").c_str(), nullptr, 10);
    String j = "{\"seq\":";
    j += logSeq;
    j += ",\"lines\":[";
    int start = (logCount < LOG_CAP) ? 0 : logHead;
    int count = min(logCount, LOG_CAP);
    bool first = true;
    for (int i = 0; i < count; i++)
    {
        int idx = (start + i) % LOG_CAP;
        if (logBuf[idx].seq <= since)
            continue;
        if (!first)
            j += ",";
        first = false;
        j += "\"" + jsonEscape(logBuf[idx].msg) + "\"";
    }
    j += "]}";
    server.send(200, "application/json", j);
}

static void handleResetStats()
{
    rxCount = 0;
    dashLog("[CFG] Stats reset");
    server.send(200, "application/json", "{\"ok\":true}");
}

static void handleCanIds()
{
    String j = "{\"ids\":[";
    for (int i = 0; i < canIdEntryCount; i++)
    {
        if (i)
            j += ",";
        j += "{\"id\":" + String(canIdEntries[i].id) +
             ",\"hex\":\"0x" + String(canIdEntries[i].id, HEX) + "\"" +
             ",\"count\":" + String(canIdEntries[i].count) + "}";
    }
    j += "]}";
    server.send(200, "application/json", j);
}

static void handleCanIdsReset()
{
    canIdEntryCount = 0;
    dashLog("[CFG] CAN ID collector reset");
    server.send(200, "application/json", "{\"ok\":true}");
}

static void handleRecStart()
{
    recCount = 0;
    recSaved = false;
    recActive = true;
    dashLog("[REC] Recording started");
    server.send(200, "application/json", "{\"ok\":true}");
}

static void handleRecStop()
{
    recActive = false;
    int n = recCount;
    File f = SPIFFS.open("/rec.csv", "w");
    if (f)
    {
        f.println("ts_ms,id,dlc,b0,b1,b2,b3,b4,b5,b6,b7");
        for (int i = 0; i < n; i++)
        {
            f.print(recBuf[i].ts);
            f.print(",");
            f.print(recBuf[i].id);
            f.print(",");
            f.print(recBuf[i].dlc);
            for (int b = 0; b < 8; b++)
            {
                f.print(",");
                f.print(recBuf[i].data[b]);
            }
            f.println();
        }
        f.close();
        recSaved = true;
        dashLog("[REC] Saved " + String(n) + " frames to SPIFFS");
    }
    else
    {
        dashLog("[REC] SPIFFS write failed");
    }
    server.send(200, "application/json", "{\"ok\":true}");
}

static void handleRecStatus()
{
    String j = "{\"active\":";
    j += recActive ? "true" : "false";
    j += ",\"count\":";
    j += recCount;
    j += ",\"cap\":";
    j += REC_CAP;
    j += ",\"saved\":";
    j += recSaved ? "true" : "false";
    j += "}";
    server.send(200, "application/json", j);
}

static void handleRecDownload()
{
    if (!SPIFFS.exists("/rec.csv"))
    {
        server.send(404, "text/plain", "No recording saved yet");
        return;
    }
    File f = SPIFFS.open("/rec.csv", "r");
    server.sendHeader("Content-Disposition", "attachment; filename=\"can_recording.csv\"");
    server.streamFile(f, "text/csv");
    f.close();
}

static void handleReboot()
{
    server.send(200, "text/plain", "Rebooting...");
    delay(200);
    ESP.restart();
}

static void handleOtaResult()
{
    if (!server.authenticate(DASH_OTA_USER, DASH_OTA_PASS))
    {
        server.requestAuthentication();
        return;
    }
    bool ok = !Update.hasError();
    server.sendHeader("Connection", "close");
    server.send(ok ? 200 : 500, "text/plain", ok ? "OK" : "FAIL");
    if (ok)
    {
        dashLog("[OTA] Upload complete -- rebooting");
        delay(300);
        ESP.restart();
    }
    else
    {
        dashLog("[OTA] Upload FAILED");
    }
}

static void handleOtaUpload()
{
    if (!server.authenticate(DASH_OTA_USER, DASH_OTA_PASS))
        return;
    HTTPUpload &upload = server.upload();
    if (upload.status == UPLOAD_FILE_START)
    {
        dashLog("[OTA] Receiving: " + String(upload.filename.c_str()));
        esp_task_wdt_deinit();
        if (!Update.begin(UPDATE_SIZE_UNKNOWN))
            dashLog("[OTA] Begin failed");
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
            dashLog("[OTA] Write error");
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        if (Update.end(true))
            dashLog("[OTA] Done: " + String(upload.totalSize) + " bytes");
        else
            dashLog("[OTA] End failed");
    }
}

// ── WiFi STA ──

static bool dashStartAccessPoint(bool withSta)
{
    WiFi.persistent(false);
    WiFi.mode(withSta ? WIFI_AP_STA : WIFI_AP);
    WiFi.setSleep(false);

    IPAddress apIp(192, 168, 4, 1);
    IPAddress apMask(255, 255, 255, 0);
    WiFi.softAPConfig(apIp, apIp, apMask);

    if (!dashApConfigValid(apSSID, apPass))
        dashUseDefaultApConfig();

    bool ok = WiFi.softAP(apSSID, apPass, kDashApChannel, apHidden ? 1 : 0, kDashApMaxConn);
    if (!ok)
    {
        dashUseDefaultApConfig();
        ok = WiFi.softAP(apSSID, apPass, kDashApChannel, 0, kDashApMaxConn);
    }
    if (!ok)
        dashLog("[WIFI] AP start failed");
    return ok;
}

static void dashBeginSTA()
{
    if (strlen(staSSID) == 0)
        return;

    if (staStaticIP && (uint32_t)staIP != 0)
    {
        WiFi.config(staIP, staGW, staMask, staDNS);
        dashLog("[WIFI] Static IP: " + staIP.toString());
    }
    else
    {
        WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
    }
    WiFi.begin(staSSID, staPass);
    staConnectAttemptActive = true;
    staConnectStartedAt = millis();
    staRetryAt = 0;
    dashLog("[WIFI] Connecting to " + String(staSSID) + "...");
}

static void dashPrepareStaReconnect()
{
    if (staConnectAttemptActive || staConnected || WiFi.status() == WL_CONNECTED)
        WiFi.disconnect(false, false);
    staConnected = false;
    staConnectAttemptActive = false;
    staRetryAt = 0;
    autoUpdateEligibleAt = 0;
}

static void dashApplyWifiSlot(uint8_t slot)
{
    if (slot >= wifiNetworkCount)
        return;
    const DashWifiNetwork &n = wifiNetworks[slot];
    strlcpy(staSSID, n.ssid, sizeof(staSSID));
    strlcpy(staPass, n.pass, sizeof(staPass));
    staStaticIP = n.useStatic;
    if (n.useStatic)
    {
        staIP.fromString(n.ip);
        staGW.fromString(n.gw);
        staMask.fromString(n.mask);
        staDNS.fromString(n.dns);
    }
    else
        staIP = IPAddress(0, 0, 0, 0);
    wifiActiveSlot = static_cast<int8_t>(slot);
}

static void dashRotateAndConnect()
{
    if (wifiNetworkCount == 0)
        return;
    uint8_t next = wifiNextRotateSlot % wifiNetworkCount;
    wifiNextRotateSlot = (next + 1) % wifiNetworkCount;
    dashApplyWifiSlot(next);
    dashLog("[WIFI] Trying slot " + String(next) + ": " + String(staSSID));
    dashStartAccessPoint(true);
    dashBeginSTA();
}

static void dashScheduleSTAConnect(unsigned long delayMs)
{
    if (strlen(staSSID) == 0)
        return;
    staConnectAttemptActive = false;
    staRetryAt = millis() + delayMs;
}

static void dashPrepareWifiScan()
{
    WiFi.mode(WIFI_AP_STA);
    WiFi.setSleep(false);
}

static void performAutoUpdate();

static void dashCheckWifi()
{
    static unsigned long lastCheck = 0;
    if (wifiNetworkCount == 0)
        return;
    unsigned long now = millis();
    if (!staConnected && !staConnectAttemptActive && staRetryAt > 0 && (long)(now - staRetryAt) >= 0)
    {
        staRetryAt = 0;
        dashRotateAndConnect();
    }

    if (now - lastCheck < 5000)
        return;
    lastCheck = now;

    bool connected = WiFi.status() == WL_CONNECTED;
    if (connected != staConnected)
    {
        staConnected = connected;
        if (connected)
        {
            staConnectAttemptActive = false;
            staRetryAt = 0;
            dashLog("[WIFI] Connected to " + String(staSSID) + " IP: " + WiFi.localIP().toString());
            if (autoUpdateEnabled && !autoUpdateDone)
                autoUpdateEligibleAt = millis() + 15000;
        }
        else
        {
            dashLog("[WIFI] Disconnected from " + String(staSSID));
            staConnectAttemptActive = false;
            staRetryAt = now + kDashStaRetryMs;
        }
    }

    if (!connected && staConnectAttemptActive && now - staConnectStartedAt >= kDashStaConnectTimeoutMs)
    {
        staConnectAttemptActive = false;
        WiFi.disconnect(false, false);
        dashStartAccessPoint(false);
        staRetryAt = now + kDashStaRetryMs;
        dashLog("[WIFI] STA connect timed out; keeping AP-only mode");
    }

    if (autoUpdateEnabled && !autoUpdateDone && staConnected && autoUpdateEligibleAt > 0 && millis() >= autoUpdateEligibleAt)
    {
        autoUpdateDone = true;
        performAutoUpdate();
    }
}

static void handleWifiScan()
{
    dashPrepareWifiScan();
    int n = WiFi.scanNetworks(false, false, false, 300);
    String j = "{\"networks\":[";
    for (int i = 0; i < n && i < 20; i++)
    {
        if (i)
            j += ",";
        j += "{\"ssid\":\"" + jsonEscape(WiFi.SSID(i).c_str()) + "\"";
        j += ",\"rssi\":" + String(WiFi.RSSI(i));
        j += ",\"enc\":" + String(WiFi.encryptionType(i) != WIFI_AUTH_OPEN ? "true" : "false");
        j += ",\"ch\":" + String(WiFi.channel(i));
        j += "}";
    }
    j += "]}";
    WiFi.scanDelete();
    server.send(200, "application/json", j);
}

static void dashPersistWifiSlot(uint8_t slot)
{
    if (slot >= wifiNetworkCount)
        return;
    const DashWifiNetwork &n = wifiNetworks[slot];
    prefs.putString(dashWifiKey(slot, "s").c_str(), String(n.ssid));
    prefs.putString(dashWifiKey(slot, "p").c_str(), String(n.pass));
    prefs.putBool(dashWifiKey(slot, "t").c_str(), n.useStatic);
    if (n.useStatic)
    {
        prefs.putString(dashWifiKey(slot, "i").c_str(), String(n.ip));
        prefs.putString(dashWifiKey(slot, "g").c_str(), String(n.gw));
        prefs.putString(dashWifiKey(slot, "m").c_str(), String(n.mask));
        prefs.putString(dashWifiKey(slot, "d").c_str(), String(n.dns));
    }
    else
    {
        prefs.remove(dashWifiKey(slot, "i").c_str());
        prefs.remove(dashWifiKey(slot, "g").c_str());
        prefs.remove(dashWifiKey(slot, "m").c_str());
        prefs.remove(dashWifiKey(slot, "d").c_str());
    }
}

static void dashRemoveWifiSlotKeys(uint8_t slot)
{
    prefs.remove(dashWifiKey(slot, "s").c_str());
    prefs.remove(dashWifiKey(slot, "p").c_str());
    prefs.remove(dashWifiKey(slot, "t").c_str());
    prefs.remove(dashWifiKey(slot, "i").c_str());
    prefs.remove(dashWifiKey(slot, "g").c_str());
    prefs.remove(dashWifiKey(slot, "m").c_str());
    prefs.remove(dashWifiKey(slot, "d").c_str());
}

static void handleWifiConfig()
{
    if (!server.hasArg("ssid"))
    {
        server.send(200, "application/json", "{\"ok\":true}");
        return;
    }

    String ssid = server.arg("ssid");
    String pass = server.arg("pass");
    if (!dashStaConfigLengthValid(ssid, pass) || dashStaSsidLooksCorrupt(ssid) || ssid.length() == 0)
    {
        server.send(400, "application/json", "{\"ok\":false,\"error\":\"Invalid SSID or password\"}");
        return;
    }

    int idx = -1;
    if (server.hasArg("idx"))
        idx = server.arg("idx").toInt();
    if (idx < 0 || idx > wifiNetworkCount)
        idx = wifiNetworkCount;

    if (idx == kDashMaxWifiNetworks)
    {
        server.send(400, "application/json", "{\"ok\":false,\"error\":\"Max networks reached\"}");
        return;
    }

    DashWifiNetwork &n = wifiNetworks[idx];
    dashClearWifiNetwork(n);
    strlcpy(n.ssid, ssid.c_str(), sizeof(n.ssid));
    strlcpy(n.pass, pass.c_str(), sizeof(n.pass));
    n.useStatic = server.hasArg("static") && server.arg("static") == "1";
    if (n.useStatic)
    {
        strlcpy(n.ip, server.arg("ip").c_str(), sizeof(n.ip));
        strlcpy(n.gw, server.arg("gw").c_str(), sizeof(n.gw));
        strlcpy(n.mask, server.arg("mask").c_str(), sizeof(n.mask));
        strlcpy(n.dns, server.arg("dns").c_str(), sizeof(n.dns));
    }

    if (idx == wifiNetworkCount)
        wifiNetworkCount++;

    prefs.begin(PREFS_NS, false);
    prefs.putUChar("wn_cnt", wifiNetworkCount);
    dashPersistWifiSlot(idx);
    prefs.end();

    dashLog("[WIFI] Saved slot " + String(idx) + ": " + ssid);

    wifiNextRotateSlot = idx;
    dashApplyWifiSlot(idx);
    dashPrepareStaReconnect();

    server.send(200, "application/json", "{\"ok\":true,\"idx\":" + String(idx) + "}");
    dashScheduleSTAConnect(1000);
}

static void handleWifiDelete()
{
    if (!server.hasArg("idx"))
    {
        server.send(400, "application/json", "{\"ok\":false,\"error\":\"missing idx\"}");
        return;
    }
    int idx = server.arg("idx").toInt();
    if (idx < 0 || idx >= wifiNetworkCount)
    {
        server.send(400, "application/json", "{\"ok\":false,\"error\":\"bad idx\"}");
        return;
    }

    String removedSsid = wifiNetworks[idx].ssid;
    for (uint8_t i = idx; i + 1 < wifiNetworkCount; i++)
        wifiNetworks[i] = wifiNetworks[i + 1];
    wifiNetworkCount--;
    dashClearWifiNetwork(wifiNetworks[wifiNetworkCount]);

    prefs.begin(PREFS_NS, false);
    prefs.putUChar("wn_cnt", wifiNetworkCount);
    for (uint8_t i = 0; i < wifiNetworkCount; i++)
        dashPersistWifiSlot(i);
    for (uint8_t i = wifiNetworkCount; i < kDashMaxWifiNetworks; i++)
        dashRemoveWifiSlotKeys(i);
    prefs.end();

    dashLog("[WIFI] Deleted slot " + String(idx) + ": " + removedSsid);

    if (wifiActiveSlot == idx)
    {
        wifiActiveSlot = -1;
        if (staConnectAttemptActive || staConnected)
        {
            WiFi.disconnect(false, false);
            staConnectAttemptActive = false;
            staConnected = false;
        }
        if (wifiNetworkCount > 0)
        {
            wifiNextRotateSlot = 0;
            dashRotateAndConnect();
        }
        else
        {
            staSSID[0] = 0;
            staPass[0] = 0;
            dashStartAccessPoint(false);
        }
    }
    else if (wifiActiveSlot > idx)
        wifiActiveSlot--;
    if (wifiNextRotateSlot >= wifiNetworkCount)
        wifiNextRotateSlot = 0;

    server.send(200, "application/json", "{\"ok\":true}");
}

static void handleWifiNetworks()
{
    String j = "{\"max\":";
    j += kDashMaxWifiNetworks;
    j += ",\"count\":";
    j += wifiNetworkCount;
    j += ",\"active\":";
    j += wifiActiveSlot;
    j += ",\"networks\":[";
    for (uint8_t i = 0; i < wifiNetworkCount; i++)
    {
        if (i)
            j += ",";
        const DashWifiNetwork &n = wifiNetworks[i];
        j += "{\"idx\":";
        j += i;
        j += ",\"ssid\":\"" + jsonEscape(n.ssid) + "\"";
        j += ",\"hasPass\":" + String(strlen(n.pass) > 0 ? "true" : "false");
        j += ",\"static\":" + String(n.useStatic ? "true" : "false");
        if (n.useStatic)
        {
            j += ",\"ip\":\"" + String(n.ip) + "\"";
            j += ",\"gw\":\"" + String(n.gw) + "\"";
            j += ",\"mask\":\"" + String(n.mask) + "\"";
            j += ",\"dns\":\"" + String(n.dns) + "\"";
        }
        j += "}";
    }
    j += "]}";
    server.send(200, "application/json", j);
}

static void handleWifiStatus()
{
    bool stored = wifiNetworkCount > 0;
    bool connectedNow = WiFi.status() == WL_CONNECTED;
    IPAddress staIp = WiFi.localIP();
    bool hasStaIp = static_cast<uint32_t>(staIp) != 0;
    bool connected = connectedNow || staConnected || hasStaIp;
    String activeSsid = connectedNow ? WiFi.SSID() : String(staSSID);
    if (dashStaSsidLooksCorrupt(activeSsid))
        activeSsid = "";
    String j = "{\"connected\":";
    j += connected ? "true" : "false";
    j += ",\"ssid\":\"" + jsonEscape(activeSsid) + "\"";
    j += ",\"stored\":" + String(stored ? "true" : "false");
    j += ",\"count\":" + String(wifiNetworkCount);
    j += ",\"active\":" + String(wifiActiveSlot);
    if (connected)
        j += ",\"ip\":\"" + staIp.toString() + "\"";
    j += ",\"static\":" + String(staStaticIP ? "true" : "false");
    if (staStaticIP)
    {
        j += ",\"cfg_ip\":\"" + staIP.toString() + "\"";
        j += ",\"cfg_gw\":\"" + staGW.toString() + "\"";
        j += ",\"cfg_mask\":\"" + staMask.toString() + "\"";
        j += ",\"cfg_dns\":\"" + staDNS.toString() + "\"";
    }
    if (!connected)
        j += ",\"connecting\":" + String(staConnectAttemptActive ? "true" : "false");
    j += "}";
    server.send(200, "application/json", j);
}

static void handleCanPins()
{
    Preferences canPrefs;
    bool customized = false;
    int tx = -1, rx = -1;
#if defined(DRIVER_TWAI)
    tx = (int)TWAI_TX_PIN;
    rx = (int)TWAI_RX_PIN;
#endif
    if (canPrefs.begin("can", false))
    {
        int storedTx = canPrefs.getChar("tx", -1);
        int storedRx = canPrefs.getChar("rx", -1);
        canPrefs.end();
        if (storedTx >= 0 && storedTx <= 39)
        {
            tx = storedTx;
            customized = true;
        }
        if (storedRx >= 0 && storedRx <= 39)
        {
            rx = storedRx;
            customized = true;
        }
    }
    String j = "{\"tx\":" + String(tx);
    j += ",\"rx\":" + String(rx);
    j += ",\"customized\":" + String(customized ? "true" : "false");
    j += "}";
    server.send(200, "application/json", j);
}

static void handleCanPinsSave()
{
    int tx = server.arg("tx").toInt();
    int rx = server.arg("rx").toInt();

    if (tx < 0 || tx > 39 || rx < 0 || rx > 39)
    {
        server.send(400, "application/json", "{\"ok\":false,\"error\":\"Pin must be 0-39\"}");
        return;
    }
    if (tx == rx)
    {
        server.send(400, "application/json", "{\"ok\":false,\"error\":\"TX and RX must differ\"}");
        return;
    }
#ifndef DASH_ALLOW_CAN_GPIO_6_11
#define DASH_ALLOW_CAN_GPIO_6_11 0
#endif
#if !DASH_ALLOW_CAN_GPIO_6_11
    if ((tx >= 6 && tx <= 11) || (rx >= 6 && rx <= 11))
    {
        server.send(400, "application/json", "{\"ok\":false,\"error\":\"GPIO 6-11 reserved for flash\"}");
        return;
    }
#endif

    Preferences canPrefs;
    if (!canPrefs.begin("can", false))
    {
        server.send(500, "application/json", "{\"ok\":false,\"error\":\"NVS open failed\"}");
        return;
    }
    canPrefs.putChar("tx", (int8_t)tx);
    canPrefs.putChar("rx", (int8_t)rx);
    canPrefs.end();

    dashLog("[CAN] Pins saved: TX=" + String(tx) + " RX=" + String(rx) + " (reboot required)");
    server.send(200, "application/json", "{\"ok\":true,\"reboot\":true}");
}

static void handleApConfig()
{
    String newSsid = server.arg("ssid");
    String newPass = server.arg("pass");
    bool hasHidden = server.hasArg("hidden");
    bool newHidden = hasHidden && (server.arg("hidden") == "1" || server.arg("hidden") == "true");

    if (newSsid.length() == 0)
    {
        server.send(400, "application/json", "{\"ok\":false,\"error\":\"SSID required\"}");
        return;
    }
    if (newSsid.length() > kDashMaxSsidLen)
    {
        server.send(400, "application/json", "{\"ok\":false,\"error\":\"SSID must be 32 bytes or less\"}");
        return;
    }
    if (newPass.length() > 0 && !dashApPasswordLengthValid(newPass.length()))
    {
        server.send(400, "application/json", "{\"ok\":false,\"error\":\"Password must be 8-64 characters\"}");
        return;
    }

    strlcpy(apSSID, newSsid.c_str(), sizeof(apSSID));
    if (newPass.length() > 0)
        strlcpy(apPass, newPass.c_str(), sizeof(apPass));
    if (hasHidden)
        apHidden = newHidden;

    prefs.begin(PREFS_NS, false);
    prefs.putString("ap_ssid", newSsid);
    if (newPass.length() > 0)
        prefs.putString("ap_pass", newPass);
    if (hasHidden)
        prefs.putBool("ap_hidden", newHidden);
    prefs.end();

    dashLog("[WIFI] AP config updated: SSID=" + newSsid + (apHidden ? " (hidden)" : ""));
    server.send(200, "application/json", "{\"ok\":true,\"msg\":\"Saved. Reboot to apply new AP settings.\"}");
}

static void handleApStatus()
{
    Preferences p;
    bool stored = false;
    if (p.begin(PREFS_NS, false))
    {
        stored = p.isKey("ap_ssid") && p.getString("ap_ssid", "").length() > 0;
        p.end();
    }
    String j = "{\"ssid\":\"" + jsonEscape(apSSID) + "\"";
    j += ",\"ip\":\"" + WiFi.softAPIP().toString() + "\"";
    j += ",\"clients\":" + String(WiFi.softAPgetStationNum());
    j += ",\"stored\":" + String(stored ? "true" : "false");
    j += ",\"hidden\":" + String(apHidden ? "true" : "false");
    j += "}";
    server.send(200, "application/json", j);
}

// ── OTA GitHub updates ──

#ifndef FIRMWARE_VERSION
#define FIRMWARE_VERSION "unknown"
#endif

static const char *GITHUB_REPO = "ev-open-can-tools/ev-open-can-tools";

static const char *getFirmwareArtifact()
{
#if defined(DRIVER_ESP32_EXT_MCP2515)
    return "firmware-esp32-ext-mcp2515.bin";
#else
    return "firmware-esp32.bin";
#endif
}

static void parseVersion(const String &v, int &maj, int &min, int &pat, int &preRank, int &preNum)
{
    maj = min = pat = 0;
    preRank = 0;
    preNum = 0;
    int i = 0;
    int len = v.length();
    auto readInt = [&](int &out)
    {
        int val = 0;
        bool any = false;
        while (i < len && v[i] >= '0' && v[i] <= '9')
        {
            val = val * 10 + (v[i] - '0');
            i++;
            any = true;
        }
        if (any)
            out = val;
    };
    readInt(maj);
    if (i < len && v[i] == '.')
    {
        i++;
        readInt(min);
    }
    if (i < len && v[i] == '.')
    {
        i++;
        readInt(pat);
    }
    if (i < len && v[i] == '-')
    {
        i++;
        String tail = v.substring(i);
        tail.toLowerCase();
        if (tail.startsWith("alpha"))
            preRank = 1;
        else if (tail.startsWith("beta"))
            preRank = 2;
        else if (tail.startsWith("rc"))
            preRank = 3;
        else
            preRank = 0;
        int dot = tail.indexOf('.');
        if (dot >= 0)
            preNum = tail.substring(dot + 1).toInt();
    }
}

static bool isVersionNewer(const String &candidate, const String &current)
{
    int cM, cm, cp, cR, cN;
    int uM, um, up, uR, uN;
    parseVersion(candidate, cM, cm, cp, cR, cN);
    parseVersion(current, uM, um, up, uR, uN);
    if (cM != uM)
        return cM > uM;
    if (cm != um)
        return cm > um;
    if (cp != up)
        return cp > up;
    int cEff = (cR == 0) ? 1000 : cR;
    int uEff = (uR == 0) ? 1000 : uR;
    if (cEff != uEff)
        return cEff > uEff;
    return cN > uN;
}

static void handleUpdateCheck()
{
    if (!staConnected)
    {
        server.send(400, "application/json", "{\"ok\":false,\"error\":\"WiFi not connected\"}");
        return;
    }

    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;

    String url;
    if (updateBetaChannel)
        url = "https://api.github.com/repos/" + String(GITHUB_REPO) + "/releases?per_page=1";
    else
        url = "https://api.github.com/repos/" + String(GITHUB_REPO) + "/releases/latest";

    http.begin(client, url);
    http.addHeader("Accept", "application/vnd.github+json");
    http.addHeader("User-Agent", "ESP32-OTA");
    int code = http.GET();

    if (code != 200)
    {
        http.end();
        server.send(502, "application/json", "{\"ok\":false,\"error\":\"GitHub API error " + String(code) + "\"}");
        return;
    }

    String payload = http.getString();
    http.end();

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, payload);
    if (err)
    {
        server.send(500, "application/json", "{\"ok\":false,\"error\":\"JSON parse error\"}");
        return;
    }

    JsonObject release;
    if (updateBetaChannel)
    {
        JsonArray arr = doc.as<JsonArray>();
        for (JsonObject r : arr)
        {
            release = r;
            break;
        }
    }
    else
        release = doc.as<JsonObject>();

    if (release.isNull())
    {
        server.send(404, "application/json", "{\"ok\":false,\"error\":\"No release found\"}");
        return;
    }

    String tagName = release["tag_name"] | "";
    bool prerelease = release["prerelease"] | false;
    String version = tagName;
    if (version.startsWith("v"))
        version = version.substring(1);

    String downloadUrl = "";
    const char *artifact = getFirmwareArtifact();
    JsonArray assets = release["assets"];
    for (JsonObject asset : assets)
    {
        String name = asset["name"] | "";
        if (name == artifact)
        {
            downloadUrl = String(asset["browser_download_url"] | "");
            break;
        }
    }

    String j = "{\"ok\":true";
    j += ",\"current\":\"" + jsonEscape(FIRMWARE_VERSION) + "\"";
    j += ",\"latest\":\"" + jsonEscape(version.c_str()) + "\"";
    j += ",\"tag\":\"" + jsonEscape(tagName.c_str()) + "\"";
    j += ",\"prerelease\":" + String(prerelease ? "true" : "false");
    j += ",\"artifact\":\"" + jsonEscape(artifact) + "\"";
    j += ",\"url\":\"" + jsonEscape(downloadUrl.c_str()) + "\"";
    bool isNewer = isVersionNewer(version, String(FIRMWARE_VERSION));
    j += ",\"update\":" + String(isNewer && downloadUrl.length() > 0 ? "true" : "false");
    j += ",\"beta\":" + String(updateBetaChannel ? "true" : "false");
    j += "}";
    server.send(200, "application/json", j);
}

static void handleUpdateInstall()
{
    if (!staConnected)
    {
        server.send(400, "application/json", "{\"ok\":false,\"error\":\"WiFi not connected\"}");
        return;
    }

    String url = server.arg("url");
    if (url.length() == 0)
    {
        server.send(400, "application/json", "{\"ok\":false,\"error\":\"No URL provided\"}");
        return;
    }

    dashLog("[OTA] Starting GitHub update from: " + url);
    server.send(200, "application/json", "{\"ok\":true,\"msg\":\"Downloading and installing... Device will reboot.\"}");
    delay(500);

    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
    http.begin(client, url);
    http.addHeader("Accept", "application/octet-stream");
    int code = http.GET();

    if (code != 200)
    {
        dashLog("[OTA] Download failed: HTTP " + String(code));
        http.end();
        return;
    }

    int contentLength = http.getSize();
    if (contentLength <= 0)
    {
        dashLog("[OTA] Invalid content length: " + String(contentLength));
        http.end();
        return;
    }

    dashLog("[OTA] Downloading " + String(contentLength) + " bytes...");

    if (!Update.begin(contentLength))
    {
        dashLog("[OTA] Update.begin failed: " + String(Update.errorString()));
        http.end();
        return;
    }

    WiFiClient *stream = http.getStreamPtr();
    size_t written = Update.writeStream(*stream);
    http.end();

    if (written != (size_t)contentLength)
    {
        dashLog("[OTA] Written " + String(written) + " of " + String(contentLength) + " bytes: " + String(Update.errorString()));
        Update.abort();
        return;
    }

    if (!Update.end(true))
    {
        dashLog("[OTA] Update finalize failed: " + String(Update.errorString()));
        return;
    }

    if (!Update.isFinished())
    {
        dashLog("[OTA] Update not finished");
        return;
    }

    dashLog("[OTA] Update successful! Rebooting...");
    delay(1000);
    ESP.restart();
}

static void performAutoUpdate()
{
    if (!staConnected)
        return;

    dashLog("[AUTO-OTA] Checking for updates...");

    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;

    String url;
    if (updateBetaChannel)
        url = "https://api.github.com/repos/" + String(GITHUB_REPO) + "/releases?per_page=1";
    else
        url = "https://api.github.com/repos/" + String(GITHUB_REPO) + "/releases/latest";

    http.begin(client, url);
    http.addHeader("Accept", "application/vnd.github+json");
    http.addHeader("User-Agent", "ESP32-OTA");
    int code = http.GET();
    if (code != 200)
    {
        dashLog("[AUTO-OTA] GitHub API error " + String(code));
        http.end();
        return;
    }
    String payload = http.getString();
    http.end();

    JsonDocument doc;
    if (deserializeJson(doc, payload))
    {
        dashLog("[AUTO-OTA] JSON parse error");
        return;
    }

    JsonObject release;
    if (updateBetaChannel)
    {
        JsonArray arr = doc.as<JsonArray>();
        for (JsonObject r : arr)
        {
            release = r;
            break;
        }
    }
    else
        release = doc.as<JsonObject>();

    if (release.isNull())
    {
        dashLog("[AUTO-OTA] No release found");
        return;
    }

    String tagName = release["tag_name"] | "";
    String version = tagName;
    if (version.startsWith("v"))
        version = version.substring(1);
    if (!isVersionNewer(version, String(FIRMWARE_VERSION)))
    {
        dashLog("[AUTO-OTA] No newer release (latest=" + version + ", current=" FIRMWARE_VERSION ")");
        return;
    }

    const char *artifact = getFirmwareArtifact();
    String downloadUrl = "";
    for (JsonObject asset : release["assets"].as<JsonArray>())
    {
        String name = asset["name"] | "";
        if (name == artifact)
        {
            downloadUrl = String(asset["browser_download_url"] | "");
            break;
        }
    }
    if (!downloadUrl.length())
    {
        dashLog("[AUTO-OTA] No matching artifact for this build");
        return;
    }

    dashLog("[AUTO-OTA] Update " + version + " available. Installing...");

    HTTPClient http2;
    http2.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
    http2.begin(client, downloadUrl);
    http2.addHeader("Accept", "application/octet-stream");
    int code2 = http2.GET();
    if (code2 != 200)
    {
        dashLog("[AUTO-OTA] Download failed: HTTP " + String(code2));
        http2.end();
        return;
    }
    int len = http2.getSize();
    if (len <= 0)
    {
        dashLog("[AUTO-OTA] Invalid content length: " + String(len));
        http2.end();
        return;
    }
    if (!Update.begin(len))
    {
        dashLog("[AUTO-OTA] Update.begin failed: " + String(Update.errorString()));
        http2.end();
        return;
    }
    WiFiClient *stream = http2.getStreamPtr();
    size_t written = Update.writeStream(*stream);
    http2.end();
    if (written != (size_t)len)
    {
        dashLog("[AUTO-OTA] Written " + String(written) + "/" + String(len) + " bytes: " + String(Update.errorString()));
        Update.abort();
        return;
    }
    if (!Update.end(true))
    {
        dashLog("[AUTO-OTA] Finalize failed: " + String(Update.errorString()));
        return;
    }
    dashLog("[AUTO-OTA] Update successful! Rebooting...");
    delay(1000);
    ESP.restart();
}

static void handleAutoUpdate()
{
    if (server.hasArg("enabled"))
    {
        autoUpdateEnabled = server.arg("enabled") == "1";
        prefs.begin(PREFS_NS, false);
        prefs.putBool("auto_upd", autoUpdateEnabled);
        prefs.end();
        dashLog("[AUTO-OTA] " + String(autoUpdateEnabled ? "enabled" : "disabled"));
    }
    String j = "{\"ok\":true,\"enabled\":";
    j += autoUpdateEnabled ? "true" : "false";
    j += "}";
    server.send(200, "application/json", j);
}

static void handleUpdateBeta()
{
    if (server.hasArg("beta"))
    {
        updateBetaChannel = server.arg("beta") == "1";
        prefs.begin(PREFS_NS, false);
        prefs.putBool("update_beta", updateBetaChannel);
        prefs.end();
        dashLog("[OTA] Channel: " + String(updateBetaChannel ? "beta" : "stable"));
    }
    String j = "{\"ok\":true,\"beta\":" + String(updateBetaChannel ? "true" : "false");
    j += ",\"version\":\"" + jsonEscape(FIRMWARE_VERSION) + "\"}";
    server.send(200, "application/json", j);
}

static void handleEspNowStatus()
{
    const EspNowCanDataPkt &d = espnowCurData;
    String j = "{";
    j += "\"scanning\":" + String(espnowScanning ? "true" : "false");
    j += ",\"broadcasting\":" + String(espnowCanBroadcastEnabled ? "true" : "false");
    j += ",\"initialized\":" + String(espnowInitialized ? "true" : "false");
    j += ",\"discovered\":[";
    for (int i = 0; i < espnowDiscoveredCount; i++)
    {
        if (i)
            j += ",";
        j += "{\"mac\":\"" + espnowGetDiscoveredMac(i) + "\"";
        j += ",\"rssi\":" + String(espnowGetDiscoveredRssi(i));
        j += ",\"paired\":" + String(espnowGetDiscoveredPaired(i) ? "true" : "false");
        j += "}";
    }
    j += "]";
    if (espnowHasPaired)
    {
        char macStr[18];
        espnowMacToStr(espnowPairedMac, macStr);
        j += ",\"paired\":{\"mac\":\"" + String(macStr) + "\"}";
    }
    else
        j += ",\"paired\":null";
    j += ",\"scenarioRunning\":" + String(espnowScenarioActive ? "true" : "false");
    j += ",\"scenarioStep\":" + String(espnowScenarioStep);

    // Signal values
    j += ",\"speed\":" + String(d.vehicleSpeed);
    j += ",\"standstill\":" + String(d.standstill);
    j += ",\"gear\":" + String(d.gear);
    j += ",\"accel\":" + String(d.accelPedalPos);
    j += ",\"regen\":" + String(d.regenLight);
    j += ",\"steeringAngle\":" + String(d.steeringAngle);
    j += ",\"steeringSpeed\":" + String(d.steeringAngleSpeed);
    j += ",\"buckle\":" + String(d.buckleStatus);
    j += ",\"leftBlink\":" + String(d.leftBlinkerBlinking);
    j += ",\"rightBlink\":" + String(d.rightBlinkerBlinking);
    j += ",\"doorOpen\":" + String(d.anyDoorOpen);
    j += ",\"highBeam\":" + String(d.uiHighBeam);
    j += ",\"ldw\":" + String(d.laneDepartureWarning);
    j += ",\"scw\":" + String(d.sideCollisionWarning);
    j += ",\"fcw\":" + String(d.forwardCollisionWarning);
    j += ",\"ssw\":" + String(d.suppressSpeedWarning);
    j += ",\"bsr\":" + String(d.blindSpotRearRight);
    j += ",\"bsl\":" + String(d.blindSpotRearLeft);
    j += ",\"brakeRod\":" + String(d.brakeRodTravel);
    j += ",\"brakeApply\":" + String(d.driverBrakeApply);
    j += ",\"turnL\":" + String(d.turnSignalLeftStatus);
    j += ",\"turnR\":" + String(d.turnSignalRightStatus);
    j += "}";
    server.send(200, "application/json", j);
}

static void handleEspNowScan()
{
    if (server.hasArg("action"))
    {
        String action = server.arg("action");
        if (action == "start")
        {
            espnowScanning = true;
            espnowDiscoveredCount = 0;
            dashLog("[ESPNOW] Scanning started");
        }
        else if (action == "stop")
        {
            espnowScanning = false;
            dashLog("[ESPNOW] Scanning stopped");
        }
    }
    server.send(200, "application/json", "{\"ok\":true,\"scanning\":" + String(espnowScanning ? "true" : "false") + "}");
}

static void handleEspNowPair()
{
    if (!server.hasArg("mac"))
    {
        server.send(400, "application/json", "{\"ok\":false,\"error\":\"MAC required\"}");
        return;
    }
    String macStr = server.arg("mac");
    uint8_t mac[6];
    if (sscanf(macStr.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
               &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]) != 6)
    {
        server.send(400, "application/json", "{\"ok\":false,\"error\":\"Invalid MAC format\"}");
        return;
    }
    if (espnowPairDevice(mac))
    {
        dashLog("[ESPNOW] Paired with " + macStr);
        espnowSendTestPackets();
        server.send(200, "application/json", "{\"ok\":true}");
    }
    else
        server.send(400, "application/json", "{\"ok\":false,\"error\":\"Device not found\"}");
}

static void handleEspNowUnpair()
{
    espnowUnpairDevice();
    dashLog("[ESPNOW] Unpaired");
    server.send(200, "application/json", "{\"ok\":true}");
}

static void webTask(void *)
{
    for (;;)
    {
        ArduinoOTA.handle();
        server.handleClient();
        dashCheckWifi();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

static void mcpDashboardSetup(CarManagerBase *handler, CanDriver *driver
#if defined(DRIVER_ESP32_EXT_MCP2515)
                              ,
                              MCP2515 *mcp
#endif
)
{
    dashHandler = handler;
    dashDriver = driver;
#if defined(DRIVER_ESP32_EXT_MCP2515)
    dashMcp = mcp;
#endif
    startMs = millis();
    fpsLastMs = millis();

    if (!SPIFFS.begin(true))
        dashLog("[WARN] SPIFFS mount failed");

    dashLoadPrefs();
    dashStartAccessPoint(true);
    if (apHidden)
        dashLog("[WIFI] AP SSID is hidden");
    Serial.printf("[WIFI] AP: %s  IP: %s\n", apSSID, WiFi.softAPIP().toString().c_str());

    vTaskDelay(pdMS_TO_TICKS(200));
    espnowInit();

    // Set up the handler's onFrame callback
    handler->onFrame = mcpDashOnFrame;

    // Set CAN filters to only our 7 signals
    if (dashDriver)
        dashDriver->setFilters(kDashboardSniffIds, kDashboardSniffIdCount);

    dashApplyFilters();

    // Clean out the old handler pool approach — single handler only
    ArduinoOTA.setHostname("ev-open-can-tools");
    ArduinoOTA.setPassword(DASH_OTA_PASS);
    ArduinoOTA.onStart([]()
                       { dashLog("[OTA] Starting..."); });
    ArduinoOTA.onEnd([]()
                     { dashLog("[OTA] Done -- rebooting"); });
    ArduinoOTA.onError([](ota_error_t e)
                       { dashLog("[OTA] Error: " + String(e)); });
    ArduinoOTA.begin();

    server.on("/", HTTP_GET, handleRoot);
    server.on("/status", HTTP_GET, handleStatus);
    server.on("/config", HTTP_POST, handleConfig);
    server.on("/logging", HTTP_POST, handleLoggingConfig);
    server.on("/frames", HTTP_GET, handleFrames);
    server.on("/log", HTTP_GET, handleLog);
    server.on("/reset_stats", HTTP_POST, handleResetStats);
    server.on("/can_ids", HTTP_GET, handleCanIds);
    server.on("/can_ids_reset", HTTP_POST, handleCanIdsReset);
    server.on("/rec_start", HTTP_POST, handleRecStart);
    server.on("/rec_stop", HTTP_POST, handleRecStop);
    server.on("/rec_status", HTTP_GET, handleRecStatus);
    server.on("/rec_download", HTTP_GET, handleRecDownload);
    server.on("/reboot", HTTP_POST, handleReboot);
    server.on("/update", HTTP_POST, handleOtaResult, handleOtaUpload);
    server.on("/ap_config", HTTP_POST, handleApConfig);
    server.on("/ap_status", HTTP_GET, handleApStatus);
    server.on("/can_pins", HTTP_GET, handleCanPins);
    server.on("/can_pins", HTTP_POST, handleCanPinsSave);
    server.on("/wifi_scan", HTTP_GET, handleWifiScan);
    server.on("/wifi_config", HTTP_POST, handleWifiConfig);
    server.on("/wifi_status", HTTP_GET, handleWifiStatus);
    server.on("/wifi_networks", HTTP_GET, handleWifiNetworks);
    server.on("/wifi_delete", HTTP_POST, handleWifiDelete);
    server.on("/update_check", HTTP_GET, handleUpdateCheck);
    server.on("/update_install", HTTP_POST, handleUpdateInstall);
    server.on("/update_beta", HTTP_POST, handleUpdateBeta);
    server.on("/auto_update", HTTP_GET, handleAutoUpdate);
    server.on("/auto_update", HTTP_POST, handleAutoUpdate);

    server.on("/espnow_status", HTTP_GET, handleEspNowStatus);
    server.on("/espnow_scan", HTTP_POST, handleEspNowScan);
    server.on("/espnow_pair", HTTP_POST, handleEspNowPair);
    server.on("/espnow_unpair", HTTP_POST, handleEspNowUnpair);
    server.on("/espnow_test", HTTP_POST, []()
              {
        espnowSendTestPackets();
        server.send(200, "application/json", "{\"ok\":true}"); });
    server.on("/espnow_scenario", HTTP_POST, []()
              {
        espnowStartScenario();
        server.send(200, "application/json", "{\"ok\":true}"); });
    server.on("/espnow_debug", HTTP_GET, []()
              {
        String r = "espnowInitialized=" + String(espnowInitialized ? "true" : "false") + "\n";
        r += "espnowScanning=" + String(espnowScanning ? "true" : "false") + "\n";
        r += "espnowHasPaired=" + String(espnowHasPaired ? "true" : "false") + "\n";
        r += "espnowDiscoveredCount=" + String(espnowDiscoveredCount) + "\n";
        r += "espnowCanBroadcastEnabled=" + String(espnowCanBroadcastEnabled ? "true" : "false") + "\n";
        server.send(200, "text/plain", r); });

    server.begin();
    if (strlen(staSSID) > 0)
        dashScheduleSTAConnect(kDashStaBootDelayMs);
#if CONFIG_FREERTOS_UNICORE
    xTaskCreate(webTask, "web", 8192, nullptr, 1, nullptr);
#else
    xTaskCreatePinnedToCore(webTask, "web", 8192, nullptr, 1, nullptr, 1);
#endif
    Serial.println("[WEB] Dashboard: http://" + WiFi.softAPIP().toString());
    dashLog("[BOOT] ev-open-can-tools ready");
}

static void mcpDashboardLoop()
{
    if (Update.isRunning())
        return;
    dashCheckBusHealth();
    if (canOnline && millis() - lastFrameMs > 10000)
    {
        canOnline = false;
        dashLog("[CAN] Bus OFFLINE (timeout)");
    }
    espnowBroadcastCanData();
}

#endif
