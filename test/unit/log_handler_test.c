#include "log_handler.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

void test_log_writer(void)
{
    int result;

    result = log_writer("test.log", "Test log message");
    CU_ASSERT(result == 0);
}

void test_log_reader(void)
{
    char buffer[256];
    int result;

    result = log_reader("test.log", buffer, sizeof(buffer));
    CU_ASSERT(result == 0);
}

int main()
{
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Log Handler Test Suite", 0, 0);

    CU_add_test(suite, "test_log_writer", test_log_writer);
    CU_add_test(suite, "test_log_reader", test_log_reader);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
