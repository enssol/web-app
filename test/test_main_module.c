/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* filepath: /devcontainer/web-app/test/test_main_module.c */

/* System headers */
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

/* CUnit header */
#include <CUnit/CUnit.h>

/* Application headers */
#include "../include/init.h"
#include "../include/fs.h"
#include "../include/app_error.h"
#include "../include/shell.h"
#include "../include/process.h"
#include "../include/scheduler.h"
#include "../include/mem.h"
#include "../include/cache.h"
#include "../include/logging.h"
#include "../include/constants.h"
#include "../include/config.h"
#include "../include/env.h"
#include "test_suite.h"

/* Function prototypes */
void test_system_memory_stress(void);  /* Changed from test_memory_stress */
void test_cache_fs_integration(void);
static void *test_task(void *arg) __attribute__((unused));
size_t memGetUsage(void); /* Add prototype for memGetUsage */

/* Test constants */
#define TEST_LOG_PATH "test/test.log"
#define TEST_CONFIG_PATH "test/test.conf"
#define TEST_ENV_PATH "test/test.env"
#define TEST_ROOT_PATH "test/fs_root"
#define TEST_FILE_PATH "test/fs_root/test.txt"
#define TEST_BLOCK_SIZE (1024 * 1024)  /* 1MB */
#define TEST_ITERATIONS 1000
#define TEST_CACHE_SIZE 100

/* Test task function prototype */
static void *test_task(void *arg)
{
    (void)arg; /* Unused parameter */
    return NULL;
}

/* Test fixture */
static int setup(void)
{
    struct stat st;

    /* Create test directories if they don't exist */
    if (stat("test", &st) == -1) {
        if (mkdir("test", 0755) == -1) {
            return -1;
        }
    }

    if (stat(TEST_ROOT_PATH, &st) == -1) {
        if (mkdir(TEST_ROOT_PATH, 0755) == -1) {
            return -1;
        }
    }

    /* Initialize error logging */
    errorInit(TEST_LOG_PATH);
    return 0;
}

static int teardown(void)
{
    remove(TEST_LOG_PATH);
    remove(TEST_FILE_PATH);
    rmdir(TEST_ROOT_PATH);
    return 0;
}

/* Test cases */
void test_main_startup(void)
{
    int status;

    /* Initialize subsystems in correct order */
    status = constants_init();
    CU_ASSERT_EQUAL(status, 0);

    status = memInit(MEM_POOL_SIZE);
    CU_ASSERT_EQUAL(status, 0);
    ASSERT_MEM_STATUS(MEM_SUCCESS);

    status = cacheInit(CACHE_TYPE_LRU, CACHE_MAX_ENTRIES);
    CU_ASSERT_EQUAL(status, 0);

    status = fsInit(TEST_ROOT_PATH);
    CU_ASSERT_EQUAL(status, FS_SUCCESS);

    status = processInit();
    CU_ASSERT_EQUAL(status, 0);

    status = scheduler_init();  /* Changed from schedulerInit */
    CU_ASSERT_EQUAL(status, SCHEDULER_SUCCESS);

    /* Cleanup in reverse order */
    scheduler_cleanup();  /* Changed from schedulerCleanup */
    processCleanup();
    cacheCleanup();
    memCleanup();
}

void test_system_memory_stress(void)  /* Changed from test_memory_stress */
{
    void *blocks[TEST_ITERATIONS];
    size_t i;
    int status;

    /* Initialize memory subsystem */
    status = memInit(TEST_BLOCK_SIZE * 2);
    CU_ASSERT_EQUAL(status, 0);

    /* Allocate blocks */
    for (i = 0; i < TEST_ITERATIONS; i++) {
        blocks[i] = memAlloc(1024);  /* 1KB blocks */
        if (blocks[i] != NULL) {
            memset(blocks[i], 0xFF, 1024);
        }
    }

    /* Free blocks in reverse order */
    for (i = TEST_ITERATIONS; i > 0; i--) {
        if (blocks[i-1] != NULL) {
            memFree(blocks[i-1]);
        }
    }

    /* Verify memory state */
    CU_ASSERT_EQUAL(memGetUsage(), 0);
    ASSERT_MEM_STATUS(MEM_SUCCESS);

    memCleanup();
}

void test_system_integration(void)
{
    int status;
    char test_data[] = "test data";
    char buffer[1024];
    size_t size = sizeof(buffer);
    const char *cache_key = "test_key";

    /* Initialize all subsystems */
    status = constants_init();
    CU_ASSERT_EQUAL(status, 0);

    status = memInit(MEM_POOL_SIZE);
    CU_ASSERT_EQUAL(status, 0);

    status = cacheInit(CACHE_TYPE_LRU, TEST_CACHE_SIZE);
    CU_ASSERT_EQUAL(status, 0);

    status = fsInit(TEST_ROOT_PATH);
    CU_ASSERT_EQUAL(status, FS_SUCCESS);

    /* Test integration between FS and Cache */
    status = fsWriteFile(TEST_FILE_PATH, test_data, strlen(test_data));
    CU_ASSERT(status > 0);

    status = cacheSet(cache_key, test_data, strlen(test_data), 60);
    CU_ASSERT_EQUAL(status, 0);

    memset(buffer, 0, sizeof(buffer));
    status = cacheGet(cache_key, buffer, &size);
    CU_ASSERT_EQUAL(status, 0);
    CU_ASSERT_STRING_EQUAL(buffer, test_data);

    /* Cleanup */
    cacheCleanup();
    memCleanup();
}

void test_cache_fs_integration(void)
{
    int status;
    const char *test_data = "test data";
    char buffer[1024];
    size_t size = sizeof(buffer);

    /* Initialize subsystems */
    status = memInit(MEM_POOL_SIZE);
    CU_ASSERT_EQUAL(status, 0);

    status = cacheInit(CACHE_TYPE_LRU, TEST_CACHE_SIZE);
    CU_ASSERT_EQUAL(status, 0);

    status = fsInit(TEST_ROOT_PATH);
    CU_ASSERT_EQUAL(status, FS_SUCCESS);

    /* Write file and cache its content */
    status = fsWriteFile(TEST_FILE_PATH, test_data, strlen(test_data));
    CU_ASSERT(status > 0);

    status = cacheSet(TEST_FILE_PATH, test_data, strlen(test_data), 60);
    CU_ASSERT_EQUAL(status, 0);

    /* Verify cache hit */
    status = cacheGet(TEST_FILE_PATH, buffer, &size);
    CU_ASSERT_EQUAL(status, 0);
    CU_ASSERT_STRING_EQUAL(buffer, test_data);

    /* Cleanup */
    cacheCleanup();
    memCleanup();
}

void test_config_env_integration(void)
{
    int status;
    char value[MAX_ENV_VALUE];
    const char *test_value = "test_value";

    /* Initialize config and env subsystems */
    status = configInit(TEST_CONFIG_PATH);
    CU_ASSERT_EQUAL(status, 0);

    status = envInit(TEST_ENV_PATH);
    CU_ASSERT_EQUAL(status, 0);

    /* Test environment variable setting and retrieval */
    status = envSet("TEST_KEY", test_value);
    CU_ASSERT_EQUAL(status, 0);

    status = envGet("TEST_KEY", value, sizeof(value));
    CU_ASSERT_EQUAL(status, 0);
    CU_ASSERT_STRING_EQUAL(value, test_value);

    /* Cleanup */
    envCleanup();
    configCleanup();
}

/* Test suite initialization */
int test_main_module(void)
{
    CU_pSuite suite;

    suite = CU_add_suite("Main Module Tests", setup, teardown);
    if (suite == NULL) {
        return -1;
    }

    /* Add existing tests */
    if ((CU_add_test(suite, "Main Startup", test_main_startup) == NULL) ||
        (CU_add_test(suite, "System Memory Stress", test_system_memory_stress) == NULL) ||  /* Changed */
        (CU_add_test(suite, "System Integration", test_system_integration) == NULL) ||
        (CU_add_test(suite, "FS Cache Integration", test_cache_fs_integration) == NULL) ||
        (CU_add_test(suite, "Config Env Integration", test_config_env_integration) == NULL)) {
        return -1;
    }

    return 0;
}
