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
    TEST_ASSERT_EQUAL_UINT8(7, handler.filterIdCount());
}

void test_hw4_filter_ids_values()
{
    const uint32_t *ids = handler.filterIds();
    TEST_ASSERT_EQUAL_UINT32(280, ids[0]);
    TEST_ASSERT_EQUAL_UINT32(297, ids[1]);
    TEST_ASSERT_EQUAL_UINT32(341, ids[2]);
    TEST_ASSERT_EQUAL_UINT32(785, ids[3]);
    TEST_ASSERT_EQUAL_UINT32(923, ids[4]);
    TEST_ASSERT_EQUAL_UINT32(925, ids[5]);
    TEST_ASSERT_EQUAL_UINT32(1013, ids[6]);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_hw4_filter_ids_count);
    RUN_TEST(test_hw4_filter_ids_values);

    return UNITY_END();
}
