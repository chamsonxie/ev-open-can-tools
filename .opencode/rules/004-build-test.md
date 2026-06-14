# 004 — Build, Test & CI Commands

## Build Commands

```bash
# ESP32 dashboard targets
pio run -e esp32_twai                    # Generic ESP32
pio run -e esp32s2_twai                  # ESP32-S2
pio run -e lilygo_tcan485_hw3            # LILYGO TCAN485
pio run -e m5stack-atomic-can-base       # M5Stack Atom CAN Base
pio run -e m5stack-atoms3-mini-can-base  # M5Stack AtomS3 Mini
pio run -e esp32_feather_v2_mcp2515      # Feather ESP32 V2 + MCP2515
pio run -e esp32_ext_mcp2515             # ESP32-S3 + MCP2515
pio run -e waveshare_ESP32_S3_RS485_CAN  # Waveshare ESP32-S3

# Legacy Arduino targets (use legacy-arduino dir)
cd legacy-arduino && pio run -e feather_rp2040_can
cd legacy-arduino && pio run -e feather_m4_can

# Flash + monitor
pio run -e esp32_twai -t upload
pio device monitor
```

## Local Build Setup

```bash
cp platformio_profile.example.h platformio_profile.h
# Edit platformio_profile.h to configure features
python scripts/platformio_set_profile.py --driver DRIVER_TWAI --vehicle HW4
```

## Test Commands

```bash
# Run ALL native tests (Unity framework)
pio test -e native

# Run specific native test suites
pio test -e native_dashboard
pio test -e native_nag

pio test -e native_injection_after_ap
pio test -e native_bypass_tlssc_requirement
pio test -e native_log_buffer
pio test -e native_mcp2515_recovery

# Run Python tests
python -m unittest discover -s test -p 'test_*.py'

# Run ALL CI tests (parallel)
pio test -e native -e native_dashboard -e native_nag \

```

## CI Pipeline (`.github/workflows/tests.yml`)

1. Release metadata check (VERSION + CHANGELOG consistency)
2. ClangFormat lint (`clang-format --dry-run --Werror`)
3. Python tests (`unittest discover`)
4. Native tests (9 envs in parallel)
5. PlatformIO build (10 targets in parallel)

## Lint Command

```bash
git ls-files '*.cpp' '*.h' '*.hpp' ':!:include/web/mcp2515_dashboard_ui.h' \
  | xargs -r clang-format --dry-run --Werror --style=file
```

## Before Committing

Always run:
1. `clang-format` (C++ files)
2. Native tests (`pio test -e native`)
3. Python tests (`python -m unittest discover -s test -p 'test_*.py'`)
