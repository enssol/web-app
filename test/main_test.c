#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

// Function prototypes for test suite initializations
int init_unit_tests(void);
int init_integration_tests(void);
int init_security_tests(void);
int init_performance_tests(void);

int
main(void)
{
    CU_pSuite pSuite = NULL;

    // Initialize the CUnit test registry
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    // Register Unit Test Suite
    if (init_unit_tests() != 0) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Register Integration Test Suite
    if (init_integration_tests() != 0) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Register Security Test Suite
    if (init_security_tests() != 0) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Register Performance Test Suite
    if (init_performance_tests() != 0) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
