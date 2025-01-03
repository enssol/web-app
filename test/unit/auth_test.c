#include "auth.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

void test_user_authenticator(void)
{
    struct user_credentials creds;
    int result;

    creds.username = "testuser";
    creds.password = "testpass";

    result = user_authenticator(&creds);
    CU_ASSERT(result == 0);
}

void test_auth_token_generator(void)
{
    char token[256];
    int result;

    result = auth_token_generator("testuser", token, sizeof(token));
    CU_ASSERT(result == 0);
}

void test_auth_token_validator(void)
{
    int result;

    result = auth_token_validator("valid_token");
    CU_ASSERT(result == 0);
}

int main()
{
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Auth Test Suite", 0, 0);

    CU_add_test(suite, "test_user_authenticator", test_user_authenticator);
    CU_add_test(suite, "test_auth_token_generator", test_auth_token_generator);
    CU_add_test(suite, "test_auth_token_validator", test_auth_token_validator);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
