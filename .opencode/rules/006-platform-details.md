# 006 — Platform-Specific Details

## Dashboard Web UI (`include/web/dashboard.h`)

- HTTP server with routes for: WiFi, plugins, OTA, sniffer, recorder, settings, backup/restore
- UI HTML is minified + gzipped into `include/web/dashboard_ui.h`
- To regenerate: `python scripts/minify_dashboard.py`
- Dashboard is only compiled with `-DESP32_DASHBOARD`
- Non-dashboard (legacy Arduino) builds have no web server

## CAN Recorder

- Records CAN frames to a buffer in memory
- Can export as JSON via dashboard API
- Maximum 512 frames in recording buffer

## CAN Sniffer

- Live CAN frame display via Server-Sent Events (SSE) endpoint
- Filterable by CAN ID in dashboard UI

## WiFi Configuration

- AP + STA mode
- Multi-SSID support (up to 4 networks)
- Configurable via dashboard web UI
- Stored in Preferences/SPIFFS

## OTA Updates

- GitHub release OTA with stable + beta channel
- Auto-update on boot option
- Manual upload via dashboard
- ESP-IDF `esp_https_ota` for secure updates

## Status LED

- RGB LED (WS2812) via `led_strip` (ESP-IDF) or `neopixelWrite` (Arduino)
- Color coding: Red = idle, Green = injecting, Blue = OTA
- Blink when no WiFi clients connected
- Disabled on legacy Arduino boards

## Partition Tables

- `partitions_4mb_ota_1536k.csv`: 4MB flash, 1536KB OTA partition
- `partitions_8mb_ota_3072k.csv`: 8MB flash, 3072KB OTA partition
- Chosen per board in `platformio.ini`

## MCP2515 Specifics

- Crystal frequencies: 8MHz or 16MHz (set via `MCP_CRYSTAL_FREQ` define)
- SPI pin mapping per board (CS, SCK, MISO, MOSI, interrupt)
