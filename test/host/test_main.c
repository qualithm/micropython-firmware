#include "unity.h"

void test_parses_version(void);
void test_parses_multi_digit_parts(void);
void test_rejects_null(void);
void test_rejects_missing_parts(void);
void test_rejects_extra_text(void);
void test_rejects_empty_parts(void);
void test_rejects_long_parts(void);
void test_leaves_out_untouched_on_failure(void);

void setUp(void) {}

void tearDown(void) {}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_parses_version);
    RUN_TEST(test_parses_multi_digit_parts);
    RUN_TEST(test_rejects_null);
    RUN_TEST(test_rejects_missing_parts);
    RUN_TEST(test_rejects_extra_text);
    RUN_TEST(test_rejects_empty_parts);
    RUN_TEST(test_rejects_long_parts);
    RUN_TEST(test_leaves_out_untouched_on_failure);
    return UNITY_END();
}
