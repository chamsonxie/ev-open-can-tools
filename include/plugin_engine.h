#pragma once

#if defined(ESP32_DASHBOARD) && (!defined(NATIVE_BUILD) || defined(PLUGIN_ENGINE_NATIVE_TEST))

#include <ArduinoJson.h>
#if defined(NATIVE_BUILD)
#include <string>
using String = std::string;
#elif defined(ESP_PLATFORM)
#include "platform/espidf_runtime.h"
#else
#include <SPIFFS.h>
#endif
#include "can_frame_types.h"
#include "can_helpers.h"
#include "drivers/can_driver.h"

#define PLUGIN_MAX 8
#define PLUGIN_RULES_MAX 16
#define PLUGIN_OPS_MAX 16
#ifndef PLUGIN_FILTER_IDS_MAX
#define PLUGIN_FILTER_IDS_MAX 32
#endif
#ifndef PLUGIN_REPLAY_COUNT
#define PLUGIN_REPLAY_COUNT 1
#endif
#ifndef PLUGIN_REPLAY_COUNT_MAX
#define PLUGIN_REPLAY_COUNT_MAX 20
#endif
#define PLUGIN_PERIODIC_INTERVAL_DEFAULT_MS 100
#define PLUGIN_PERIODIC_INTERVAL_MIN_MS 10
#define PLUGIN_PERIODIC_INTERVAL_MAX_MS 5000
#define PLUGIN_GTW_UDS_REQUEST_ID 0x684
#define PLUGIN_GTW_UDS_RESPONSE_ID 0x685
#define PLUGIN_GTW_UDS_KEEPALIVE_MS 2000 // 会话激活后的TesterPresent节奏
#define PLUGIN_GTW_UDS_RESPONSE_TIMEOUT_MS 400
#define PLUGIN_GTW_UDS_RETRY_BACKOFF_MS 5000 // 在NRC或超时后，等待一段时间再重试序列
#define PLUGIN_GTW_UDS_SEED_MAX 6

enum PluginOpType : uint8_t
{
    OP_SET_BIT = 0,
    OP_SET_BYTE = 1,
    OP_OR_BYTE = 2,
    OP_AND_BYTE = 3,
    OP_CHECKSUM = 4,
    OP_COUNTER = 5,
    OP_EMIT_PERIODIC = 6,
};

struct PluginOp
{
    PluginOpType type;
    uint8_t index; // bit (0-63) or byte (0-7) index
    uint8_t value;
    uint8_t mask; // for SET_BYTE
    uint16_t intervalMs;
    bool gtwSilent;
};

enum PluginGtwUdsState : uint8_t
{
    GTW_UDS_IDLE = 0,           // 无静默尝试进行中
    GTW_UDS_SESSION_REQ = 1,    // 已发送0x10 0x03，等待肯定响应
    GTW_UDS_SEED_REQ = 2,       // 已发送0x27 0x01，等待种子
    GTW_UDS_KEY_SENT = 3,       // 已发送0x27 0x02，等待肯定响应
    GTW_UDS_COMM_CTRL_SENT = 4, // 已发送0x28 0x01 0x01，等待肯定响应
    GTW_UDS_ACTIVE = 5,         // 静默已激活；周期性0x3E TesterPresent保持其存活
    GTW_UDS_FAILED = 6,         // NRC/超时 — 在重试前退避
};

struct PluginGtwUdsMachine
{
    PluginGtwUdsState state;
    unsigned long stateEnteredAt;
    unsigned long nextActionAt;
    unsigned long retryAfterMs;
    uint8_t lastNrc;
    uint8_t seed[PLUGIN_GTW_UDS_SEED_MAX];
    uint8_t seedLen;
    uint8_t bus;
    uint8_t lastSeed[PLUGIN_GTW_UDS_SEED_MAX];
    uint8_t lastSeedLen;
    uint8_t lastKey[PLUGIN_GTW_UDS_SEED_MAX];
    uint8_t lastKeyLen;
};

struct PluginPeriodicEmitState
{
    bool active;
    bool gtwSilent;
    bool checksum;
    CanFrame frame;
    PluginOp counterOps[PLUGIN_OPS_MAX];
    uint8_t counterOpCount;
    uint16_t intervalMs;
    unsigned long nextFrameAt;
    PluginGtwUdsMachine uds;
};

struct PluginRuleDiagnostics
{
    uint32_t matchCount;
    uint32_t changedCount;
    uint32_t sendOkCount;
    uint32_t sendFailCount;
    unsigned long lastMatchMs;
    unsigned long lastSendMs;
    uint8_t lastOriginal[8];
    uint8_t lastModified[8];
};

struct PluginRule
{
    uint32_t canId;
    int16_t mux; // -1 = match any mux
    uint8_t muxMask;
    uint8_t matchByte;
    uint8_t matchMask; // 0 = no byte match
    uint8_t matchValue;
    uint8_t busMask;
    PluginOp ops[PLUGIN_OPS_MAX];
    uint8_t opCount;
    bool sendAfter;
    PluginRuleDiagnostics diag;
};

struct PluginData
{
    char name[32];
    char version[16];
    char author[32];
    char filename[32];
    char sourceUrl[200];
    bool enabled;
    uint8_t priority;
    PluginRule rules[PLUGIN_RULES_MAX];
    uint8_t ruleCount;
    uint32_t filterIds[PLUGIN_FILTER_IDS_MAX];
    uint8_t filterIdCount;
};

static PluginData pluginStore[PLUGIN_MAX];
static uint8_t pluginCount = 0;
static volatile bool pluginsLocked = false;
static PluginPeriodicEmitState pluginPeriodicEmit = {};
static bool (*pluginBeforeSend)(CanFrame &modified, const CanFrame &original) = nullptr;
static void (*pluginDiagnosticsLog)(const char *message) = nullptr;

static uint8_t pluginClampReplayCount(int32_t count)
{
    if (count < 1)
        return 1;
    if (count > PLUGIN_REPLAY_COUNT_MAX)
        return PLUGIN_REPLAY_COUNT_MAX;
    return static_cast<uint8_t>(count);
}

static uint8_t pluginReplayCount = pluginClampReplayCount(PLUGIN_REPLAY_COUNT);

static void pluginResetDiagnostics();

static void pluginSetReplayCount(int32_t count)
{
    pluginReplayCount = pluginClampReplayCount(count);
}

static void pluginSetDiagnosticsLogger(void (*logger)(const char *message))
{
    pluginDiagnosticsLog = logger;
}

static void pluginLogDiagnosticsEvent(const char *message)
{
    if (pluginDiagnosticsLog)
        pluginDiagnosticsLog(message);
}

static uint8_t pluginGetReplayCount()
{
    return pluginReplayCount;
}

static uint16_t pluginClampPeriodicInterval(int32_t intervalMs)
{
    if (intervalMs < PLUGIN_PERIODIC_INTERVAL_MIN_MS)
        return PLUGIN_PERIODIC_INTERVAL_MIN_MS;
    if (intervalMs > PLUGIN_PERIODIC_INTERVAL_MAX_MS)
        return PLUGIN_PERIODIC_INTERVAL_MAX_MS;
    return static_cast<uint16_t>(intervalMs);
}

static void pluginResetPeriodicEmit()
{
    pluginPeriodicEmit = {};
}

static bool pluginGtwSilentSupported()
{
#if defined(PLUGIN_GTW_UDS_KEY_READY)
    return true;
#else
    return false;
#endif
}

// ── GTW UDS 静默密钥钩子 ──────────────────────────────────────────
//
// SecurityAccess密钥计算。Tesla使用专有的种子到密钥算法。
// 没有真正的算法，网关将响应NRC invalidKey，静默将不会生效。
//
// 将PLUGIN_GTW_UDS_KEY_READY定义为当前种子到密钥算法使用的字节。
// 没有它，gtw_silent在解析时被忽略。

// ── GTW UDS 状态机（已禁用 — 只读模式） ─────────────────────────

static void pluginGtwUdsTick(PluginGtwUdsMachine &, CanDriver &, unsigned long)
{
}

// ── JSON 解析 ────────────────────────────────────────────────────

static uint8_t pluginDefaultMuxMask(int16_t mux)
{
    if (mux < 0)
        return 0;
    return mux > 7 ? 0xFF : 0x07;
}

static bool pluginBusTokenEquals(const char *token, uint8_t len, const char *expected)
{
    for (uint8_t i = 0; i < len || expected[i] != '\0'; i++)
    {
        char a = i < len ? token[i] : '\0';
        if (a >= 'a' && a <= 'z')
            a = static_cast<char>(a - 'a' + 'A');
        char b = expected[i];
        if (a != b)
            return false;
    }
    return true;
}

static uint8_t pluginBusMaskForToken(const char *token, uint8_t len)
{
    if (len == 0 || pluginBusTokenEquals(token, len, "ANY"))
        return CAN_BUS_ANY;
    if (pluginBusTokenEquals(token, len, "CH"))
        return CAN_BUS_CH;
    if (pluginBusTokenEquals(token, len, "VEH"))
        return CAN_BUS_VEH;
    if (pluginBusTokenEquals(token, len, "PARTY"))
        return CAN_BUS_PARTY;
    return CAN_BUS_ANY;
}

static uint8_t pluginParseBusString(const char *bus)
{
    if (!bus)
        return CAN_BUS_ANY;

    uint8_t mask = CAN_BUS_ANY;
    const char *token = nullptr;
    uint8_t len = 0;
    for (const char *p = bus;; p++)
    {
        char c = *p;
        bool separator = c == '\0' || c == ',' || c == '|' || c == '+' || c == ' ';
        if (!separator)
        {
            if (!token)
                token = p;
            if (len < 16)
                len++;
            continue;
        }
        if (token)
        {
            mask |= pluginBusMaskForToken(token, len);
            token = nullptr;
            len = 0;
        }
        if (c == '\0')
            break;
    }
    return mask;
}

static uint8_t pluginParseBus(JsonVariant value)
{
    if (value.isNull())
        return CAN_BUS_ANY;
    if (value.is<uint8_t>())
        return value.as<uint8_t>() & (CAN_BUS_CH | CAN_BUS_VEH | CAN_BUS_PARTY);
    if (value.is<const char *>())
        return pluginParseBusString(value.as<const char *>());
    if (value.is<JsonArray>())
    {
        uint8_t mask = CAN_BUS_ANY;
        for (JsonVariant item : value.as<JsonArray>())
            mask |= pluginParseBus(item);
        return mask;
    }
    return CAN_BUS_ANY;
}

static bool pluginRuleMatchesBus(const PluginRule &rule, const CanFrame &frame)
{
    if (rule.busMask == CAN_BUS_ANY || frame.bus == CAN_BUS_ANY)
        return true;
    return (rule.busMask & frame.bus) != 0;
}

static bool pluginRuleMatchesMux(const PluginRule &rule, const CanFrame &frame)
{
    if (rule.mux < 0)
        return true;
    if (frame.dlc == 0)
        return false;
    uint8_t mask = rule.muxMask ? rule.muxMask : pluginDefaultMuxMask(rule.mux);
    return (frame.data[0] & mask) == (static_cast<uint8_t>(rule.mux) & mask);
}

static bool pluginRuleMatchesByte(const PluginRule &rule, const CanFrame &frame)
{
    if (rule.matchMask == 0)
        return true;
    if (rule.matchByte >= frame.dlc || rule.matchByte >= 8)
        return false;
    return (frame.data[rule.matchByte] & rule.matchMask) == (rule.matchValue & rule.matchMask);
}

static bool pluginRuleMuxIncludes(const PluginRule &rule, uint8_t mux)
{
    if (rule.mux < 0)
        return false;
    uint8_t mask = rule.muxMask ? rule.muxMask : pluginDefaultMuxMask(rule.mux);
    return (static_cast<uint8_t>(rule.mux) & mask) == (mux & mask);
}

static bool pluginParseJson(const String &json, PluginData &out)
{
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, json);
    if (err)
        return false;

    strlcpy(out.name, doc["name"] | "Unknown", sizeof(out.name));
    strlcpy(out.version, doc["version"] | "1.0", sizeof(out.version));
    strlcpy(out.author, doc["author"] | "", sizeof(out.author));
    out.filename[0] = '\0';
    out.sourceUrl[0] = '\0';
    out.enabled = true;
    out.priority = 0;
    out.ruleCount = 0;
    out.filterIdCount = 0;

    JsonArray rules = doc["rules"];
    if (!rules)
        return false;

    for (JsonObject rule : rules)
    {
        if (out.ruleCount >= PLUGIN_RULES_MAX)
            break;
        PluginRule &r = out.rules[out.ruleCount];
        r = {};
        r.canId = rule["id"] | (uint32_t)0;
        int mux = rule["mux"] | (int)-1;
        if (mux < -1)
            mux = -1;
        if (mux > 255)
            mux = 255;
        r.mux = static_cast<int16_t>(mux);
        JsonVariant muxMaskValue = rule["mux_mask"];
        if (muxMaskValue.isNull())
            muxMaskValue = rule["muxMask"];
        r.muxMask = muxMaskValue | pluginDefaultMuxMask(r.mux);
        if (r.mux >= 0 && r.muxMask == 0)
            r.muxMask = pluginDefaultMuxMask(r.mux);
        r.busMask = pluginParseBus(rule["bus"]);
        r.matchByte = 0;
        r.matchMask = 0;
        r.matchValue = 0;
        JsonVariant match = rule["match"];
        if (match.is<JsonObject>())
        {
            r.matchByte = match["byte"] | (uint8_t)0;
            r.matchMask = match["mask"] | (uint8_t)0xFF;
            JsonVariant matchValue = match["val"];
            if (matchValue.isNull())
                matchValue = match["value"];
            r.matchValue = matchValue | (uint8_t)0;
        }
        JsonVariant matchByte = rule["match_byte"];
        if (matchByte.isNull())
            matchByte = rule["matchByte"];
        JsonVariant matchMask = rule["match_mask"];
        if (matchMask.isNull())
            matchMask = rule["matchMask"];
        JsonVariant matchValue = rule["match_val"];
        if (matchValue.isNull())
            matchValue = rule["match_value"];
        if (matchValue.isNull())
            matchValue = rule["matchValue"];
        if (!matchByte.isNull())
            r.matchByte = matchByte | (uint8_t)0;
        if (!matchMask.isNull())
            r.matchMask = matchMask | (uint8_t)0;
        if (!matchValue.isNull())
            r.matchValue = matchValue | (uint8_t)0;
        if (r.matchByte > 7)
            r.matchMask = 0;
        r.sendAfter = rule["send"] | true;
        r.opCount = 0;

        JsonArray ops = rule["ops"];
        if (ops)
        {
            for (JsonObject op : ops)
            {
                if (r.opCount >= PLUGIN_OPS_MAX)
                    break;
                PluginOp &o = r.ops[r.opCount];
                o.index = 0;
                o.value = 0;
                o.mask = 0xFF;
                o.intervalMs = 0;
                o.gtwSilent = false;
                const char *type = op["type"] | "";
                if (strcmp(type, "set_bit") == 0)
                {
                    o.type = OP_SET_BIT;
                    o.index = op["bit"] | (uint8_t)0;
                    o.value = op["val"] | (uint8_t)1;
                }
                else if (strcmp(type, "set_byte") == 0)
                {
                    o.type = OP_SET_BYTE;
                    o.index = op["byte"] | (uint8_t)0;
                    o.mask = op["mask"] | (uint8_t)0xFF;
                    o.value = op["val"] | (uint8_t)0;
                }
                else if (strcmp(type, "or_byte") == 0)
                {
                    o.type = OP_OR_BYTE;
                    o.index = op["byte"] | (uint8_t)0;
                    o.value = op["val"] | (uint8_t)0;
                }
                else if (strcmp(type, "and_byte") == 0)
                {
                    o.type = OP_AND_BYTE;
                    o.index = op["byte"] | (uint8_t)0;
                    o.value = op["val"] | (uint8_t)0xFF;
                }
                else if (strcmp(type, "checksum") == 0)
                {
                    o.type = OP_CHECKSUM;
                }
                else if (strcmp(type, "counter") == 0)
                {
                    o.type = OP_COUNTER;
                    o.index = op["byte"] | (uint8_t)0;
                    o.mask = op["mask"] | (uint8_t)0x0F;
                    o.value = op["step"] | (uint8_t)1;
                    if (o.value == 0)
                        o.value = 1;
                }
                else if (strcmp(type, "emit_periodic") == 0)
                {
                    o.type = OP_EMIT_PERIODIC;
                    o.intervalMs =
                        pluginClampPeriodicInterval(op["interval"] | PLUGIN_PERIODIC_INTERVAL_DEFAULT_MS);
                    bool requestedSilent = op["gtw_silent"] | false;
                    if (!requestedSilent)
                        requestedSilent = op["silent"] | false;
                    o.gtwSilent = requestedSilent && pluginGtwSilentSupported();

                    // 当请求gtw_silent时，确保硬件CAN过滤器
                    // 接受UDS流量，以便状态机可以看到响应。
                    if (o.gtwSilent)
                    {
                        const uint32_t udsIds[] = {PLUGIN_GTW_UDS_REQUEST_ID,
                                                   PLUGIN_GTW_UDS_RESPONSE_ID};
                        for (uint8_t k = 0; k < 2; k++)
                        {
                            bool dup = false;
                            for (uint8_t j = 0; j < out.filterIdCount; j++)
                            {
                                if (out.filterIds[j] == udsIds[k])
                                {
                                    dup = true;
                                    break;
                                }
                            }
                            if (!dup && out.filterIdCount < PLUGIN_FILTER_IDS_MAX)
                                out.filterIds[out.filterIdCount++] = udsIds[k];
                        }
                    }
                }
                else
                {
                    continue;
                }
                r.opCount++;
            }
        }

        // 去重过滤器ID
        bool found = false;
        for (uint8_t i = 0; i < out.filterIdCount; i++)
        {
            if (out.filterIds[i] == r.canId)
            {
                found = true;
                break;
            }
        }
        if (!found && out.filterIdCount < PLUGIN_FILTER_IDS_MAX)
            out.filterIds[out.filterIdCount++] = r.canId;

        out.ruleCount++;
    }

    return out.ruleCount > 0;
}

// ── SPIFFS 存储 ──────────────────────────────────────────────────

#if !defined(NATIVE_BUILD)
static String pluginFilePath(const char *filename)
{
    return String("/p_") + filename;
}

static bool pluginSaveToSpiffs(const String &json, const char *filename)
{
    File f = SPIFFS.open(pluginFilePath(filename), "w");
    if (!f)
        return false;
    f.print(json);
    f.close();
    return true;
}

static void pluginLoadAll()
{
    pluginResetPeriodicEmit();
    pluginsLocked = true;
    pluginCount = 0;

    File root = SPIFFS.open("/");
    if (!root)
    {
        pluginsLocked = false;
        return;
    }

    File f = root.openNextFile();
    while (f && pluginCount < PLUGIN_MAX)
    {
        String name = f.name();
        // 标准化：某些SPIFFS版本包含前导/
        if (name.startsWith("/"))
            name = name.substring(1);

        if (name.startsWith("p_") && name.endsWith(".json"))
        {
            String json = f.readString();
            PluginData &p = pluginStore[pluginCount];
            if (pluginParseJson(json, p))
            {
                // 仅存储面向用户的文件名（不含/p_前缀）
                String userFilename = name.substring(2); // remove "p_"
                strlcpy(p.filename, userFilename.c_str(), sizeof(p.filename));
                p.priority = pluginCount;
                pluginCount++;
            }
        }
        f = root.openNextFile();
    }

    pluginResetDiagnostics();
    pluginsLocked = false;
}

static bool pluginRemove(uint8_t index)
{
    if (index >= pluginCount)
        return false;
    pluginsLocked = true;

    SPIFFS.remove(pluginFilePath(pluginStore[index].filename));

    for (uint8_t i = index; i < pluginCount - 1; i++)
    {
        pluginStore[i] = pluginStore[i + 1];
        pluginStore[i].priority = i;
    }
    pluginCount--;
    pluginResetDiagnostics();

    pluginsLocked = false;
    return true;
}
#endif

static void pluginNormalizePriorities()
{
    for (uint8_t i = 0; i < pluginCount; i++)
        pluginStore[i].priority = i;
}

static void pluginResetDiagnostics()
{
    for (uint8_t p = 0; p < pluginCount; p++)
    {
        for (uint8_t r = 0; r < pluginStore[p].ruleCount; r++)
            pluginStore[p].rules[r].diag = {};
    }
}

static void pluginSortByPriority()
{
    pluginsLocked = true;
    for (uint8_t i = 1; i < pluginCount; i++)
    {
        PluginData current = pluginStore[i];
        uint8_t j = i;
        while (j > 0 && pluginStore[j - 1].priority > current.priority)
        {
            pluginStore[j] = pluginStore[j - 1];
            j--;
        }
        pluginStore[j] = current;
    }
    pluginNormalizePriorities();
    pluginsLocked = false;
}

static bool pluginInsert(uint8_t index, const PluginData &plugin)
{
    if (pluginCount >= PLUGIN_MAX)
        return false;
    if (index > pluginCount)
        index = pluginCount;

    pluginsLocked = true;
    for (uint8_t i = pluginCount; i > index; i--)
        pluginStore[i] = pluginStore[i - 1];
    pluginStore[index] = plugin;
    pluginCount++;
    pluginNormalizePriorities();
    pluginResetDiagnostics();
    pluginsLocked = false;
    return true;
}

static bool pluginMove(uint8_t from, uint8_t to)
{
    if (from >= pluginCount || to >= pluginCount || from == to)
        return from < pluginCount && to < pluginCount;

    pluginsLocked = true;
    PluginData moving = pluginStore[from];
    if (from < to)
    {
        for (uint8_t i = from; i < to; i++)
            pluginStore[i] = pluginStore[i + 1];
    }
    else
    {
        for (uint8_t i = from; i > to; i--)
            pluginStore[i] = pluginStore[i - 1];
    }
    pluginStore[to] = moving;
    pluginNormalizePriorities();
    pluginResetDiagnostics();
    pluginsLocked = false;
    return true;
}

static int pluginFindByName(const char *name)
{
    for (uint8_t i = 0; i < pluginCount; i++)
    {
        if (strcmp(pluginStore[i].name, name) == 0)
            return i;
    }
    return -1;
}

// ── 规则执行 ────────────────────────────────────────────────────

static uint8_t pluginCounterShift(uint8_t mask)
{
    uint8_t shift = 0;
    while (shift < 8 && (mask & (1U << shift)) == 0)
        shift++;
    return shift;
}

static void pluginApplyCounter(CanFrame &frame, const PluginOp &op)
{
    if (op.index >= 8 || op.mask == 0)
        return;

    uint8_t shift = pluginCounterShift(op.mask);
    uint8_t fieldMask = op.mask >> shift;
    if (fieldMask == 0)
        return;

    uint8_t current = (frame.data[op.index] >> shift) & fieldMask;
    uint8_t next = (current + op.value) & fieldMask;
    frame.data[op.index] = (frame.data[op.index] & ~op.mask) | ((next << shift) & op.mask);
}

static void pluginApplyOp(CanFrame &frame, const PluginOp &op)
{
    switch (op.type)
    {
    case OP_SET_BIT:
        setBit(frame, op.index, op.value);
        break;
    case OP_SET_BYTE:
        if (op.index < 8)
            frame.data[op.index] = (frame.data[op.index] & ~op.mask) | (op.value & op.mask);
        break;
    case OP_OR_BYTE:
        if (op.index < 8)
            frame.data[op.index] |= op.value;
        break;
    case OP_AND_BYTE:
        if (op.index < 8)
            frame.data[op.index] &= op.value;
        break;
    case OP_CHECKSUM:
        frame.data[7] = computeVehicleChecksum(frame);
        break;
    case OP_COUNTER:
        pluginApplyCounter(frame, op);
        break;
    case OP_EMIT_PERIODIC:
        break;
    }
}

static uint64_t pluginOpWriteMask(const PluginOp &op)
{
    switch (op.type)
    {
    case OP_SET_BIT:
        if (op.index < 64)
            return 1ULL << op.index;
        return 0;
    case OP_SET_BYTE:
        if (op.index < 8)
            return static_cast<uint64_t>(op.mask) << (op.index * 8);
        return 0;
    case OP_OR_BYTE:
        if (op.index < 8)
            return static_cast<uint64_t>(op.value) << (op.index * 8);
        return 0;
    case OP_AND_BYTE:
        if (op.index < 8)
            return static_cast<uint64_t>(static_cast<uint8_t>(~op.value)) << (op.index * 8);
        return 0;
    case OP_CHECKSUM:
        return 0xFFULL << 56;
    case OP_COUNTER:
        if (op.index < 8)
            return static_cast<uint64_t>(op.mask) << (op.index * 8);
        return 0;
    case OP_EMIT_PERIODIC:
        return 0;
    }
    return 0;
}

static bool pluginApplyOpMasked(CanFrame &frame, const PluginOp &op, uint64_t allowedMask)
{
    switch (op.type)
    {
    case OP_SET_BIT:
        if ((allowedMask & pluginOpWriteMask(op)) != 0)
        {
            setBit(frame, op.index, op.value);
            return true;
        }
        return false;
    case OP_SET_BYTE:
        if (op.index < 8)
        {
            uint8_t allowed = static_cast<uint8_t>((allowedMask >> (op.index * 8)) & 0xFF);
            if (allowed == 0)
                return false;
            frame.data[op.index] = (frame.data[op.index] & ~allowed) | (op.value & allowed);
            return true;
        }
        return false;
    case OP_OR_BYTE:
        if (op.index < 8)
        {
            uint8_t allowed = static_cast<uint8_t>((allowedMask >> (op.index * 8)) & 0xFF);
            if (allowed == 0)
                return false;
            frame.data[op.index] |= allowed;
            return true;
        }
        return false;
    case OP_AND_BYTE:
        if (op.index < 8)
        {
            uint8_t allowed = static_cast<uint8_t>((allowedMask >> (op.index * 8)) & 0xFF);
            if (allowed == 0)
                return false;
            frame.data[op.index] &= static_cast<uint8_t>(~allowed);
            return true;
        }
        return false;
    case OP_CHECKSUM:
        return allowedMask == pluginOpWriteMask(op);
    case OP_COUNTER:
        if (allowedMask == pluginOpWriteMask(op))
        {
            pluginApplyCounter(frame, op);
            return true;
        }
        return false;
    case OP_EMIT_PERIODIC:
        return false;
    }
    return false;
}

static void pluginAdvanceRuleCounters(CanFrame &frame, const PluginRule &rule)
{
    bool checksum = false;
    for (uint8_t o = 0; o < rule.opCount; o++)
    {
        const PluginOp &op = rule.ops[o];
        if (op.type == OP_COUNTER)
            pluginApplyCounter(frame, op);
        else if (op.type == OP_CHECKSUM)
            checksum = true;
    }
    if (checksum)
        frame.data[7] = computeVehicleChecksum(frame);
}

static void pluginAdvanceCounters(CanFrame &frame, const PluginOp *counterOps,
                                  uint8_t counterOpCount, bool checksum)
{
    for (uint8_t i = 0; i < counterOpCount; i++)
        pluginApplyCounter(frame, counterOps[i]);
    if (checksum)
        frame.data[7] = computeVehicleChecksum(frame);
}

static bool pluginFrameChanged(const CanFrame &a, const CanFrame &b)
{
    if (a.id != b.id || a.dlc != b.dlc)
        return true;

    uint8_t dlc = (a.dlc <= 8) ? a.dlc : 8;
    for (uint8_t i = 0; i < dlc; i++)
    {
        if (a.data[i] != b.data[i])
            return true;
    }
    return false;
}

static void pluginCopyDiagFrame(uint8_t *out, const CanFrame &frame)
{
    for (uint8_t i = 0; i < 8; i++)
        out[i] = frame.data[i];
}

static void pluginNoteRuleMatched(PluginRule &rule, unsigned long now)
{
    uint32_t before = rule.diag.matchCount;
    rule.diag.matchCount++;
    rule.diag.lastMatchMs = now;
    if (before == 0)
    {
        char msg[64];
        snprintf(msg, sizeof(msg), "[PLG] Rule match CAN 0x%03lX mux %d",
                 static_cast<unsigned long>(rule.canId), static_cast<int>(rule.mux));
        pluginLogDiagnosticsEvent(msg);
    }
}

static void pluginNoteRuleChanged(PluginRule &rule, const CanFrame &original, const CanFrame &modified)
{
    rule.diag.changedCount++;
    pluginCopyDiagFrame(rule.diag.lastOriginal, original);
    pluginCopyDiagFrame(rule.diag.lastModified, modified);
}

static bool pluginHasEnabledPeriodicEmit()
{
    for (uint8_t p = 0; p < pluginCount; p++)
    {
        if (!pluginStore[p].enabled)
            continue;
        for (uint8_t r = 0; r < pluginStore[p].ruleCount; r++)
        {
            const PluginRule &rule = pluginStore[p].rules[r];
            if (rule.canId != 2047 || !pluginRuleMuxIncludes(rule, 3) || !rule.sendAfter)
                continue;
            for (uint8_t o = 0; o < rule.opCount; o++)
            {
                if (rule.ops[o].type == OP_EMIT_PERIODIC)
                    return true;
            }
        }
    }
    return false;
}

static void pluginCachePeriodicEmit(const CanFrame &frame, uint16_t intervalMs, bool gtwSilent,
                                    const PluginOp *counterOps, uint8_t counterOpCount,
                                    bool checksum, unsigned long now)
{
    bool wasActive = pluginPeriodicEmit.active;
    bool wasGtwSilent = pluginPeriodicEmit.gtwSilent;
    PluginGtwUdsMachine preservedUds = pluginPeriodicEmit.uds;

    pluginPeriodicEmit.active = true;
    pluginPeriodicEmit.gtwSilent = gtwSilent;
    pluginPeriodicEmit.checksum = checksum;
    pluginPeriodicEmit.frame = frame;
    pluginPeriodicEmit.counterOpCount = counterOpCount;
    pluginPeriodicEmit.intervalMs = pluginClampPeriodicInterval(intervalMs);
    pluginPeriodicEmit.nextFrameAt = now + pluginPeriodicEmit.intervalMs;

    // 在重复缓存之间保留UDS状态，这样我们不会在每次
    // 周期性帧刷新时重新启动握手。
    if (wasActive && wasGtwSilent && gtwSilent)
    {
        pluginPeriodicEmit.uds = preservedUds;
        pluginPeriodicEmit.uds.bus = frame.bus;
    }
    else
    {
        pluginPeriodicEmit.uds = {};
        pluginPeriodicEmit.uds.bus = frame.bus;
        if (gtwSilent)
        {
            pluginPeriodicEmit.uds.retryAfterMs = PLUGIN_GTW_UDS_RETRY_BACKOFF_MS;
            pluginPeriodicEmit.uds.nextActionAt = now;
            pluginPeriodicEmit.uds.stateEnteredAt = now;
        }
    }

    for (uint8_t i = 0; i < counterOpCount && i < PLUGIN_OPS_MAX; i++)
        pluginPeriodicEmit.counterOps[i] = counterOps[i];
}

static void pluginEmitPeriodicTick(CanDriver &driver, unsigned long now)
{
    if (pluginsLocked || !pluginPeriodicEmit.active)
        return;
    if (!pluginHasEnabledPeriodicEmit())
    {
        pluginResetPeriodicEmit();
        return;
    }

    if (pluginPeriodicEmit.gtwSilent)
        pluginGtwUdsTick(pluginPeriodicEmit.uds, driver, now);

    if ((long)(now - pluginPeriodicEmit.nextFrameAt) < 0)
        return;

    pluginAdvanceCounters(pluginPeriodicEmit.frame, pluginPeriodicEmit.counterOps,
                          pluginPeriodicEmit.counterOpCount, pluginPeriodicEmit.checksum);
    pluginPeriodicEmit.nextFrameAt = now + pluginPeriodicEmit.intervalMs;
}

static bool pluginProcessFrame(const CanFrame &original, CanDriver &)
{
    if (pluginsLocked || pluginCount == 0)
        return false;

    bool processed = false;
    bool checksumPending = false;
    bool emitPeriodicRequested = false;
    bool emitPeriodicGtwSilent = false;
    uint16_t emitPeriodicIntervalMs = PLUGIN_PERIODIC_INTERVAL_MAX_MS;
    PluginOp counterOps[PLUGIN_OPS_MAX];
    uint8_t counterOpCount = 0;
    PluginRule *changedRules[PLUGIN_MAX * PLUGIN_RULES_MAX];
    uint8_t changedRuleCount = 0;
    uint64_t claimed = 0;
    CanFrame modified = original;
    unsigned long now = millis();

    for (uint8_t p = 0; p < pluginCount; p++)
    {
        if (!pluginStore[p].enabled)
            continue;
        uint64_t pluginTouched = 0;
        for (uint8_t r = 0; r < pluginStore[p].ruleCount; r++)
        {
            PluginRule &rule = pluginStore[p].rules[r];
            if (rule.canId != original.id)
                continue;

            if (!pluginRuleMatchesBus(rule, original) || !pluginRuleMatchesMux(rule, original) ||
                !pluginRuleMatchesByte(rule, original))
                continue;

            processed = true;
            pluginNoteRuleMatched(rule, now);

            bool ruleTouched = false;
            for (uint8_t o = 0; o < rule.opCount; o++)
            {
                const PluginOp &op = rule.ops[o];
                if (op.type == OP_EMIT_PERIODIC)
                {
                    emitPeriodicRequested = true;
                    emitPeriodicGtwSilent |= op.gtwSilent;
                    if (op.intervalMs < emitPeriodicIntervalMs)
                        emitPeriodicIntervalMs = op.intervalMs;
                    continue;
                }
                uint64_t opMask = pluginOpWriteMask(op);
                uint64_t allowedMask = opMask & ~claimed;
                if (op.type == OP_CHECKSUM)
                {
                    if (allowedMask == opMask && pluginApplyOpMasked(modified, op, allowedMask))
                    {
                        pluginTouched |= opMask;
                        checksumPending = true;
                        ruleTouched = true;
                    }
                    continue;
                }
                if (op.type == OP_COUNTER)
                {
                    if (opMask != 0 && allowedMask == opMask && pluginApplyOpMasked(modified, op, allowedMask))
                    {
                        pluginTouched |= opMask;
                        if (counterOpCount < PLUGIN_OPS_MAX)
                            counterOps[counterOpCount++] = op;
                        ruleTouched = true;
                    }
                    continue;
                }

                if (allowedMask != 0 && pluginApplyOpMasked(modified, op, allowedMask))
                {
                    pluginTouched |= allowedMask;
                    ruleTouched = true;
                }
            }
            if (ruleTouched && changedRuleCount < (PLUGIN_MAX * PLUGIN_RULES_MAX))
                changedRules[changedRuleCount++] = &rule;
        }
        claimed |= pluginTouched;
    }

    if (checksumPending)
        modified.data[7] = computeVehicleChecksum(modified);
    bool cachePeriodic = emitPeriodicRequested && original.id == 2047 && (original.data[0] & 0x07) == 3;
    if (pluginFrameChanged(original, modified))
    {
        for (uint8_t i = 0; i < changedRuleCount; i++)
            pluginNoteRuleChanged(*changedRules[i], original, modified);
        pluginAdvanceCounters(modified, counterOps, counterOpCount, checksumPending);
    }
    if (cachePeriodic)
        pluginCachePeriodicEmit(modified, emitPeriodicIntervalMs, emitPeriodicGtwSilent,
                                counterOps, counterOpCount, checksumPending, millis());

    return processed;
}

// ── 过滤器合并 ──────────────────────────────────────────────────

static uint8_t pluginGetFilterIds(uint32_t *ids, uint8_t maxIds)
{
    uint8_t count = 0;
    for (uint8_t p = 0; p < pluginCount; p++)
    {
        if (!pluginStore[p].enabled)
            continue;
        for (uint8_t i = 0; i < pluginStore[p].filterIdCount; i++)
        {
            if (count >= maxIds)
                return count;
            bool dup = false;
            for (uint8_t j = 0; j < count; j++)
            {
                if (ids[j] == pluginStore[p].filterIds[i])
                {
                    dup = true;
                    break;
                }
            }
            if (!dup)
                ids[count++] = pluginStore[p].filterIds[i];
        }
    }
    return count;
}

#endif
