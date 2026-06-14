# 008 — Testing Conventions

## Native C++ Tests

- **Framework**: Unity (embedded-friendly, included with PlatformIO)
- **Location**: `test/test_native_*/`
- **Run with**: `pio test -e <env>`
- **Test envs** are defined in `platformio.ini` with `test_filter` to select specific dirs
- **Test files**: compiled as part of the PlatformIO test build; Unity's `TEST_CASE`, `TEST_ASSERT_*` macros
- **Native build defines** always include `-DNATIVE_BUILD` to bypass hardware-dependent code

## Test Patterns

- Each handler has a dedicated test environment and file
- Tests construct a `CanFrame`, call `handleMessage()`, assert output frame bytes
- Mock driver (`include/drivers/mock_driver.h`) captures sent frames for verification
- Tests verify filter IDs, modification logic, and edge cases (null frames, boundary values)

## Python Tests

- **Framework**: `unittest`
- **Run with**: `python -m unittest discover -s test -p 'test_*.py'`
- Test files: `test_can_analyzer.py`, `test_wifi_settings_regression.py`, `test_platformio_set_profile.py`

## Writing New Tests

1. Create `test/test_native_<feature>/test_<feature>.cpp`
2. Add environment in `platformio.ini` if new (or add to existing `native` env's `test_filter`)
3. Include `<unity.h>` and use `TEST_CASE` / `TEST_ASSERT_*` macros
4. Use `MockDriver` for CAN send verification
5. Verify with: `pio test -e <env>`
6. For Python: add `test_*.py` file in `test/` dir

## Existing Test Environments

| Env | Tests |
|-----|-------|
| `native` | helpers, legacy, hw3, hw4, twai_filter |
| `native_dashboard` | Dashboard mode handler behavior |
| `native_nag` | NagHandler (CAN 880) |
| `native_mcp2515_recovery` | MCP2515 bus-off recovery |
| `native_bypass_tlssc_requirement` | TLSSC bypass behavior |
| `native_injection_after_ap` | AP Injection Gate behavior |
| `native_log_buffer` | Log ring buffer |
