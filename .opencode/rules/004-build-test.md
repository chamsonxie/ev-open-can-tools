# 004 — Build, Test & CI Commands

## Build Commands

```bash
# ESP32 dashboard target
pio run -e waveshare_ESP32_S3_RS485_CAN  # Waveshare ESP32-S3

# Flash + monitor
pio run -e waveshare_ESP32_S3_RS485_CAN -t upload
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
pio test -e native_log_buffer

# Run Python tests
python -m unittest discover -s test -p 'test_*.py'
```

## CI Pipeline (`.github/workflows/tests.yml`)

1. ClangFormat lint (`clang-format --dry-run --Werror`)
2. Python tests (`unittest discover`)
3. Native tests (all suites via `pio test -e native`)
4. PlatformIO build (`waveshare_ESP32_S3_RS485_CAN`)

## Lint Command

```bash
git ls-files '*.cpp' '*.h' '*.hpp' ':!:include/web/dashboard_ui.h' \
  | xargs -r clang-format --dry-run --Werror --style=file
```

## Before Committing

Always run:
1. `clang-format` (C++ files)
2. Native tests (`pio test -e native`)
3. Python tests (`python -m unittest discover -s test -p 'test_*.py'`)

## Running Tests

### C++ Native Tests

```bash
# Run all native tests
pio test -e native

# Individual test suites
pio test -e native_dashboard  # Dashboard mode handler
pio test -e native_log_buffer # Log ring buffer
```

### Python Tests

```bash
python -m unittest discover -s test -p 'test_*.py'
```
