#include <unity.h>
#include "can_frame_types.h"
#include "handlers.h"

static HW4Handler handler;

void setUp()
{
    handler = HW4Handler();
    handler.enablePrint = false;
}

void tearDown() {}

// --- 状态跟踪 ---

void test_hw4_das_status_available_does_not_mark_ap_active()
{
    CanFrame f = {.id = 921};
    f.data[0] = 0x02;

    handler.handleMessage(f);

    TEST_ASSERT_FALSE(handler.APActive);
}

void test_hw4_das_status_active_marks_ap_active()
{
    CanFrame f = {.id = 921};
    f.data[0] = 0x04;

    handler.handleMessage(f);

    TEST_ASSERT_TRUE(handler.APActive);
}

void test_hw4_gw_autopilot_mux2_updates_state()
{
    CanFrame f = {.id = 2047};
    f.data[0] = 0x02;
    f.data[5] = 0x0C;
    handler.handleMessage(f);
    TEST_ASSERT_EQUAL_INT(3, handler.gatewayAutopilot);
}

void test_hw4_gear_park_marks_parked()
{
    CanFrame f = {.id = 390};
    f.dlc = 8;
    f.data[7] = static_cast<uint8_t>(1U << 3);

    handler.handleMessage(f);

    TEST_ASSERT_TRUE(handler.Parked);
}

void test_hw4_gear_drive_clears_parked()
{
    handler.Parked = true;
    CanFrame f = {.id = 390};
    f.dlc = 8;
    f.data[7] = static_cast<uint8_t>(4U << 3);

    handler.handleMessage(f);

    TEST_ASSERT_FALSE(handler.Parked);
}

// --- 过滤器 ID ---

void test_hw4_filter_ids_count()
{
    TEST_ASSERT_EQUAL_UINT8(6, handler.filterIdCount());
}

void test_hw4_filter_ids_values()
{
    const uint32_t *ids = handler.filterIds();
    TEST_ASSERT_EQUAL_UINT32(280, ids[0]);
    TEST_ASSERT_EQUAL_UINT32(390, ids[1]);
    TEST_ASSERT_EQUAL_UINT32(921, ids[2]);
    TEST_ASSERT_EQUAL_UINT32(1016, ids[3]);
    TEST_ASSERT_EQUAL_UINT32(1021, ids[4]);
    TEST_ASSERT_EQUAL_UINT32(2047, ids[5]);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_hw4_filter_ids_count);
    RUN_TEST(test_hw4_filter_ids_values);

    RUN_TEST(test_hw4_das_status_available_does_not_mark_ap_active);
    RUN_TEST(test_hw4_das_status_active_marks_ap_active);
    RUN_TEST(test_hw4_gw_autopilot_mux2_updates_state);
    RUN_TEST(test_hw4_gear_park_marks_parked);
    RUN_TEST(test_hw4_gear_drive_clears_parked);

    return UNITY_END();
}
