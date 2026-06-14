#include <unity.h>
#include "can_frame_types.h"
#include "can_helpers.h"

void setUp() {}
void tearDown() {}

// --- 设置位 ---

void test_setBit_sets_bit0_of_byte0()
{
    CanFrame f = {};
    setBit(f, 0, true);
    TEST_ASSERT_EQUAL_HEX8(0x01, f.data[0]);
}

void test_setBit_sets_bit7_of_byte0()
{
    CanFrame f = {};
    setBit(f, 7, true);
    TEST_ASSERT_EQUAL_HEX8(0x80, f.data[0]);
}

void test_setBit_sets_bit_in_byte5()
{
    CanFrame f = {};
    setBit(f, 46, true); // 字节 5, 位 6
    TEST_ASSERT_EQUAL_HEX8(0x40, f.data[5]);
}

void test_setBit_sets_bit_in_byte7()
{
    CanFrame f = {};
    setBit(f, 60, true); // 字节 7, 位 4
    TEST_ASSERT_EQUAL_HEX8(0x10, f.data[7]);
}

void test_setBit_clears_bit()
{
    CanFrame f = {};
    f.data[2] = 0xFF;
    setBit(f, 19, false); // 字节 2, 位 3
    TEST_ASSERT_EQUAL_HEX8(0xF7, f.data[2]);
}

void test_setBit_does_not_affect_other_bytes()
{
    CanFrame f = {};
    f.data[0] = 0xAA;
    f.data[1] = 0xBB;
    setBit(f, 8, true); // 字节 1, 位 0
    TEST_ASSERT_EQUAL_HEX8(0xAA, f.data[0]);
    TEST_ASSERT_EQUAL_HEX8(0xBB, f.data[1]);
}

// --- 读取 MUX ID ---

void test_readMuxID_extracts_lower_3_bits()
{
    CanFrame f = {};
    f.data[0] = 0x05;
    TEST_ASSERT_EQUAL_UINT8(5, readMuxID(f));
}

void test_readMuxID_masks_upper_bits()
{
    CanFrame f = {};
    f.data[0] = 0xFA; // 二进制：11111010 -> 低 3 位 = 010 = 2
    TEST_ASSERT_EQUAL_UINT8(2, readMuxID(f));
}

void test_readMuxID_zero()
{
    CanFrame f = {};
    f.data[0] = 0x00;
    TEST_ASSERT_EQUAL_UINT8(0, readMuxID(f));
}

void test_readMuxID_max_value()
{
    CanFrame f = {};
    f.data[0] = 0x07;
    TEST_ASSERT_EQUAL_UINT8(7, readMuxID(f));
}

// --- 判断 UI 中 AD 是否已选择 ---

void test_isADSelectedInUI_true_when_bit5_set()
{
    CanFrame f = {};
    f.data[4] = 0x20; // 位 5 已设置
    TEST_ASSERT_TRUE(isADSelectedInUI(f));
}

void test_isADSelectedInUI_false_when_bit5_clear()
{
    CanFrame f = {};
    f.data[4] = 0x00;
    TEST_ASSERT_FALSE(isADSelectedInUI(f));
}

void test_isADSelectedInUI_ignores_other_bits()
{
    CanFrame f = {};
    f.data[4] = 0xDF; // 除位 5 外所有位均已设置
    TEST_ASSERT_FALSE(isADSelectedInUI(f));
}

void test_isADSelectedInUI_true_with_other_bits()
{
    CanFrame f = {};
    f.data[4] = 0xFF;
    TEST_ASSERT_TRUE(isADSelectedInUI(f));
}

// --- 读取网关自动驾驶状态 ---

void test_readGTWAutopilot_extracts_bits_42_to_44()
{
    CanFrame f = {};
    f.data[5] = 0x0C; // 位 42-44 处为 0b011
    TEST_ASSERT_EQUAL_UINT8(3, readGTWAutopilot(f));
}

void test_readGTWAutopilot_masks_other_bits()
{
    CanFrame f = {};
    f.data[5] = 0xFF;
    TEST_ASSERT_EQUAL_UINT8(7, readGTWAutopilot(f));
}

// --- DAS 自动驾驶状态 ---

void test_readDASAutopilotStatus_extracts_lower_nibble()
{
    CanFrame f = {};
    f.data[0] = 0xA5;
    TEST_ASSERT_EQUAL_UINT8(5, readDASAutopilotStatus(f));
}

void test_isDASAutopilotActive_true_for_active_states()
{
    TEST_ASSERT_TRUE(isDASAutopilotActive(3));
    TEST_ASSERT_TRUE(isDASAutopilotActive(4));
    TEST_ASSERT_TRUE(isDASAutopilotActive(5));
}

void test_isDASAutopilotActive_false_for_available_state()
{
    TEST_ASSERT_FALSE(isDASAutopilotActive(2));
}

// --- 档位状态 ---

void test_readVehicleGear_extracts_dif_gear_bits()
{
    CanFrame f = {};
    f.data[7] = static_cast<uint8_t>(4U << 3);
    TEST_ASSERT_EQUAL_UINT8(4, readVehicleGear(f));
}

void test_isVehicleParked_true_for_park()
{
    TEST_ASSERT_TRUE(isVehicleParked(1));
}

void test_isVehicleParked_false_for_drive()
{
    TEST_ASSERT_FALSE(isVehicleParked(4));
}

void test_isVehicleParked_true_for_sna()
{
    // DI 在车辆休眠/哨兵模式下报告 SNA (7)：门控
    // 必须保持开启，以便在冷接近时召唤解锁注入正常工作。
    TEST_ASSERT_TRUE(isVehicleParked(7));
}

void test_isVehicleParked_true_for_invalid()
{
    // DI 完全启动前报告无效 (0)：与 SNA 相同的理由，视为已驻车。
    TEST_ASSERT_TRUE(isVehicleParked(0));
}

void test_isVehicleParked_false_for_reverse_neutral()
{
    TEST_ASSERT_FALSE(isVehicleParked(2));
    TEST_ASSERT_FALSE(isVehicleParked(3));
}

// --- 设置速度配置文件 V12/V13 ---

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_setBit_sets_bit0_of_byte0);
    RUN_TEST(test_setBit_sets_bit7_of_byte0);
    RUN_TEST(test_setBit_sets_bit_in_byte5);
    RUN_TEST(test_setBit_sets_bit_in_byte7);
    RUN_TEST(test_setBit_clears_bit);
    RUN_TEST(test_setBit_does_not_affect_other_bytes);

    RUN_TEST(test_readMuxID_extracts_lower_3_bits);
    RUN_TEST(test_readMuxID_masks_upper_bits);
    RUN_TEST(test_readMuxID_zero);
    RUN_TEST(test_readMuxID_max_value);

    RUN_TEST(test_isADSelectedInUI_true_when_bit5_set);
    RUN_TEST(test_isADSelectedInUI_false_when_bit5_clear);
    RUN_TEST(test_isADSelectedInUI_ignores_other_bits);
    RUN_TEST(test_isADSelectedInUI_true_with_other_bits);
    RUN_TEST(test_readGTWAutopilot_extracts_bits_42_to_44);
    RUN_TEST(test_readGTWAutopilot_masks_other_bits);
    RUN_TEST(test_readDASAutopilotStatus_extracts_lower_nibble);
    RUN_TEST(test_isDASAutopilotActive_true_for_active_states);
    RUN_TEST(test_isDASAutopilotActive_false_for_available_state);
    RUN_TEST(test_readVehicleGear_extracts_dif_gear_bits);
    RUN_TEST(test_isVehicleParked_true_for_park);
    RUN_TEST(test_isVehicleParked_false_for_drive);
    RUN_TEST(test_isVehicleParked_true_for_sna);
    RUN_TEST(test_isVehicleParked_true_for_invalid);
    RUN_TEST(test_isVehicleParked_false_for_reverse_neutral);

    return UNITY_END();
}
