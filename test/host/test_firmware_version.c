#include "firmware_version.h"
#include "unity.h"

#include <stddef.h>

void test_parses_version(void)
{
    firmware_version_t version;
    TEST_ASSERT_EQUAL_INT(FIRMWARE_VERSION_OK, firmware_version_parse("0.1.0", &version));
    TEST_ASSERT_EQUAL_UINT(0, version.major);
    TEST_ASSERT_EQUAL_UINT(1, version.minor);
    TEST_ASSERT_EQUAL_UINT(0, version.patch);
}

void test_parses_multi_digit_parts(void)
{
    firmware_version_t version;
    TEST_ASSERT_EQUAL_INT(FIRMWARE_VERSION_OK, firmware_version_parse("12.345.999999", &version));
    TEST_ASSERT_EQUAL_UINT(12, version.major);
    TEST_ASSERT_EQUAL_UINT(345, version.minor);
    TEST_ASSERT_EQUAL_UINT(999999, version.patch);
}

void test_rejects_null(void)
{
    firmware_version_t version;
    TEST_ASSERT_EQUAL_INT(FIRMWARE_VERSION_ERR_INVALID, firmware_version_parse(NULL, &version));
    TEST_ASSERT_EQUAL_INT(FIRMWARE_VERSION_ERR_INVALID, firmware_version_parse("0.1.0", NULL));
}

void test_rejects_missing_parts(void)
{
    firmware_version_t version;
    TEST_ASSERT_EQUAL_INT(FIRMWARE_VERSION_ERR_INVALID, firmware_version_parse("1", &version));
    TEST_ASSERT_EQUAL_INT(FIRMWARE_VERSION_ERR_INVALID, firmware_version_parse("1.2", &version));
    TEST_ASSERT_EQUAL_INT(FIRMWARE_VERSION_ERR_INVALID, firmware_version_parse("1-2.3", &version));
}

void test_rejects_extra_text(void)
{
    firmware_version_t version;
    TEST_ASSERT_EQUAL_INT(FIRMWARE_VERSION_ERR_INVALID, firmware_version_parse("v1.2.3", &version));
    TEST_ASSERT_EQUAL_INT(FIRMWARE_VERSION_ERR_INVALID,
                          firmware_version_parse("1.2.3-rc1", &version));
    TEST_ASSERT_EQUAL_INT(FIRMWARE_VERSION_ERR_INVALID,
                          firmware_version_parse("1.2.3.4", &version));
}

void test_rejects_empty_parts(void)
{
    firmware_version_t version;
    TEST_ASSERT_EQUAL_INT(FIRMWARE_VERSION_ERR_INVALID, firmware_version_parse("", &version));
    TEST_ASSERT_EQUAL_INT(FIRMWARE_VERSION_ERR_INVALID, firmware_version_parse("1..3", &version));
    TEST_ASSERT_EQUAL_INT(FIRMWARE_VERSION_ERR_INVALID, firmware_version_parse(".2.3", &version));
}

void test_rejects_long_parts(void)
{
    firmware_version_t version;
    TEST_ASSERT_EQUAL_INT(FIRMWARE_VERSION_ERR_INVALID,
                          firmware_version_parse("1.2.1234567", &version));
}

void test_leaves_out_untouched_on_failure(void)
{
    firmware_version_t version = {7, 8, 9};
    TEST_ASSERT_EQUAL_INT(FIRMWARE_VERSION_ERR_INVALID, firmware_version_parse("1.2.x", &version));
    TEST_ASSERT_EQUAL_UINT(7, version.major);
    TEST_ASSERT_EQUAL_UINT(8, version.minor);
    TEST_ASSERT_EQUAL_UINT(9, version.patch);
}
