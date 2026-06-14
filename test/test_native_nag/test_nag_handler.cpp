#include <unity.h>
#include "can_frame_types.h"
#include "handlers.h"

static NagHandler handler;

void setUp()
{
    handler = NagHandler();
    handler.enablePrint = false;
}

void tearDown() {}

// ============================================================
// 过滤器 ID
// ============================================================

void test_nag_filter_ids_count()
{
    TEST_ASSERT_EQUAL_UINT8(1, handler.filterIdCount());
}

void test_nag_filter_ids_value()
{
    const uint32_t *ids = handler.filterIds();
    TEST_ASSERT_EQUAL_UINT32(880, ids[0]);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_nag_filter_ids_count);
    RUN_TEST(test_nag_filter_ids_value);

    return UNITY_END();
}
