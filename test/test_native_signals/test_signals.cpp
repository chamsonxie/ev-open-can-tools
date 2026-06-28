#include <unity.h>
#include <cstring>
#include "can_frame_types.h"
#include "can_helpers.h"

void setUp() {}
void tearDown() {}

// ── DI_systemStatus (0x118 / 280) ──

void test_parseDI_systemStatus_gear_park()
{
    uint8_t data[8] = {};
    data[2] = 1U << 5;
    auto s = parseDI_systemStatus(data, 8);
    TEST_ASSERT_EQUAL_UINT8(1, s.gear);
}

void test_parseDI_systemStatus_gear_drive()
{
    uint8_t data[8] = {};
    data[2] = 4U << 5;
    auto s = parseDI_systemStatus(data, 8);
    TEST_ASSERT_EQUAL_UINT8(4, s.gear);
}

void test_parseDI_systemStatus_accel()
{
    uint8_t data[8] = {};
    data[4] = 128;
    auto s = parseDI_systemStatus(data, 8);
    TEST_ASSERT_EQUAL_UINT8(128, s.accelPedalPos);
}

void test_parseDI_systemStatus_regen_on()
{
    uint8_t data[8] = {};
    data[3] = 1U << 2;
    auto s = parseDI_systemStatus(data, 8);
    TEST_ASSERT_EQUAL_UINT8(1, s.regenLight);
}

void test_parseDI_systemStatus_regen_off()
{
    uint8_t data[8] = {};
    data[3] = 0;
    auto s = parseDI_systemStatus(data, 8);
    TEST_ASSERT_EQUAL_UINT8(0, s.regenLight);
}

void test_parseDI_systemStatus_brake_pressed()
{
    uint8_t data[8] = {};
    data[2] = 1U << 3;
    auto s = parseDI_systemStatus(data, 8);
    TEST_ASSERT_EQUAL_UINT8(1, s.brakePedalState);
}

void test_parseDI_systemStatus_short_dlc_returns_defaults()
{
    uint8_t data[8] = {};
    data[2] = 4U << 5;
    auto s = parseDI_systemStatus(data, 3);
    TEST_ASSERT_EQUAL_UINT8(0, s.gear);
}

void test_parseDI_systemStatus_epb_apply()
{
    uint8_t data[8] = {};
    data[5] = 1U << 4;
    auto s = parseDI_systemStatus(data, 8);
    TEST_ASSERT_EQUAL_UINT8(1, s.epbRequest);
}

// ── DI_speed (0x257 / 599) ──

void test_parseDI_speed_normal()
{
    uint8_t data[8] = {};
    data[2] = 0x19;
    data[3] = 80;
    auto s = parseDI_speed(data, 5);
    TEST_ASSERT_EQUAL_UINT16(400, s.vehicleSpeed);
    TEST_ASSERT_EQUAL_UINT8(80, s.uiSpeed);
}

void test_parseDI_speed_short_dlc_returns_defaults()
{
    uint8_t data[8] = {};
    data[3] = 80;
    auto s = parseDI_speed(data, 3);
    TEST_ASSERT_EQUAL_UINT16(0, s.vehicleSpeed);
}

// ── IBST_status (0x107 / 263) ──

void test_parseIBST_status_brake_apply()
{
    uint8_t data[8] = {};
    data[2] = 1U << 1;
    auto s = parseIBST_status(data, 3);
    TEST_ASSERT_EQUAL_UINT8(2, s.driverBrakeApply);
}

void test_parseIBST_status_short_dlc_returns_defaults()
{
    uint8_t data[8] = {};
    data[2] = 1;
    auto s = parseIBST_status(data, 1);
    TEST_ASSERT_EQUAL_UINT8(0, s.driverBrakeApply);
}

// ── VCFRONT_lighting (0x3F5 / 1013) ──

void test_parseVCFRONT_lighting_low_beam()
{
    uint8_t data[8] = {};
    data[3] = (1U << 4);
    auto s = parseVCFRONT_lighting(data, 8);
    TEST_ASSERT_EQUAL_UINT8(1, s.lowBeamLeftStatus);
    TEST_ASSERT_EQUAL_UINT8(0, s.lowBeamRightStatus);
}

void test_parseVCFRONT_lighting_hazard()
{
    uint8_t data[8] = {};
    data[0] = 2U << 4;
    auto s = parseVCFRONT_lighting(data, 8);
    TEST_ASSERT_EQUAL_UINT8(2, s.hazardLightRequest);
}

void test_parseVCFRONT_lighting_short_dlc_returns_defaults()
{
    uint8_t data[8] = {};
    data[3] = 1U << 4;
    auto s = parseVCFRONT_lighting(data, 5);
    TEST_ASSERT_EQUAL_UINT8(0, s.lowBeamLeftStatus);
}

// ── DAS_status2 (0x389 / 905) ──

void test_parseDAS_status2_long_collision()
{
    uint8_t data[8] = {};
    data[6] = 2;
    auto s = parseDAS_status2(data, 8);
    TEST_ASSERT_EQUAL_UINT8(2, s.longCollisionWarning);
}

void test_parseDAS_status2_short_dlc_returns_defaults()
{
    uint8_t data[8] = {};
    data[6] = 2;
    auto s = parseDAS_status2(data, 5);
    TEST_ASSERT_EQUAL_UINT8(0, s.longCollisionWarning);
}

void test_parseDAS_status2_acc_speed_limit()
{
    uint8_t data[8] = {};
    data[0] = 0x64;
    data[1] = 0x03;
    auto s = parseDAS_status2(data, 8);
    TEST_ASSERT_EQUAL_UINT16(868, s.accSpeedLimit);
}

// ── 0x3E2 brake light ──

void test_parseE32_brakeLight_on()
{
    uint8_t data[8] = {0x0D};
    auto s = parseE32_brakeLight(data, 8);
    TEST_ASSERT_EQUAL_UINT8(1, s.brakeLight);
}

void test_parseE32_brakeLight_off()
{
    uint8_t data[8] = {0x0C};
    auto s = parseE32_brakeLight(data, 8);
    TEST_ASSERT_EQUAL_UINT8(0, s.brakeLight);
}

void test_parseE32_brakeLight_short_dlc_returns_default()
{
    uint8_t data[8] = {0x0D};
    auto s = parseE32_brakeLight(data, 0);
    TEST_ASSERT_EQUAL_UINT8(0, s.brakeLight);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_parseDI_systemStatus_gear_park);
    RUN_TEST(test_parseDI_systemStatus_gear_drive);
    RUN_TEST(test_parseDI_systemStatus_accel);
    RUN_TEST(test_parseDI_systemStatus_regen_on);
    RUN_TEST(test_parseDI_systemStatus_regen_off);
    RUN_TEST(test_parseDI_systemStatus_brake_pressed);
    RUN_TEST(test_parseDI_systemStatus_short_dlc_returns_defaults);
    RUN_TEST(test_parseDI_systemStatus_epb_apply);

    RUN_TEST(test_parseDI_speed_normal);
    RUN_TEST(test_parseDI_speed_short_dlc_returns_defaults);

    RUN_TEST(test_parseIBST_status_brake_apply);
    RUN_TEST(test_parseIBST_status_short_dlc_returns_defaults);

    RUN_TEST(test_parseVCFRONT_lighting_low_beam);
    RUN_TEST(test_parseVCFRONT_lighting_hazard);
    RUN_TEST(test_parseVCFRONT_lighting_short_dlc_returns_defaults);

    RUN_TEST(test_parseDAS_status2_long_collision);
    RUN_TEST(test_parseDAS_status2_short_dlc_returns_defaults);
    RUN_TEST(test_parseDAS_status2_acc_speed_limit);

    RUN_TEST(test_parseE32_brakeLight_on);
    RUN_TEST(test_parseE32_brakeLight_off);
    RUN_TEST(test_parseE32_brakeLight_short_dlc_returns_default);

    return UNITY_END();
}
