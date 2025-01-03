#include "request_handler.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

void test_handle_get_request(void)
{
    struct http_request req;
    struct http_response res;
    int result;

    req.method = "GET";
    req.uri = "/test";
    req.body = NULL;

    result = handle_get_request(&req, &res);
    CU_ASSERT(result == 0);
}

void test_handle_post_request(void)
{
    struct http_request req;
    struct http_response res;
    int result;

    req.method = "POST";
    req.uri = "/test";
    req.body = "data=test";

    result = handle_post_request(&req, &res);
    CU_ASSERT(result == 0);
}

int main()
{
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Request Handler Test Suite", 0, 0);

    CU_add_test(suite, "test_handle_get_request", test_handle_get_request);
    CU_add_test(suite, "test_handle_post_request", test_handle_post_request);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
