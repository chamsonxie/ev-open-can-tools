#pragma once
// ── Bluetooth Classic SPP bridge ─────────────────────────────────────────────
//
// Streams CAN frames to a paired Android device using the SLCAN text protocol.
// Any Android serial-terminal or CAN-monitor app that speaks SLCAN can connect.
//
// SLCAN frame format (sent for every received CAN frame):
//   Standard frame: t<3-hex-ID><DLC><data-hex>\r
//   Example: t1FD8AABBCCDDEEFF0011\r
//
// Commands accepted from Android (optional — for future use):
//   O\r  — Open  (start forwarding)
//   C\r  — Close (stop forwarding)
//   S5\r — Set 500 kbps (acknowledged only, bus speed is fixed)
//   V\r  — Version string
//
// Usage:
//   #include "bt_bridge.h"
//   btBridgeInit();          // in setup(), after WiFi is up
//   btBridgeFrame(frame);    // in the CAN receive loop

#if defined(ESP32_DASHBOARD) && defined(BLUETOOTH_SERIAL) && !defined(NATIVE_BUILD)

#include <Arduino.h>
#include <BluetoothSerial.h>
#include "can_frame_types.h"

#ifndef BT_DEVICE_NAME
#define BT_DEVICE_NAME "ev-open-can-tools"
#endif

static BluetoothSerial btSerial;
static bool btBridgeOpen = true; // forwarding active by default once connected

static void btBridgeInit(const char *deviceName = BT_DEVICE_NAME)
{
    btSerial.begin(deviceName);
    Serial.printf("[BT] SPP started — device name: %s\n", deviceName);
}

// Forward one CAN frame over BT in SLCAN format.
// Call this for every frame received from the bus.
static void btBridgeFrame(const CanFrame &f)
{
    if (!btSerial.hasClient() || !btBridgeOpen)
        return;

    // Standard 11-bit IDs: "t<3-hex-ID><DLC><data-hex>\r"
    // Extended 29-bit IDs: "T<8-hex-ID><DLC><data-hex>\r"
    const bool extended = (f.id & 0x80000000u) != 0;
    const uint32_t rawId = f.id & 0x1FFFFFFFu;

    char buf[64];
    int pos = 0;

    if (extended)
        pos += snprintf(buf + pos, sizeof(buf) - pos, "T%08X%u", rawId, (unsigned)f.len);
    else
        pos += snprintf(buf + pos, sizeof(buf) - pos, "t%03X%u", rawId, (unsigned)f.len);

    for (uint8_t i = 0; i < f.len && i < 8; i++)
        pos += snprintf(buf + pos, sizeof(buf) - pos, "%02X", f.data[i]);

    buf[pos++] = '\r';
    buf[pos] = '\0';
    btSerial.write((const uint8_t *)buf, pos);
}

// Process incoming commands from the Android client.
// Call from main loop / dashboard poll task.
static void btBridgePoll()
{
    if (!btSerial.hasClient())
        return;

    static char cmdBuf[16];
    static uint8_t cmdLen = 0;

    while (btSerial.available())
    {
        char c = (char)btSerial.read();
        if (c == '\r' || c == '\n')
        {
            if (cmdLen > 0)
            {
                cmdBuf[cmdLen] = '\0';
                // Handle SLCAN commands
                if (cmdBuf[0] == 'O')
                {
                    btBridgeOpen = true;
                    btSerial.write("\r");
                }
                else if (cmdBuf[0] == 'C')
                {
                    btBridgeOpen = false;
                    btSerial.write("\r");
                }
                else if (cmdBuf[0] == 'V')
                {
                    // Version string — mimic SLCAN v1.0
                    btSerial.write("V1010\r");
                }
                else if (cmdBuf[0] == 'S')
                {
                    // Speed command — acknowledge (bus speed is fixed at 500k)
                    btSerial.write("\r");
                }
                else
                {
                    // Unknown command — send BEL (error)
                    btSerial.write("\a");
                }
                cmdLen = 0;
            }
        }
        else if (cmdLen < (uint8_t)(sizeof(cmdBuf) - 1))
        {
            cmdBuf[cmdLen++] = c;
        }
    }
}

#else

// Stubs when Bluetooth is disabled
static inline void btBridgeInit(const char * = nullptr) {}
static inline void btBridgeFrame(const CanFrame &) {}
static inline void btBridgePoll() {}

#endif // ESP32_DASHBOARD && BLUETOOTH_SERIAL && !NATIVE_BUILD
