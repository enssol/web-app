#include "data_validator.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

void test_validate_email(void)
{
    int result;

    result = validate_email("test@example.com");
    CU_ASSERT(result == 0);
}

void test_validate_password(void)
{
    int result;

    result = validate_password("ValidPass123");
    CU_ASSERT(result == 0);
}

int main()
{
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Data Validator Test Suite", 0, 0);

    CU_add_test(suite, "test_validate_email", test_validate_email);
    CU_add_test(suite, "test_validate_password", test_validate_password);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
