/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../include/server.h"

/* Test suite setup and cleanup */
static int init_suite(void)
{
    return 0;
}

static int clean_suite(void)
{
    return 0;
}

/* Test cases */
static void test_server_init(void)
{
    struct server_config config;
    int result;

    config.port = 8080;
    config.doc_root = "/tmp/test_www";
    config.cert_file = NULL;
    config.key_file = NULL;

    /* Create test directory */
    result = mkdir(config.doc_root, (mode_t)0755);
    CU_ASSERT_EQUAL_FATAL(result, 0);

    result = server_init(&config);
    CU_ASSERT_EQUAL(result, 0);

    /* Cleanup */
    rmdir(config.doc_root);
}

static void test_handle_static_file(void)
{
    const char *test_content = "<html><body>Test</body></html>";
    char test_path[] = "/tmp/test_www/test.html";
    FILE *fp;
    struct http_response resp;
    int result;

    /* Setup test directory */
    result = mkdir("/tmp/test_www", (mode_t)0755);
    CU_ASSERT_EQUAL_FATAL(result, 0);

    /* Create test file */
    fp = fopen(test_path, "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(fp);
    fprintf(fp, "%s", test_content);
    fclose(fp);

    /* Test file handling */
    result = handle_static_file("/test.html", &resp);
    CU_ASSERT_EQUAL(result, 0);
    CU_ASSERT_EQUAL(resp.status_code, 200);

    /* Cleanup */
    unlink(test_path);
    rmdir("/tmp/test_www");
}

static void test_handle_404(void)
{
    struct http_response resp;
    int result;

    result = handle_static_file("/nonexistent.html", &resp);
    CU_ASSERT_EQUAL(result, -1);
    CU_ASSERT_EQUAL(resp.status_code, 404);
}

/* Test runner */
int main(void)
{
    CU_pSuite pSuite;

    /* Initialize CUnit registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add suite to registry */
    pSuite = CU_add_suite("Server Test Suite", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add tests to suite */
    if ((NULL == CU_add_test(pSuite, "test server initialization",
                            test_server_init)) ||
        (NULL == CU_add_test(pSuite, "test static file handling",
                            test_handle_static_file)) ||
        (NULL == CU_add_test(pSuite, "test 404 handling",
                            test_handle_404)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run tests */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
