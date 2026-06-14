#include <unity.h>
#include "can_frame_types.h"
#include "drivers/can_driver.h"
#include "can_helpers.h"
#include "handlers.h"
#include "drivers/mock_driver.h"

static MockDriver mock;
static HW3Handler handler;

void setUp()
{
    mock.reset();
    handler = HW3Handler();
    handler.enablePrint = false;
    enhancedAutopilotRuntime = true;
}

void tearDown() {}

// --- 基于跟车距离的速度配置文件 (CAN ID 1016) ---

void test_hw3_follow_distance_1_sets_profile_2()
{
    CanFrame f = {.id = 1016};
    f.data[5] = 0b00100000; // 跟车距离 = 1
    handler.handleMessage(f, mock);
    TEST_ASSERT_EQUAL_INT(2, handler.speedProfile);
    TEST_ASSERT_EQUAL(0, mock.sent.size());
}

void test_hw3_follow_distance_2_sets_profile_1()
{
    CanFrame f = {.id = 1016};
    f.data[5] = 0b01000000; // 跟车距离 = 2
    handler.handleMessage(f, mock);
    TEST_ASSERT_EQUAL_INT(1, handler.speedProfile);
}

void test_hw3_follow_distance_3_sets_profile_0()
{
    CanFrame f = {.id = 1016};
    f.data[5] = 0b01100000; // 跟车距离 = 3
    handler.handleMessage(f, mock);
    TEST_ASSERT_EQUAL_INT(0, handler.speedProfile);
}

void test_hw3_follow_distance_0_keeps_default()
{
    CanFrame f = {.id = 1016};
    f.data[5] = 0x00; // 跟车距离 = 0
    handler.handleMessage(f, mock);
    TEST_ASSERT_EQUAL_INT(1, handler.speedProfile); // 默认值
}

void test_hw3_manual_profile_ignores_follow_distance()
{
    handler.speedProfileAuto = false;
    handler.speedProfile = 1;

    CanFrame f = {.id = 1016};
    f.data[5] = 0b00100000; // 跟车距离 = 1 将映射到配置文件 2
    handler.handleMessage(f, mock);

    TEST_ASSERT_EQUAL_INT(1, handler.speedProfile);
    TEST_ASSERT_FALSE(handler.speedProfileAuto);
}

void test_hw3_follow_distance_profile_survives_mux0_without_injection()
{
    CanFrame followDistanceFrame = {.id = 1016};
    followDistanceFrame.data[5] = 0b00100000; // 跟车距离 = 1 => 配置文件 2
    handler.handleMessage(followDistanceFrame, mock);
    TEST_ASSERT_EQUAL_INT(2, handler.speedProfile);

    mock.reset();
    CanFrame autopilotFrame = {.id = 1021};
    autopilotFrame.data[0] = 0x00; // MUX 0
    autopilotFrame.data[4] = 0x20; // AD 已选择
    autopilotFrame.data[3] = 60;   // 速度偏移 = 0
    autopilotFrame.data[6] = 0x02;
    handler.handleMessage(autopilotFrame, mock);

    TEST_ASSERT_EQUAL_INT(0, handler.speedOffset);
    TEST_ASSERT_EQUAL_INT(2, handler.speedProfile);
    TEST_ASSERT_EQUAL(1, mock.sent.size());
    TEST_ASSERT_EQUAL_HEX8(0x02, mock.sent[0].data[6] & 0x06);
}

// --- AD 影子修复回归测试 ---

void test_hw3_AD_enabled_only_set_on_mux0()
{
    CanFrame f0 = {.id = 1021};
    f0.data[0] = 0x00; // MUX 0
    f0.data[4] = 0x20; // AD 已选择
    handler.handleMessage(f0, mock);
    TEST_ASSERT_TRUE(handler.ADEnabled);

    mock.reset();
    CanFrame f2 = {.id = 1021};
    f2.data[0] = 0x02; // MUX 2
    f2.data[4] = 0x00; // 此帧中 AD 位未设置
    handler.handleMessage(f2, mock);
    TEST_ASSERT_TRUE(handler.ADEnabled);
    TEST_ASSERT_EQUAL(0, mock.sent.size());
}

void test_hw3_AD_disabled_on_mux0_prevents_mux2_send()
{
    // MUX 0，AD 禁用
    CanFrame f0 = {.id = 1021};
    f0.data[0] = 0x00;
    f0.data[4] = 0x00; // AD 未选择
    handler.handleMessage(f0, mock);
    TEST_ASSERT_FALSE(handler.ADEnabled);

    mock.reset();
    CanFrame f2 = {.id = 1021};
    f2.data[0] = 0x02;
    handler.handleMessage(f2, mock);
    TEST_ASSERT_EQUAL(0, mock.sent.size());
}

// --- AD 激活 (MUX 0) ---

void test_hw3_AD_mux0_sends_with_bit46()
{
    CanFrame f = {.id = 1021};
    f.data[0] = 0x00;
    f.data[4] = 0x20;
    handler.handleMessage(f, mock);
    TEST_ASSERT_EQUAL(1, mock.sent.size());
    TEST_ASSERT_EQUAL_HEX8(0x40, mock.sent[0].data[5] & 0x40);
}

void test_hw3_recorded_ap_mux0_enables_ad()
{
    CanFrame f = {.id = 1021};
    f.data[0] = 0x00;
    f.data[1] = 0x00;
    f.data[2] = 0x00;
    f.data[3] = 0x40;
    f.data[4] = 0x20;
    f.data[5] = 0x01;
    f.data[6] = 0x01;
    f.data[7] = 0x80;

    handler.handleMessage(f, mock);

    TEST_ASSERT_TRUE(handler.ADEnabled);
    TEST_ASSERT_EQUAL(1, mock.sent.size());
}

void test_hw3_das_status_available_does_not_mark_ap_active()
{
    CanFrame f = {.id = 921};
    f.data[0] = 0x02; // 可用

    handler.handleMessage(f, mock);

    TEST_ASSERT_FALSE(handler.APActive);
    TEST_ASSERT_EQUAL(0, mock.sent.size());
}

void test_hw3_das_status_active_marks_ap_active()
{
    CanFrame f = {.id = 921};
    f.data[0] = 0x03; // 激活_1

    handler.handleMessage(f, mock);

    TEST_ASSERT_TRUE(handler.APActive);
    TEST_ASSERT_EQUAL(0, mock.sent.size());
}

void test_hw3_gear_park_marks_parked()
{
    CanFrame f = {.id = 390};
    f.dlc = 8;
    f.data[7] = static_cast<uint8_t>(1U << 3);

    handler.handleMessage(f, mock);

    TEST_ASSERT_TRUE(handler.Parked);
    TEST_ASSERT_EQUAL(0, mock.sent.size());
}

void test_hw3_gear_drive_clears_parked()
{
    handler.Parked = true;
    CanFrame f = {.id = 390};
    f.dlc = 8;
    f.data[7] = static_cast<uint8_t>(4U << 3);

    handler.handleMessage(f, mock);

    TEST_ASSERT_FALSE(handler.Parked);
    TEST_ASSERT_EQUAL(0, mock.sent.size());
}

// --- 提示抑制 (MUX 1) ---

void test_hw3_nag_suppression_clears_bit19_on_mux1()
{
    CanFrame f = {.id = 1021};
    f.data[0] = 0x01;
    setBit(f, 19, true);
    handler.handleMessage(f, mock);
    TEST_ASSERT_EQUAL(1, mock.sent.size());
    TEST_ASSERT_FALSE((mock.sent[0].data[2] >> 3) & 0x01);
}

void test_hw3_nag_suppression_skips_mux1_changes_when_eap_runtime_disabled()
{
    enhancedAutopilotRuntime = false;
    CanFrame f = {.id = 1021};
    f.data[0] = 0x01;
    setBit(f, 19, true);
    handler.handleMessage(f, mock);
    TEST_ASSERT_EQUAL(0, mock.sent.size()); // 运行时禁用时不发送帧
}
void test_hw3_mux1_sets_track_labels_bit46()
{
    CanFrame f = {.id = 1021};
    f.data[0] = 0x01;
    handler.handleMessage(f, mock);
    TEST_ASSERT_EQUAL(1, mock.sent.size());
    TEST_ASSERT_EQUAL_HEX8(0x40, mock.sent[0].data[5] & 0x40); // 位 46
}

// --- 无关 CAN ID 不发送 ---

void test_hw3_ignores_unrelated_can_id()
{
    CanFrame f = {.id = 999};
    handler.handleMessage(f, mock);
    TEST_ASSERT_EQUAL(0, mock.sent.size());
}

void test_hw3_gw_autopilot_mux2_updates_state_without_send()
{
    CanFrame f = {.id = 2047};
    f.data[0] = 0x02;
    f.data[5] = 0x08; // 增强版 = 2
    handler.handleMessage(f, mock);
    TEST_ASSERT_EQUAL_INT(2, handler.gatewayAutopilot);
    TEST_ASSERT_EQUAL(0, mock.sent.size());
}

// --- 发送计数 ---

void test_hw3_AD_enabled_mux0_sends_exactly_1()
{
    CanFrame f = {.id = 1021};
    f.data[0] = 0x00;
    f.data[4] = 0x20;
    handler.handleMessage(f, mock);
    TEST_ASSERT_EQUAL(1, mock.sent.size());
}

void test_hw3_mux1_sends_exactly_1()
{
    CanFrame f = {.id = 1021};
    f.data[0] = 0x01;
    handler.handleMessage(f, mock);
    TEST_ASSERT_EQUAL(1, mock.sent.size());
}

// --- 过滤器 ID ---

void test_hw3_filter_ids_count()
{
    TEST_ASSERT_EQUAL_UINT8(6, handler.filterIdCount());
}

void test_hw3_filter_ids_values()
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

    RUN_TEST(test_hw3_filter_ids_count);
    RUN_TEST(test_hw3_filter_ids_values);

    RUN_TEST(test_hw3_follow_distance_1_sets_profile_2);
    RUN_TEST(test_hw3_follow_distance_2_sets_profile_1);
    RUN_TEST(test_hw3_follow_distance_3_sets_profile_0);
    RUN_TEST(test_hw3_follow_distance_0_keeps_default);
    RUN_TEST(test_hw3_manual_profile_ignores_follow_distance);
    RUN_TEST(test_hw3_follow_distance_profile_survives_mux0_without_injection);

    RUN_TEST(test_hw3_AD_enabled_only_set_on_mux0);
    RUN_TEST(test_hw3_AD_disabled_on_mux0_prevents_mux2_send);

    RUN_TEST(test_hw3_AD_mux0_sends_with_bit46);
    RUN_TEST(test_hw3_recorded_ap_mux0_enables_ad);
    RUN_TEST(test_hw3_das_status_available_does_not_mark_ap_active);
    RUN_TEST(test_hw3_das_status_active_marks_ap_active);
    RUN_TEST(test_hw3_gear_park_marks_parked);
    RUN_TEST(test_hw3_gear_drive_clears_parked);
    RUN_TEST(test_hw3_nag_suppression_clears_bit19_on_mux1);
    RUN_TEST(test_hw3_nag_suppression_skips_mux1_changes_when_eap_runtime_disabled);
    RUN_TEST(test_hw3_mux1_sets_track_labels_bit46);
    RUN_TEST(test_hw3_ignores_unrelated_can_id);
    RUN_TEST(test_hw3_gw_autopilot_mux2_updates_state_without_send);

    RUN_TEST(test_hw3_AD_enabled_mux0_sends_exactly_1);
    RUN_TEST(test_hw3_mux1_sends_exactly_1);

    return UNITY_END();
}
