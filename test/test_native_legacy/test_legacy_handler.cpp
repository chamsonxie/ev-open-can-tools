#include <unity.h>
#include "can_frame_types.h"
#include "handlers.h"

static LegacyHandler handler;

void setUp()
{
    handler = LegacyHandler();
    handler.enablePrint = false;
}

void tearDown() {}

// --- 过滤器 ID ---

void test_legacy_filter_ids_count()
{
    TEST_ASSERT_EQUAL_UINT8(5, handler.filterIdCount());
}

void test_legacy_filter_ids_values()
{
    const uint32_t *ids = handler.filterIds();
    TEST_ASSERT_EQUAL_UINT32(69, ids[0]);
    TEST_ASSERT_EQUAL_UINT32(280, ids[1]);
    TEST_ASSERT_EQUAL_UINT32(390, ids[2]);
    TEST_ASSERT_EQUAL_UINT32(921, ids[3]);
    TEST_ASSERT_EQUAL_UINT32(1006, ids[4]);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_legacy_filter_ids_count);
    RUN_TEST(test_legacy_filter_ids_values);

    return UNITY_END();
}
