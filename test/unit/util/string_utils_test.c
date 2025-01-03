#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_utils.h"
#include <CUnit/Basic.h>
#include <assert.h>

// Setup function
void setup(void)
{
    // Initialize resources needed for tests
}

// Teardown function
void teardown(void)
{
    // Clean up resources
}

// Test function for string length
void test_stringUtils_strlen(void)
{
    size_t len;

    setup();
    len = stringUtils_strlen("test");
    assert(len == 4);
    teardown();
}

// Test function for string copy
void test_stringUtils_strcpy(void)
{
    char dest[10];

    setup();
    stringUtils_strcpy(dest, "test");
    assert(strcmp(dest, "test") == 0);
    teardown();
}

void test_string_length(void)
{
    const char *str = "test";
    CU_ASSERT_EQUAL(string_length(str), 4);
}

void test_string_copy(void)
{
    char dest[10];
    const char *src = "copy";
    CU_ASSERT_STRING_EQUAL(string_copy(dest, src), "copy");
}

void test_string_concat(void)
{
    char dest[10] = "hello";
    const char *src = "world";
    CU_ASSERT_STRING_EQUAL(string_concat(dest, src), "helloworld");
}

int main(void)
{
    CU_pSuite suite = NULL;

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    suite = CU_add_suite("string_utils_test_suite", setup, teardown);
    if (NULL == suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(suite, "test_string_length", test_string_length)) ||
        (NULL == CU_add_test(suite, "test_string_copy", test_string_copy)) ||
        (NULL == CU_add_test(suite, "test_string_concat", test_string_concat))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
