#include <unity.h>
#include "can_frame_types.h"
#include "can_helpers.h"
#include "handlers.h"

void setUp() {}

void tearDown() {}

void test_dashboard_legacy_filter_ids()
{
    LegacyHandler handler;
    handler.enablePrint = false;
    TEST_ASSERT_EQUAL_UINT8(5, handler.filterIdCount());
}

void test_dashboard_hw3_filter_ids()
{
    HW3Handler handler;
    handler.enablePrint = false;
    TEST_ASSERT_EQUAL_UINT8(6, handler.filterIdCount());
}

void test_dashboard_hw4_filter_ids()
{
    HW4Handler handler;
    handler.enablePrint = false;
    TEST_ASSERT_EQUAL_UINT8(6, handler.filterIdCount());
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_dashboard_legacy_filter_ids);
    RUN_TEST(test_dashboard_hw3_filter_ids);
    RUN_TEST(test_dashboard_hw4_filter_ids);

    return UNITY_END();
}
