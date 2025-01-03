#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "checksum_sha256_verifier.h"
#include "checksum_md5_verifier.h"

// Setup function
static int
setup(void)
{
    // Initialize resources needed for the tests
    return 0;
}

// Teardown function
static int
teardown(void)
{
    // Clean up resources used by the tests
    return 0;
}

// Test case for SHA-256 verification
static void
test_sha256_verification(void)
{
    const char *data = "test data";
    const char *expected_checksum = "9c56cc51b1eaea2f8b62c8b0e6e1e6e1e6e1e6e1e6e1e6e1e6e1e6e1e6e1e6e1";
    int result;

    result = verify_sha256_checksum(data, expected_checksum);
    CU_ASSERT(result == 0);
}

// Test case for MD5 verification
static void
test_md5_verification(void)
{
    const char *data = "test data";
    const char *expected_checksum = "eb733a00c0c9d336e65691a37ab54293";
    int result;

    result = verify_md5_checksum(data, expected_checksum);
    CU_ASSERT(result == 0);
}

// Test case for error handling
static void
test_error_handling(void)
{
    const char *data = "test data";
    const char *invalid_checksum = "invalid checksum";
    int result;

    result = verify_sha256_checksum(data, invalid_checksum);
    CU_ASSERT(result != 0);

    result = verify_md5_checksum(data, invalid_checksum);
    CU_ASSERT(result != 0);
}

int
main(void)
{
    CU_pSuite suite = NULL;

    // Initialize the CUnit test registry
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    // Add a suite to the registry
    suite = CU_add_suite("Checksum Verification Suite", setup, teardown);
    if (NULL == suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add the tests to the suite
    if ((NULL == CU_add_test(suite, "test_sha256_verification", test_sha256_verification)) ||
        (NULL == CU_add_test(suite, "test_md5_verification", test_md5_verification)) ||
        (NULL == CU_add_test(suite, "test_error_handling", test_error_handling))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
