# EV Open Can Mod Project Rules

This is an ESP32-based CAN-bus modification firmware for Tesla vehicles. See `.opencode/rules/` for detailed progressive rules.

## Quick Reference

### Build
- `pio run -e esp32_twai` — build for generic ESP32
- `pio run -e esp32_ext_mcp2515` — build for ESP32-S3 + MCP2515

### Test
- `pio test -e native` — run primary native tests
- `python -m unittest discover -s test -p 'test_*.py'` — run Python tests

### Verify Before Commit
1. `clang-format --dry-run --Werror` on C++ files
2. `pio test -e native`
3. `python -m unittest discover -s test -p 'test_*.py'`

### Key Conventions
- Header-only C++17 with template/inline pattern (`-std=gnu++17`)
- Allman braces, spaces only, no column limit
- `Shared<T>` = `std::atomic<T>` on device, plain `T` on native
- Feature toggles via compile-time defines in `platformio_profile.h`
- CAN data always 8 bytes; checksum at byte 7

### Architecture
- `include/drivers/` — CAN driver abstraction (TWAI, MCP2515, SAME51, Mock)
- `include/handlers.h` — Vehicle-specific message handlers (Legacy, HW3, HW4, Nag)
- `include/app.h` — Template-based application framework (setup/loop)
- `include/web/mcp2515_dashboard.h` — Dashboard web server
