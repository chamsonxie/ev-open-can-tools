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
    TEST_ASSERT_EQUAL_UINT8(0, handler.filterIdCount());
    TEST_ASSERT_NULL(handler.filterIds());
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_dashboard_handler_filter_ids);

    return UNITY_END();
}
