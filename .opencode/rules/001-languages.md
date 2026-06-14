# 001 — Language & Framework Rules

## C++ (Primary Language)

- **Standard**: C++17 (`-std=gnu++17`)
- **Style**: Template/inline-heavy, header-only architecture
- **No exceptions, no RTTI** (ESP-IDF constraints)
- **Use `constexpr` where possible** for compile-time evaluation
- **Prefer C++ headers** (`<cstdint>`, `<cstring>`) over C equivalents
- **Use `uint8_t`, `uint32_t`** from `<cstdint>` for fixed-width types
- **No dynamic allocation** in hot paths (CAN frame processing loop)
- **`Shared<T>` wrapper**: `std::atomic<T>` on device, plain `T` on native builds

## Python (Tooling & Tests)

- **Python 3** only (no Python 2)
- **Standard library** for tests (`unittest`), minimal external deps
- **ruff** for formatting (dev container)
- Scripts in `scripts/` are self-contained with argparse

## ESP-IDF Specifics

- **ESP-IDF v6.0.1** via PlatformIO (`platformio/espressif32@7.0.0`)
- Arduino-compatible shim in `include/platform/espidf_runtime.h` / `src/espidf_runtime.cpp`
- WiFi, WebServer, Preferences, Update, SPIFFS, HTTPClient all have shim implementations
- `#ifdef ESP_PLATFORM` vs `#ifndef NATIVE_BUILD` for platform-specific code
