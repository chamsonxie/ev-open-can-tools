# 003 — Architecture & Module Design

## CAN Driver Hierarchy (`include/drivers/`)

```
CanDriver (abstract base)
├── TWAI_driver     — ESP32 built-in CAN (TWAI peripheral)
└── Mock_driver     — Unit test mock
```

- All drivers implement `init()`, `setFilters()`, `enableInterrupt()`, `read()`, `send()`, `diagnosticsJson()`, `diagnosticsSummary()`
- `kSupportsISR` static constexpr controls interrupt-driven vs polling read
- `read()` returns `bool` (true if frame available), frame written to out-param

## Handler System (`include/handlers.h`)

```
CarManagerBase<Derived> (CRTP base)
├── LegacyHandler  — Tesla Legacy (MCU1) CAN IDs: 69, 280, 390, 921, 1006
├── HW3Handler     — Tesla HW3 CAN IDs: 280, 390, 921, 1016, 1021, 2047
├── HW4Handler     — Tesla HW4 (same as HW3 but different speed profile mapping)
├── NagHandler     — Nag suppression only (CAN ID 880)
└── MyCanHandler   — Common handler for ESP-NOW signals (5 IDs: 0x118, 0x257, 0x389, 0x39D, 0x3F5)
```

- CRTP pattern (`CarManagerBase<Derived>`) eliminates virtual dispatch; all calls through `MyCanHandler*`
- `handleMessage(CanFrame &frame, CanDriver &driver)` — modifies frame in-place and sends if changed
- `filterIds()` / `filterIdCount()` — which CAN IDs to subscribe to
- `onFrame(CanFrame &frame)` — hook for ESP-NOW broadcast and other observation
- Shared state via `Shared<T>` (atomic on device, plain on native)

## AP Injection Gate

- Only allows injection when: `APActive || Parked || Summoning`
- `injectionGateOpen()` in `CarManagerBase`
- Park detection: DI gear (CAN 280 byte 2 bits 5-7); defaults to `true` on boot
- Summon detection: tracks `sprSeen` flag from UI_selfParkRequest
- Safety-critical: never inject when car is being driven manually

## Template Application Pattern (`include/app.h`)

```cpp
appSetup(std::unique_ptr<Driver> drv, const char *readyMsg)
appLoop()
```

- `main.cpp` creates driver based on build defines, calls `appSetup<Driver>(...)`
- Loop reads frames, calls handler, calls plugin hook
- Dashboard-enabled builds add WiFi/HTTP server, status LED, toggle button

## Feature Toggle System

- Build-time defines in `platformio_profile.h` + `platformio_sync_profile.py`
- Runtime overrides in dashboard web UI (preferences stored in NVS/SPIFFS)
- Pattern: `kFeatureBuildEnabled` compile-time constant + `featureRuntime` runtime toggle
- Dashboard mode: all features compiled in, controlled via web UI
- Non-dashboard mode: fixed feature set per build
