# 000 — Core Project Identity

This is **EV Open Can Mod** (v3.0.1), an open-source CAN-bus modification firmware for ESP32 microcontrollers targeting **Tesla vehicles**. Licensed under GPL-3.0.

## What It Does

Sits on a vehicle CAN bus, monitors CAN frames, and applies real-time modifications:
- Speed profile injection (follow distance → speed profile mapping)
- Autopilot nag suppression (CAN ID 880 counter+1 echo)
- AP Injection Gate (only inject when AP active, parked, or summoning)
- GTW UDS silencing (gateway broadcast suppression)
- Dashboard web UI for configuration, sniffer, recorder, OTA updates

## Repository Structure

```
include/          — All firmware logic (header-only C++ templates/inlines)
  drivers/        — CAN driver abstractions (TWAI, MCP2515, SAME51, Mock)
  web/            — Dashboard web server + UI
  platform/       — ESP-IDF Arduino-compatible shim
src/              — Entry points (main.cpp, espidf_runtime.cpp)
test/             — Native unit tests (Unity framework) + Python tests
scripts/          — Build/CI/dev tooling
docs/             — Documentation (plugins, building, dashboard)
legacy-arduino/   — Legacy PlatformIO config for Arduino-only boards
.github/          — CI/CD workflows
```

## Key Design Decisions

- **All firmware logic lives in headers** (`include/*.h`) — template/inline pattern for both ESP-IDF and native test compilation
- **Native tests compile the same headers** on macOS/Linux with `-DNATIVE_BUILD` for fast iteration
- **Build config via `platformio_profile.h`** — compile-time feature toggles (git-ignored)
- **Feature toggles as compile-time defines**: `NAG_KILLER`, `LEGACY`, `HW3`, `HW4`, `ESP32_DASHBOARD`, `ENHANCED_AUTOPILOT`, etc.
- **CAN is always-on** — frame processing is the core loop; the dashboard is a co-process on ESP32 builds
