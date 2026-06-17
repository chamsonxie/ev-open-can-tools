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

void test_nag_filter_ids_count()
{
    TEST_ASSERT_EQUAL_UINT8(7, handler.filterIdCount());
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_nag_filter_ids_count);

    return UNITY_END();
}
