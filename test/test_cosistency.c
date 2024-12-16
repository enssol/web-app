/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */
/* filepath: test/test_consistency.c */
#include <CUnit/CUnit.h>
#include <sys/stat.h>
#include <limits.h>
#include "../include/constants.h"
#include "../include/common.h"
#include "../include/version.h"
#include "../include/env.h"
#include "test_suite.h"

/* Function prototypes */
void test_path_consistency(void);
void test_buffer_sizes(void);
void test_version_consistency(void);
void test_env_consistency(void);
int test_consistency(void);

/* Test implementations */
void
test_path_consistency(void)
{
    struct stat st = {0}; /* Initialize to avoid uninitialized warning */

    /* Test existence of configuration paths */
    CU_ASSERT_EQUAL(stat(DEFAULT_CONFIG_PATH, &st), 0);
    CU_ASSERT_EQUAL(stat(DEFAULT_LOG_PATH, &st), 0);
    CU_ASSERT_EQUAL(stat(DEFAULT_ENV_PATH, &st), 0);
}

void
test_buffer_sizes(void)
{
    /* Test buffer size relationships */
    CU_ASSERT(MAX_LINE_LENGTH >= 0);
    CU_ASSERT(MAX_ENV_VALUE >= MAX_ENV_KEY);
    CU_ASSERT(MAX_CONFIG_LINE >= MAX_LINE_LENGTH);
}

void
test_version_consistency(void)
{
    /* Test API version consistency */
    CU_ASSERT_EQUAL(CACHE_API_VERSION, CACHE_API_VERSION_REQUIRED);
    CU_ASSERT_EQUAL(MEM_API_VERSION, MEM_API_VERSION_REQUIRED);
}

void
test_env_consistency(void)
{
    char value[MAX_ENV_VALUE];
    int status;

    /* Test environment variable access */
    status = envGet("APP_NAME", value, sizeof(value));
    CU_ASSERT_EQUAL(status, ENV_SUCCESS);
    CU_ASSERT_STRING_EQUAL(value, APP_NAME);

    status = envGet("APP_VERSION", value, sizeof(value));
    CU_ASSERT_EQUAL(status, ENV_SUCCESS);
    CU_ASSERT_STRING_EQUAL(value, APP_VERSION);
}

/* Test suite initialization */
int
test_consistency(void)
{
    CU_pSuite suite = NULL;

    /* Add suite to registry */
    suite = CU_add_suite("Consistency Tests", NULL, NULL);
    if (NULL == suite) {
        return -1;
    }

    /* Add tests to suite */
    if ((NULL == CU_add_test(suite, "Path Consistency", test_path_consistency)) ||
        (NULL == CU_add_test(suite, "Buffer Sizes", test_buffer_sizes)) ||
        (NULL == CU_add_test(suite, "Version Consistency", test_version_consistency)) ||
        (NULL == CU_add_test(suite, "Environment Consistency", test_env_consistency))) {
        return -1;
    }

    return 0;
}
