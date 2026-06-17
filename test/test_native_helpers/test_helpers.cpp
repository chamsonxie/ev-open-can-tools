#include <unity.h>
#include "can_frame_types.h"
#include "can_helpers.h"

void setUp() {}
void tearDown() {}

// --- setBit ---

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
    setBit(f, 46, true);
    TEST_ASSERT_EQUAL_HEX8(0x40, f.data[5]);
}

void test_setBit_clears_bit()
{
    CanFrame f = {};
    f.data[2] = 0xFF;
    setBit(f, 19, false);
    TEST_ASSERT_EQUAL_HEX8(0xF7, f.data[2]);
}

void test_setBit_does_not_affect_other_bytes()
{
    CanFrame f = {};
    f.data[0] = 0xAA;
    f.data[1] = 0xBB;
    setBit(f, 8, true);
    TEST_ASSERT_EQUAL_HEX8(0xAA, f.data[0]);
    TEST_ASSERT_EQUAL_HEX8(0xBB, f.data[1]);
}

// --- extractIntel (little-endian) ---

void test_extractIntel_single_bit_byte0()
{
    uint8_t data[8] = {0x04};
    TEST_ASSERT_EQUAL_UINT64(1, extractIntel(data, 2, 1));
}

void test_extractIntel_multi_bit()
{
    uint8_t data[8] = {0x00, 0x03};
    TEST_ASSERT_EQUAL_UINT64(3, extractIntel(data, 8, 2));
}

void test_extractIntel_spanning_bytes()
{
    // data[0]=0xF0 (bits 0-7), data[1]=0x0F (bits 8-11)
    // extract bits 4-11: data[0] bits 4-7 = 0xF, data[1] bits 0-3 = 0xF => 0xFF
    uint8_t data[8] = {0xF0, 0x0F};
    TEST_ASSERT_EQUAL_UINT64(0xFF, extractIntel(data, 4, 8));
}

// --- extractMotorola (big-endian) ---
// Motorola numbering: bit 0 = MSB of byte 0, bit 7 = LSB of byte 0.

void test_extractMotorola_single_bit_msb()
{
    uint8_t data[8] = {0x80};
    TEST_ASSERT_EQUAL_UINT64(1, extractMotorola(data, 0, 1));
}

void test_extractMotorola_single_bit_lsb()
{
    uint8_t data[8] = {0x01};
    TEST_ASSERT_EQUAL_UINT64(1, extractMotorola(data, 7, 1));
}

void test_extractMotorola_byte_reversed()
{
    // Motorola bits 0-7 = byte 0 reversed
    // 0xAB (0b10101011) reversed = 0b11010101 = 0xD5
    uint8_t data[8] = {0xAB};
    TEST_ASSERT_EQUAL_UINT64(0xD5, extractMotorola(data, 0, 8));
}

// --- readDIGear ---

void test_readDIGear_extracts_bits_21_23()
{
    CanFrame f = {};
    f.data[2] = static_cast<uint8_t>(4U << 5);
    TEST_ASSERT_EQUAL_UINT8(4, readDIGear(f));
}

void test_readDIGear_park()
{
    CanFrame f = {};
    f.data[2] = static_cast<uint8_t>(1U << 5);
    TEST_ASSERT_EQUAL_UINT8(1, readDIGear(f));
}

// --- isVehicleParked ---

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
    TEST_ASSERT_TRUE(isVehicleParked(7));
}

void test_isVehicleParked_true_for_invalid()
{
    TEST_ASSERT_TRUE(isVehicleParked(0));
}

void test_isVehicleParked_false_for_reverse_neutral()
{
    TEST_ASSERT_FALSE(isVehicleParked(2));
    TEST_ASSERT_FALSE(isVehicleParked(3));
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_setBit_sets_bit0_of_byte0);
    RUN_TEST(test_setBit_sets_bit7_of_byte0);
    RUN_TEST(test_setBit_sets_bit_in_byte5);
    RUN_TEST(test_setBit_clears_bit);
    RUN_TEST(test_setBit_does_not_affect_other_bytes);

    RUN_TEST(test_extractIntel_single_bit_byte0);
    RUN_TEST(test_extractIntel_multi_bit);
    RUN_TEST(test_extractIntel_spanning_bytes);

    RUN_TEST(test_extractMotorola_single_bit_msb);
    RUN_TEST(test_extractMotorola_single_bit_lsb);
    RUN_TEST(test_extractMotorola_byte_reversed);

    RUN_TEST(test_readDIGear_extracts_bits_21_23);
    RUN_TEST(test_readDIGear_park);

    RUN_TEST(test_isVehicleParked_true_for_park);
    RUN_TEST(test_isVehicleParked_false_for_drive);
    RUN_TEST(test_isVehicleParked_true_for_sna);
    RUN_TEST(test_isVehicleParked_true_for_invalid);
    RUN_TEST(test_isVehicleParked_false_for_reverse_neutral);

    return UNITY_END();
}
