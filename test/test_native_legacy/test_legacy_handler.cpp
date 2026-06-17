#include <unity.h>
#include "can_frame_types.h"
#include "handlers.h"

static MyCanHandler handler;
static int onFrameCallCount = 0;

static void testOnFrame(const CanFrame &)
{
    onFrameCallCount++;
}

void setUp()
{
    handler = MyCanHandler();
    handler.enablePrint = false;
    onFrameCallCount = 0;
}

void tearDown() {}

// --- Filter IDs ---

void test_handler_filter_ids_count()
{
    TEST_ASSERT_EQUAL_UINT8(0, handler.filterIdCount());
    TEST_ASSERT_NULL(handler.filterIds());
}

// --- Gear tracking (from 0x118) ---

void test_handler_gear_park_marks_parked()
{
    CanFrame f = {.id = 280};
    f.dlc = 3;
    f.data[2] = static_cast<uint8_t>(1U << 5); // gear = P (1)

    handler.handleMessage(f);

    TEST_ASSERT_TRUE(handler.Parked);
}

void test_handler_gear_drive_clears_parked()
{
    handler.Parked = true;
    CanFrame f = {.id = 280};
    f.dlc = 3;
    f.data[2] = static_cast<uint8_t>(4U << 5); // gear = D (4)

    handler.handleMessage(f);

    TEST_ASSERT_FALSE(handler.Parked);
}

void test_handler_gear_sna_parked()
{
    CanFrame f = {.id = 280};
    f.dlc = 3;
    f.data[2] = static_cast<uint8_t>(7U << 5); // gear = SNA (7)

    handler.handleMessage(f);

    TEST_ASSERT_TRUE(handler.Parked);
}

// --- onFrame callback ---

void test_handler_onFrame_invoked()
{
    handler.onFrame = testOnFrame;

    CanFrame f = {.id = 280};
    f.dlc = 3;
    f.data[2] = static_cast<uint8_t>(4U << 5);

    handler.handleMessage(f);
    TEST_ASSERT_EQUAL_INT(1, onFrameCallCount);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_handler_filter_ids_count);

    RUN_TEST(test_handler_gear_park_marks_parked);
    RUN_TEST(test_handler_gear_drive_clears_parked);
    RUN_TEST(test_handler_gear_sna_parked);

    RUN_TEST(test_handler_onFrame_invoked);

    return UNITY_END();
}
