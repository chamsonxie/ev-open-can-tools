#include <unity.h>
#include "can_frame_types.h"
#include "handlers.h"

static MyCanHandler handler;

void setUp()
{
    handler = MyCanHandler();
    handler.enablePrint = false;
}

void tearDown() {}

void test_hw4_filter_ids_count()
{
    constexpr auto kExpectedCount = sizeof(MyCanHandler::kFilterIds) / sizeof(MyCanHandler::kFilterIds[0]);
    TEST_ASSERT_EQUAL_UINT8(kExpectedCount, handler.filterIdCount());
    TEST_ASSERT_NOT_NULL(handler.filterIds());
    TEST_ASSERT_EQUAL_UINT32(0x118, handler.filterIds()[0]);
    TEST_ASSERT_EQUAL_UINT32(0x257, handler.filterIds()[1]);
    TEST_ASSERT_EQUAL_UINT32(0x389, handler.filterIds()[2]);
    TEST_ASSERT_EQUAL_UINT32(0x107, handler.filterIds()[3]);
    TEST_ASSERT_EQUAL_UINT32(0x3F5, handler.filterIds()[4]);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_hw4_filter_ids_count);

    return UNITY_END();
}
