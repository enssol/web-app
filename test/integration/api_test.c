#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <CUnit/Basic.h>

// Define constants
#define BASE_URL "http://localhost:8080"

// Function prototypes
int init_suite(void);
int clean_suite(void);
void test_get_endpoint(void);
void test_post_endpoint(void);
void test_put_endpoint(void);
void test_delete_endpoint(void);

// Setup function
int
init_suite(void)
{
    // Initialize CURL
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != 0) {
        return -1;
    }
    return 0;
}

// Teardown function
int
clean_suite(void)
{
    // Cleanup CURL
    curl_global_cleanup();
    return 0;
}

// Test GET endpoint
void
test_get_endpoint(void)
{
    CURL *curl;
    CURLcode res;
    long response_code;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, BASE_URL "/api/get");
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L); // HEAD request
        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            CU_ASSERT(response_code == 200);
        } else {
            CU_FAIL("GET request failed");
        }
        curl_easy_cleanup(curl);
    } else {
        CU_FAIL("Failed to initialize CURL");
    }
}

// Test POST endpoint
void
test_post_endpoint(void)
{
    CURL *curl;
    CURLcode res;
    long response_code;
    const char *post_data = "field1=value1&field2=value2";

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, BASE_URL "/api/post");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            CU_ASSERT(response_code == 200);
        } else {
            CU_FAIL("POST request failed");
        }
        curl_easy_cleanup(curl);
    } else {
        CU_FAIL("Failed to initialize CURL");
    }
}

// Test PUT endpoint
void
test_put_endpoint(void)
{
    CURL *curl;
    CURLcode res;
    long response_code;
    const char *put_data = "field1=value1&field2=value2";

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, BASE_URL "/api/put");
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, put_data);
        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            CU_ASSERT(response_code == 200);
        } else {
            CU_FAIL("PUT request failed");
        }
        curl_easy_cleanup(curl);
    } else {
        CU_FAIL("Failed to initialize CURL");
    }
}

// Test DELETE endpoint
void
test_delete_endpoint(void)
{
    CURL *curl;
    CURLcode res;
    long response_code;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, BASE_URL "/api/delete");
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            CU_ASSERT(response_code == 200);
        } else {
            CU_FAIL("DELETE request failed");
        }
        curl_easy_cleanup(curl);
    } else {
        CU_FAIL("Failed to initialize CURL");
    }
}

// Main function to run tests
int
main(void)
{
    CU_pSuite pSuite = NULL;

    // Initialize CUnit test registry
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    // Add a suite to the registry
    pSuite = CU_add_suite("API Test Suite", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add tests to the suite
    if ((NULL == CU_add_test(pSuite, "test of GET /api/get", test_get_endpoint)) ||
        (NULL == CU_add_test(pSuite, "test of POST /api/post", test_post_endpoint)) ||
        (NULL == CU_add_test(pSuite, "test of PUT /api/put", test_put_endpoint)) ||
        (NULL == CU_add_test(pSuite, "test of DELETE /api/delete", test_delete_endpoint))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
