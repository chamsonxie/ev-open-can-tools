#pragma once
// WiFi uplink bridge (NAT/NAPT) + DNS proxy with allow/blocklist for ESP32
// dashboard builds. Included from mcp2515_dashboard.h inside the
// ESP32_DASHBOARD guard. All symbols are file-static.
//
// Depends on the surrounding dashboard translation unit for:
//   - Preferences prefs        (open inside dashLoadPrefs while we read prefs)
//   - WebServer server         (HTTP route handlers)
//   - char apSSID[]/apPass[]   (AP credentials, for status display)
//   - char staSSID[]/staPass[] (STA credentials)
//   - bool staConnected        (current STA link state)
//   - dashLog(), jsonEscape()  (logging + JSON helpers)

// The NAPT header ships with arduino-esp32 even when NAPT is not compiled in.
// `IP_NAPT` (from lwip/opt.h via lwip_napt.h) is the real gate that decides
// whether ip_napt_enable() is declared *and* linked. Treat it as the single
// source of truth, otherwise the link step fails on the default SDK build.
#if __has_include("lwip/lwip_napt.h")
#  include "lwip/lwip_napt.h"
#endif
#if defined(IP_NAPT) && IP_NAPT
#  define BRIDGE_NAPT_AVAILABLE 1
#else
#  define BRIDGE_NAPT_AVAILABLE 0
#endif

#if __has_include(<esp_netif.h>)
#  include <esp_netif.h>
#  define BRIDGE_HAVE_ESP_NETIF 1
#else
#  define BRIDGE_HAVE_ESP_NETIF 0
#endif

#include <WiFiUdp.h>
#include <ArduinoJson.h>

// ─── Limits ───────────────────────────────────────────────────────────────────

static constexpr int      kBridgeDnsRulesMax     = 50;
static constexpr int      kBridgeDnsRuleLen      = 64;
static constexpr size_t   kBridgeDnsRulesJsonMax = 3800;
static constexpr size_t   kBridgeDnsBufSize      = 512;
static constexpr uint32_t kBridgeDnsUpstreamMs   = 3000;
static constexpr uint32_t kBridgeDnsTaskStack    = 4096;

// ─── State ────────────────────────────────────────────────────────────────────

static bool     bridgeEnabled       = false;
static bool     bridgeNaptActive    = false;
static bool     bridgeBootApplied   = false;

static bool     dnsFilterEnabled    = false;
static uint8_t  dnsFilterMode       = 0; // 0 = blocklist, 1 = allowlist
static String   dnsRulesJson        = "[]";

static char     dnsRules[kBridgeDnsRulesMax][kBridgeDnsRuleLen];
static int      dnsRuleCount        = 0;

static TaskHandle_t      dnsTaskHandle    = nullptr;
static volatile bool     dnsTaskShouldRun = false;
static volatile uint32_t dnsQueriesTotal  = 0;
static volatile uint32_t dnsQueriesBlocked = 0;
static volatile uint32_t dnsQueriesFailed  = 0;

// ─── Rule helpers ─────────────────────────────────────────────────────────────

static void bridgeParseRules()
{
    dnsRuleCount = 0;
    JsonDocument doc;
    if (deserializeJson(doc, dnsRulesJson) != DeserializationError::Ok)
        return;
    if (!doc.is<JsonArray>())
        return;
    for (JsonVariant v : doc.as<JsonArray>())
    {
        if (dnsRuleCount >= kBridgeDnsRulesMax)
            break;
        const char *s = v.as<const char *>();
        if (!s || s[0] == '\0')
            continue;
        strlcpy(dnsRules[dnsRuleCount], s, kBridgeDnsRuleLen);
        for (char *p = dnsRules[dnsRuleCount]; *p; ++p)
            *p = (char)tolower((unsigned char)*p);
        ++dnsRuleCount;
    }
}

static void bridgeExtractQName(const uint8_t *pkt, int len, char *out, size_t outSize)
{
    int pos = 12, outPos = 0;
    while (pos < len && pkt[pos] != 0 && outPos < (int)outSize - 1)
    {
        uint8_t llen = pkt[pos++];
        if (llen > 63 || pos + llen > len)
            break;
        if (outPos > 0)
            out[outPos++] = '.';
        for (int i = 0; i < llen && outPos < (int)outSize - 1; i++)
            out[outPos++] = (char)tolower((unsigned char)pkt[pos++]);
    }
    out[outPos] = '\0';
}

static bool bridgeDomainMatches(const char *query, const char *rule)
{
    // Wildcard prefix: "*.example.com" matches "example.com" and any subdomain.
    if (rule[0] == '*' && rule[1] == '.')
    {
        const char *suffix = rule + 1;          // ".example.com"
        size_t qlen = strlen(query), slen = strlen(suffix);
        if (qlen >= slen && strcmp(query + qlen - slen, suffix) == 0)
            return true;
        if (strcmp(query, suffix + 1) == 0)     // bare apex match
            return true;
        return false;
    }
    return strcmp(query, rule) == 0;
}

static bool bridgeShouldBlock(const char *qname)
{
    if (dnsRuleCount == 0)
        return dnsFilterMode == 1; // allowlist with no rules ⇒ block all
    bool matched = false;
    for (int i = 0; i < dnsRuleCount; ++i)
    {
        if (bridgeDomainMatches(qname, dnsRules[i]))
        {
            matched = true;
            break;
        }
    }
    return dnsFilterMode == 0 ? matched : !matched;
}

static int bridgeBuildNxdomain(const uint8_t *query, int qlen, uint8_t *reply, int replySize)
{
    if (qlen < 12 || qlen > replySize)
        return -1;
    memcpy(reply, query, qlen);
    reply[2] = 0x81;                    // QR=1 RD=1
    reply[3] = 0x83;                    // RA=1 RCODE=3 (NXDOMAIN)
    reply[6] = reply[7] = 0;            // ANCOUNT
    reply[8] = reply[9] = 0;            // NSCOUNT
    reply[10] = reply[11] = 0;          // ARCOUNT
    return qlen;
}

static IPAddress bridgeUpstreamDns()
{
    if (staConnected)
    {
        IPAddress d = WiFi.dnsIP(0);
        if ((uint32_t)d != 0)
            return d;
    }
    return IPAddress(8, 8, 8, 8);
}

// ─── DHCP DNS option (CRUCIAL: makes AP clients query our local proxy) ────────
//
// By default the ESP32 softAP DHCP server does NOT advertise a DNS server in
// its DHCP offers. AP clients then either get no DNS at all or fall back to
// their last-known/manual DNS — which means our DNS proxy on AP_IP:53 is
// silently bypassed and the allow/blocklist has no effect.
//
// Pushing option 6 (Domain Name Server) pointing at the AP IP forces all
// AP-side DNS resolution through our proxy, which is the only point where
// we can apply the filter cheaply without a full transparent redirect.

static void bridgeSetDhcpDns(bool enable)
{
#if BRIDGE_HAVE_ESP_NETIF
    esp_netif_t *ap = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");
    if (!ap)
        return;

    // Stop the DHCP server before mutating options (ESP-IDF requires this).
    esp_netif_dhcps_stop(ap);

    if (enable)
    {
        IPAddress apIp = WiFi.softAPIP();
        esp_netif_dns_info_t dns = {};
        dns.ip.type = ESP_IPADDR_TYPE_V4;
        dns.ip.u_addr.ip4.addr = (uint32_t)apIp;
        esp_netif_set_dns_info(ap, ESP_NETIF_DNS_MAIN, &dns);

        dhcps_offer_t offer = OFFER_DNS;
        esp_netif_dhcps_option(ap, ESP_NETIF_OP_SET,
                               ESP_NETIF_DOMAIN_NAME_SERVER,
                               &offer, sizeof(offer));
    }
    else
    {
        dhcps_offer_t offer = 0;
        esp_netif_dhcps_option(ap, ESP_NETIF_OP_SET,
                               ESP_NETIF_DOMAIN_NAME_SERVER,
                               &offer, sizeof(offer));
    }

    esp_netif_dhcps_start(ap);
#else
    (void)enable;
#endif
}

// ─── DNS proxy task ───────────────────────────────────────────────────────────

static void bridgeDnsTask(void *)
{
    WiFiUDP udp;
    IPAddress apIp = WiFi.softAPIP();

    // AP IP can briefly be 0.0.0.0 right after softAP() returns; wait it out.
    for (int i = 0; i < 50 && (uint32_t)apIp == 0; ++i)
    {
        vTaskDelay(pdMS_TO_TICKS(20));
        apIp = WiFi.softAPIP();
    }

    if (!udp.begin(apIp, 53))
    {
        dashLog("[BRIDGE] DNS proxy bind failed on " + apIp.toString());
        dnsTaskHandle = nullptr;
        vTaskDelete(nullptr);
        return;
    }
    dashLog("[BRIDGE] DNS proxy on " + apIp.toString() + ":53");

    uint8_t buf[kBridgeDnsBufSize];
    while (dnsTaskShouldRun)
    {
        int pktSize = udp.parsePacket();
        if (pktSize <= 0)
        {
            vTaskDelay(pdMS_TO_TICKS(5));
            continue;
        }
        if (pktSize > (int)kBridgeDnsBufSize)
        {
            udp.flush();
            continue;
        }

        IPAddress clientIp   = udp.remoteIP();
        uint16_t  clientPort = udp.remotePort();
        int n = udp.read(buf, kBridgeDnsBufSize);
        if (n < 12)
            continue;

        char qname[256];
        bridgeExtractQName(buf, n, qname, sizeof(qname));

        ++dnsQueriesTotal;

        if (dnsFilterEnabled && bridgeShouldBlock(qname))
        {
            ++dnsQueriesBlocked;
            uint8_t reply[kBridgeDnsBufSize];
            int rlen = bridgeBuildNxdomain(buf, n, reply, sizeof(reply));
            if (rlen > 0)
            {
                udp.beginPacket(clientIp, clientPort);
                udp.write(reply, rlen);
                udp.endPacket();
            }
            continue;
        }

        // Forward to upstream resolver and relay the answer back.
        IPAddress upstream = bridgeUpstreamDns();
        WiFiUDP fwd;
        if (!fwd.begin(0))
        {
            ++dnsQueriesFailed;
            continue;
        }
        fwd.beginPacket(upstream, 53);
        fwd.write(buf, n);
        fwd.endPacket();

        unsigned long t0 = millis();
        int rn = 0;
        uint8_t rbuf[kBridgeDnsBufSize];
        while (millis() - t0 < kBridgeDnsUpstreamMs)
        {
            rn = fwd.parsePacket();
            if (rn > 0)
                break;
            vTaskDelay(pdMS_TO_TICKS(5));
        }
        if (rn > 0 && rn <= (int)kBridgeDnsBufSize)
        {
            fwd.read(rbuf, rn);
            udp.beginPacket(clientIp, clientPort);
            udp.write(rbuf, rn);
            udp.endPacket();
        }
        else
        {
            ++dnsQueriesFailed;
        }
        fwd.stop();
    }

    udp.stop();
    dnsTaskHandle = nullptr;
    vTaskDelete(nullptr);
}

static void bridgeDnsStart()
{
    if (dnsTaskHandle != nullptr)
        return;
    dnsTaskShouldRun = true;
    BaseType_t ok = xTaskCreatePinnedToCore(bridgeDnsTask, "dns_proxy",
                                            kBridgeDnsTaskStack, nullptr, 1,
                                            &dnsTaskHandle, 0);
    if (ok != pdPASS)
    {
        dnsTaskShouldRun = false;
        dnsTaskHandle = nullptr;
        dashLog("[BRIDGE] DNS task create failed");
    }
}

static void bridgeDnsStop()
{
    if (dnsTaskHandle == nullptr)
        return;
    dnsTaskShouldRun = false;          // task self-deletes on next loop
    // Counters are kept; UI consumer decides when to ignore stale values.
    dashLog("[BRIDGE] DNS proxy stopping");
}

// ─── NAPT (NAT) bridge ────────────────────────────────────────────────────────

static bool bridgeNaptEnable()
{
#if !BRIDGE_NAPT_AVAILABLE
    dashLog("[BRIDGE] NAPT unavailable in this SDK build");
    return false;
#else
    if (bridgeNaptActive)
        return true;
    uint32_t apIp = (uint32_t)WiFi.softAPIP();
    if (apIp == 0)
    {
        dashLog("[BRIDGE] AP IP not ready, NAPT deferred");
        return false;
    }
    esp_err_t ret = ip_napt_enable(apIp, 1);
    if (ret == ESP_OK)
    {
        bridgeNaptActive = true;
        dashLog("[BRIDGE] NAT on  AP=" + WiFi.softAPIP().toString()
                + "  uplink=" + WiFi.localIP().toString());
        return true;
    }
    dashLog("[BRIDGE] ip_napt_enable failed err=" + String(ret));
    return false;
#endif
}

static void bridgeNaptDisable()
{
#if BRIDGE_NAPT_AVAILABLE
    if (!bridgeNaptActive)
        return;
    uint32_t apIp = (uint32_t)WiFi.softAPIP();
    if (apIp != 0)
        ip_napt_enable(apIp, 0);
    bridgeNaptActive = false;
    dashLog("[BRIDGE] NAT off");
#endif
}

// Reconciles requested vs. actual NAPT and DNS-proxy state. Idempotent —
// safe to call repeatedly (boot, STA up, STA down, config save).
static void bridgeApplyState()
{
    bool napShould = bridgeEnabled && staConnected;
    if (napShould && !bridgeNaptActive)
        bridgeNaptEnable();
    else if (!napShould && bridgeNaptActive)
        bridgeNaptDisable();

    bool dnsShould = dnsFilterEnabled;
    if (dnsShould && dnsTaskHandle == nullptr)
    {
        bridgeSetDhcpDns(true);        // crucial: redirect AP clients to us
        bridgeDnsStart();
    }
    else if (!dnsShould && dnsTaskHandle != nullptr)
    {
        bridgeSetDhcpDns(false);
        bridgeDnsStop();
    }
}

// One-shot apply at boot, after the AP is up (called from mcpDashboardSetup).
// Separate from bridgeApplyState so we only emit the boot log line once.
static void bridgeBootApply()
{
    if (bridgeBootApplied)
        return;
    bridgeBootApplied = true;
    dashLog(String("[BRIDGE] boot bridge=") + (bridgeEnabled ? "ON" : "OFF")
            + " dns=" + (dnsFilterEnabled ? "ON" : "OFF")
            + " mode=" + (dnsFilterMode == 0 ? "block" : "allow")
            + " rules=" + String(dnsRuleCount));
    bridgeApplyState();
}

// ─── NVS persistence ──────────────────────────────────────────────────────────

// Called from dashLoadPrefs while the global `prefs` namespace is still open.
static void bridgeLoadPrefsInline()
{
    bridgeEnabled    = prefs.getBool("bridge_en", false);
    dnsFilterEnabled = prefs.getBool("dns_flt_en", false);
    dnsFilterMode    = prefs.getUChar("dns_mode", 0);
    if (dnsFilterMode > 1)
        dnsFilterMode = 0;
    dnsRulesJson = prefs.isKey("dns_rules")
                       ? prefs.getString("dns_rules", "[]")
                       : String("[]");
    bridgeParseRules();
}

static void bridgeSavePrefs()
{
    prefs.begin(PREFS_NS, false);
    prefs.putBool("bridge_en", bridgeEnabled);
    prefs.putBool("dns_flt_en", dnsFilterEnabled);
    prefs.putUChar("dns_mode", dnsFilterMode);
    prefs.putString("dns_rules", dnsRulesJson);
    prefs.end();
}

// ─── HTTP handlers ────────────────────────────────────────────────────────────

static void handleBridgeStatus()
{
    String j = "{";
    j += "\"bridge_en\":";       j += (bridgeEnabled ? "true" : "false");
    j += ",\"napt_active\":";    j += (bridgeNaptActive ? "true" : "false");
    j += ",\"napt_supported\":"; j += (BRIDGE_NAPT_AVAILABLE ? "true" : "false");
    j += ",\"dns_flt_en\":";     j += (dnsFilterEnabled ? "true" : "false");
    j += ",\"dns_active\":";     j += (dnsTaskHandle != nullptr ? "true" : "false");
    j += ",\"dns_mode\":";       j += String(dnsFilterMode);
    j += ",\"dns_total\":";      j += String(dnsQueriesTotal);
    j += ",\"dns_blocked\":";    j += String(dnsQueriesBlocked);
    j += ",\"dns_failed\":";     j += String(dnsQueriesFailed);
    j += ",\"sta_connected\":";  j += (staConnected ? "true" : "false");
    j += ",\"sta_ssid\":\"";     j += jsonEscape(staSSID); j += "\"";
    if (staConnected)
        j += ",\"sta_ip\":\"" + WiFi.localIP().toString() + "\"";
    j += ",\"ap_clients\":";     j += String(WiFi.softAPgetStationNum());
    j += ",\"ap_ip\":\"";        j += WiFi.softAPIP().toString(); j += "\"";
    // Reason that bridge isn't currently forwarding (UI hint).
    const char *reason = "";
    if (!bridgeEnabled)             reason = "disabled";
    else if (!BRIDGE_NAPT_AVAILABLE) reason = "unsupported";
    else if (!staConnected)         reason = "no_uplink";
    else if (!bridgeNaptActive)     reason = "starting";
    j += ",\"bridge_reason\":\""; j += reason; j += "\"";
    j += ",\"dns_rules\":";      j += dnsRulesJson; // raw JSON array
    j += "}";
    server.send(200, "application/json", j);
}

static void handleBridgeConfig()
{
    bool changed = false;

    if (server.hasArg("bridge_en"))
    {
        bool v = (server.arg("bridge_en") == "1");
        if (v != bridgeEnabled) { bridgeEnabled = v; changed = true; }
    }
    if (server.hasArg("dns_flt_en"))
    {
        bool v = (server.arg("dns_flt_en") == "1");
        if (v != dnsFilterEnabled) { dnsFilterEnabled = v; changed = true; }
    }
    if (server.hasArg("dns_mode"))
    {
        uint8_t v = (uint8_t)(server.arg("dns_mode").toInt() == 1 ? 1 : 0);
        if (v != dnsFilterMode) { dnsFilterMode = v; changed = true; }
    }
    if (server.hasArg("dns_rules"))
    {
        String rules = server.arg("dns_rules");
        if (rules.length() > kBridgeDnsRulesJsonMax)
        {
            server.send(400, "application/json",
                        "{\"ok\":false,\"error\":\"Rules list too long\"}");
            return;
        }
        JsonDocument doc;
        if (deserializeJson(doc, rules) != DeserializationError::Ok ||
            !doc.is<JsonArray>())
        {
            server.send(400, "application/json",
                        "{\"ok\":false,\"error\":\"Invalid rules JSON\"}");
            return;
        }
        dnsRulesJson = rules;
        bridgeParseRules();
        changed = true;
    }

    if (changed)
    {
        bridgeSavePrefs();
        bridgeApplyState();
        dashLog("[BRIDGE] cfg bridge=" + String(bridgeEnabled ? "ON" : "OFF")
                + " dns=" + String(dnsFilterEnabled ? "ON" : "OFF")
                + " mode=" + String(dnsFilterMode == 0 ? "block" : "allow")
                + " rules=" + String(dnsRuleCount));
    }
    server.send(200, "application/json", "{\"ok\":true}");
}
