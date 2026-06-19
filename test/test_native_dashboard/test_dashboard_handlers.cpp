#include <unity.h>
#include "can_frame_types.h"
#include "can_helpers.h"
#include "handlers.h"

void setUp() {}

void tearDown() {}

void test_dashboard_handler_filter_ids()
{
    MyCanHandler handler;
    handler.enablePrint = false;
    TEST_ASSERT_EQUAL_UINT8(5, handler.filterIdCount());
    TEST_ASSERT_NOT_NULL(handler.filterIds());
    TEST_ASSERT_EQUAL_UINT32(0x118, handler.filterIds()[0]);
    TEST_ASSERT_EQUAL_UINT32(0x257, handler.filterIds()[1]);
    TEST_ASSERT_EQUAL_UINT32(0x389, handler.filterIds()[2]);
    TEST_ASSERT_EQUAL_UINT32(0x39D, handler.filterIds()[3]);
    TEST_ASSERT_EQUAL_UINT32(0x3F5, handler.filterIds()[4]);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_dashboard_handler_filter_ids);

    return UNITY_END();
}
