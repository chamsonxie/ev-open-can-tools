#include <unity.h>
#include <cstring>
#include "can_frame_types.h"
#include "can_helpers.h"
#include "handlers.h"
#include "app.h"

static MyCanHandler handler;
static int onFrameCount = 0;

static void countOnFrame(const CanFrame &)
{
    onFrameCount++;
}

void setUp()
{
    handler = MyCanHandler();
    handler.enablePrint = false;
    handler.onFrame = countOnFrame;
    onFrameCount = 0;
}

void tearDown() {}

void test_batcher_empty_does_nothing()
{
    CanFrameBatcher batcher;
    batcher.process(&handler);
    TEST_ASSERT_EQUAL_UINT32(0, handler.frameCount);
    TEST_ASSERT_EQUAL_INT(0, onFrameCount);
}

void test_batcher_single_frame()
{
    CanFrameBatcher batcher;
    CanFrame f = {.id = 280};
    f.dlc = 3;
    f.data[2] = 1U << 5;
    batcher.add(f);

    TEST_ASSERT_EQUAL_UINT8(1, batcher.count);
    batcher.process(&handler);
    TEST_ASSERT_EQUAL_UINT32(1, handler.frameCount);
    TEST_ASSERT_TRUE(handler.Parked);
    TEST_ASSERT_EQUAL_INT(1, onFrameCount);
}

void test_batcher_dedup_keeps_last()
{
    CanFrameBatcher batcher;
    CanFrame f1 = {.id = 280};
    f1.dlc = 3;
    f1.data[2] = 1U << 5;
    batcher.add(f1);

    CanFrame f2 = {.id = 280};
    f2.dlc = 3;
    f2.data[2] = 4U << 5;
    batcher.add(f2);

    TEST_ASSERT_EQUAL_UINT8(1, batcher.count);
    TEST_ASSERT_EQUAL_UINT32(2, batcher.totalRead);

    batcher.process(&handler);
    TEST_ASSERT_FALSE(handler.Parked);
    TEST_ASSERT_EQUAL_INT(1, onFrameCount);
}

void test_batcher_max_capacity()
{
    CanFrameBatcher batcher;
    for (uint8_t i = 0; i < 100; i++)
    {
        CanFrame f = {.id = (uint32_t)i};
        f.dlc = 8;
        batcher.add(f);
    }
    TEST_ASSERT_EQUAL_UINT8(48, batcher.count);
    TEST_ASSERT_EQUAL_UINT32(100, batcher.totalRead);
}

void test_batcher_multiple_unique_ids()
{
    CanFrameBatcher batcher;
    CanFrame ids[5] = {};
    ids[0].id = 0x118; ids[0].dlc = 8;
    ids[1].id = 0x257; ids[1].dlc = 8;
    ids[2].id = 0x389; ids[2].dlc = 8;
    ids[3].id = 0x117; ids[3].dlc = 8;
    ids[4].id = 0x3F5; ids[4].dlc = 8;
    for (auto &f : ids)
        batcher.add(f);
    TEST_ASSERT_EQUAL_UINT8(5, batcher.count);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_batcher_empty_does_nothing);
    RUN_TEST(test_batcher_single_frame);
    RUN_TEST(test_batcher_dedup_keeps_last);
    RUN_TEST(test_batcher_max_capacity);
    RUN_TEST(test_batcher_multiple_unique_ids);

    return UNITY_END();
}
