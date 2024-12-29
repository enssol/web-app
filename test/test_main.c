/* filepath: test/test_main.c */
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../include/web_server.h"
#include "mock_socket.h"
#include <string.h>

/* External function declarations */
extern int init_web_server_suite(void);
extern int clean_web_server_suite(void);
extern void test_handle_request(void);
extern void test_handle_request_not_found(void);
extern void test_handle_invalid_request(void);

int
main(void)
{
    CU_pSuite suite;
    int ret;

    /* Initialize CUnit registry */
    ret = CU_initialize_registry();
    if (ret != CUE_SUCCESS) {
        return ret;
    }

    /* Add suite to registry */
    suite = CU_add_suite("Web Server Test Suite",
                        init_web_server_suite,
                        clean_web_server_suite);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add tests to suite */
    if ((CU_add_test(suite, "Test Handle Request", test_handle_request) == NULL) ||
        (CU_add_test(suite, "Test Handle Request Not Found",
            test_handle_request_not_found) == NULL) ||
        (CU_add_test(suite, "Test Handle Invalid Request",
            test_handle_invalid_request) == NULL)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run tests */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return CU_get_error();
}
