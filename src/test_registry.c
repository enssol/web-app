#include "test_registry.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

/* Suite initialization function */
static int
suite_init(void)
{
    /* Initialization code */
    return 0;
}

/* Suite cleanup function */
static int
suite_cleanup(void)
{
    /* Cleanup code */
    return 0;
}

/* Example test function */
static void
test_example(void)
{
    CU_ASSERT(1 == 1);
}

/* Initialize test suites */
int
initialize_suites(void)
{
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    /* Add a suite to the registry */
    pSuite = CU_add_suite("Example Suite", suite_init, suite_cleanup);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "test_example", test_example))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    return CUE_SUCCESS;
}

/* Cleanup test suites */
int
cleanup_suites(void)
{
    /* Cleanup the CUnit test registry */
    CU_cleanup_registry();
    return CU_get_error();
}
