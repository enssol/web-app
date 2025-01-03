#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "checksum_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_FILE "test_file.txt"
#define TEST_CONTENT "This is a test file."

/* Setup and teardown functions */
static int setup(void)
{
    FILE *file = fopen(TEST_FILE, "w");
    if (file == NULL) {
        return -1;
    }
    fprintf(file, "%s", TEST_CONTENT);
    fclose(file);
    return 0;
}

static int teardown(void)
{
    return remove(TEST_FILE);
}

/* Test SHA256 calculation */
static void test_calculate_sha256(void)
{
    unsigned char digest[SHA256_DIGEST_LENGTH];
    int result = calculate_sha256(TEST_FILE, digest);
    CU_ASSERT_EQUAL(result, 0);
}

/* Test MD5 calculation */
static void test_calculate_md5(void)
{
    unsigned char digest[MD5_DIGEST_LENGTH];
    int result = calculate_md5(TEST_FILE, digest);
    CU_ASSERT_EQUAL(result, 0);
}

/* Test checksum comparison */
static void test_compare_checksums(void)
{
    unsigned char digest1[SHA256_DIGEST_LENGTH];
    unsigned char digest2[SHA256_DIGEST_LENGTH];
    calculate_sha256(TEST_FILE, digest1);
    calculate_sha256(TEST_FILE, digest2);
    int result = compare_checksums(digest1, digest2, SHA256_DIGEST_LENGTH);
    CU_ASSERT_EQUAL(result, 0);
}

/* Test error handling for non-existent file */
static void test_error_handling(void)
{
    unsigned char digest[SHA256_DIGEST_LENGTH];
    int result = calculate_sha256("non_existent_file.txt", digest);
    CU_ASSERT_EQUAL(result, -1);
}

/* Main function to run tests */
int main(void)
{
    CU_pSuite suite = NULL;

    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    suite = CU_add_suite("ChecksumUtilsTestSuite", setup, teardown);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((CU_add_test(suite, "test_calculate_sha256", test_calculate_sha256) == NULL) ||
        (CU_add_test(suite, "test_calculate_md5", test_calculate_md5) == NULL) ||
        (CU_add_test(suite, "test_compare_checksums", test_compare_checksums) == NULL) ||
        (CU_add_test(suite, "test_error_handling", test_error_handling) == NULL)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
