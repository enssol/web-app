/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include <CUnit/CUnit.h>
#include "../include/cache.h"
#include "test_suite.h"
#include <stdlib.h>
#include <string.h>

/* Test fixture */
static const char *TEST_KEY = "test_key";
static const char *TEST_VALUE = "test_value";

/* Setup and teardown */
static int setup(void)
{
    return cacheInit(CACHE_TYPE_LRU, 16);
}

static int teardown(void)
{
    cacheCleanup();
    return 0;
}

/* Test cases */
void test_cache_init(void)
{
    CU_ASSERT_EQUAL(cacheInit(CACHE_TYPE_LRU, 16), 0);
    CU_ASSERT_EQUAL(cacheInit(CACHE_TYPE_LFU, 0), -1);
}

void test_cache_set_get(void)
{
    char value[64];
    size_t size = sizeof(value);

    CU_ASSERT_EQUAL(cacheSet(TEST_KEY, TEST_VALUE, strlen(TEST_VALUE) + 1, 60), 0);
    CU_ASSERT_EQUAL(cacheGet(TEST_KEY, value, &size), 0);
    CU_ASSERT_STRING_EQUAL(value, TEST_VALUE);
}

void test_cache_delete(void)
{
    char value[64];
    size_t size = sizeof(value);

    cacheSet(TEST_KEY, TEST_VALUE, strlen(TEST_VALUE) + 1, 60);
    CU_ASSERT_EQUAL(cacheDelete(TEST_KEY), 0);
    CU_ASSERT_EQUAL(cacheGet(TEST_KEY, value, &size), -1);
}

void test_cache_capacity(void)
{
    char key[16];
    char value[64];
    size_t i;
    size_t size;
    int result;

    for (i = 0; i < 20; i++) {
        /* Use %lu instead of %zu for C90 compliance */
        sprintf(key, "key%lu", (unsigned long)i);
        result = cacheSet(key, TEST_VALUE, strlen(TEST_VALUE) + 1, 60);
        CU_ASSERT_EQUAL(result, 0);
    }

    /* Verify first entry was evicted due to capacity */
    size = sizeof(value);
    result = cacheGet("key0", value, &size);
    CU_ASSERT_EQUAL(result, -1);
}

void test_cache_lru(void)
{
    char value[64];
    size_t size;

    cacheInit(CACHE_TYPE_LRU, 2);

    cacheSet("key1", "value1", 7, 60);
    cacheSet("key2", "value2", 7, 60);

    /* Access key1 to make it most recently used */
    size = sizeof(value);
    cacheGet("key1", value, &size);

    /* Add key3, should evict key2 */
    cacheSet("key3", "value3", 7, 60);

    size = sizeof(value);
    CU_ASSERT_EQUAL(cacheGet("key2", value, &size), -1);
    CU_ASSERT_EQUAL(cacheGet("key1", value, &size), 0);
    CU_ASSERT_EQUAL(cacheGet("key3", value, &size), 0);
}

void test_cache_lfu(void)
{
    char value[64];
    size_t size;

    cacheInit(CACHE_TYPE_LFU, 2);

    cacheSet("key1", "value1", 7, 60);
    cacheSet("key2", "value2", 7, 60);

    /* Access key1 multiple times */
    size = sizeof(value);
    cacheGet("key1", value, &size);
    cacheGet("key1", value, &size);

    /* Add key3, should evict key2 */
    cacheSet("key3", "value3", 7, 60);

    size = sizeof(value);
    CU_ASSERT_EQUAL(cacheGet("key2", value, &size), -1);
    CU_ASSERT_EQUAL(cacheGet("key1", value, &size), 0);
    CU_ASSERT_EQUAL(cacheGet("key3", value, &size), 0);
}

/* Add these test functions */

void test_cache_edge_cases(void)
{
    char value[CACHE_MAX_VALUE_SIZE + 1];
    char stored_value[CACHE_MAX_VALUE_SIZE + 1];
    char long_key[CACHE_MAX_KEY_SIZE + 1];
    size_t value_size;

    value_size = sizeof(stored_value);

    /* Test NULL inputs */
    CU_ASSERT_EQUAL(cacheSet(NULL, "test", 5, 60), -1);
    CU_ASSERT_EQUAL(cacheGetStatus(), CACHE_INVALID_PARAM);

    CU_ASSERT_EQUAL(cacheSet("test", NULL, 0, 60), -1);
    CU_ASSERT_EQUAL(cacheGetStatus(), CACHE_INVALID_PARAM);

    CU_ASSERT_EQUAL(cacheGet("test", NULL, &value_size), -1);
    CU_ASSERT_EQUAL(cacheGetStatus(), CACHE_INVALID_PARAM);

    /* Test maximum key size */
    memset(long_key, 'A', CACHE_MAX_KEY_SIZE);
    long_key[CACHE_MAX_KEY_SIZE] = '\0';

    CU_ASSERT_EQUAL(cacheSet(long_key, "test", 5, 60), -1);
    CU_ASSERT_EQUAL(cacheGetStatus(), CACHE_INVALID_PARAM);

    /* Test empty key */
    CU_ASSERT_EQUAL(cacheSet("", "test", 5, 60), -1);
    CU_ASSERT_EQUAL(cacheGetStatus(), CACHE_INVALID_PARAM);

    /* Test maximum value size */
    memset(value, 'B', CACHE_MAX_VALUE_SIZE);
    value[CACHE_MAX_VALUE_SIZE] = '\0';
    CU_ASSERT_EQUAL(cacheSet("test", value, CACHE_MAX_VALUE_SIZE + 1, 60), -1);
    CU_ASSERT_EQUAL(cacheGetStatus(), CACHE_INVALID_PARAM);

    /* Test zero TTL */
    CU_ASSERT_EQUAL(cacheSet("test", "value", 6, 0), 0);

    /* Test value size boundary */
    value_size = 0;
    CU_ASSERT_EQUAL(cacheGet("test", stored_value, &value_size), -1);
    CU_ASSERT_EQUAL(cacheGetStatus(), CACHE_INVALID_PARAM);
}

void test_cache_eviction_policy(void)
{
    char key[16];
    char value[64];
    size_t size;
    int i;

    /* Initialize cache with small capacity */
    cacheInit(CACHE_TYPE_LRU, 3);

    /* Fill cache */
    for (i = 0; i < 3; i++) {
        sprintf(key, "key%d", i);
        CU_ASSERT_EQUAL(cacheSet(key, "value", 6, 60), 0);
    }

    /* Access key1 to make it most recently used */
    size = sizeof(value);
    CU_ASSERT_EQUAL(cacheGet("key1", value, &size), 0);

    /* Add new entry to trigger eviction */
    CU_ASSERT_EQUAL(cacheSet("key3", "value", 6, 60), 0);

    /* Verify key0 was evicted (LRU) */
    size = sizeof(value);
    CU_ASSERT_EQUAL(cacheGet("key0", value, &size), -1);
    CU_ASSERT_EQUAL(cacheGetStatus(), CACHE_NOT_FOUND);

    /* Verify key1 is still present */
    size = sizeof(value);
    CU_ASSERT_EQUAL(cacheGet("key1", value, &size), 0);
}

void test_cache_concurrent_access(void)
{
    char value[64];
    size_t size;

    /* Set initial value */
    CU_ASSERT_EQUAL(cacheSet("test", "initial", 8, 60), 0);

    /* Update value */
    CU_ASSERT_EQUAL(cacheSet("test", "updated", 8, 60), 0);

    /* Verify update */
    size = sizeof(value);
    CU_ASSERT_EQUAL(cacheGet("test", value, &size), 0);
    CU_ASSERT_STRING_EQUAL(value, "updated");

    /* Delete entry */
    CU_ASSERT_EQUAL(cacheDelete("test"), 0);

    /* Verify deletion */
    size = sizeof(value);
    CU_ASSERT_EQUAL(cacheGet("test", value, &size), -1);
    CU_ASSERT_EQUAL(cacheGetStatus(), CACHE_NOT_FOUND);
}

/* Test suite initialization */
int test_cache(void)
{
    CU_pSuite suite;

    suite = CU_add_suite("Cache Tests", setup, teardown);
    if (suite == NULL) {
        return -1;
    }

    if ((CU_add_test(suite, "Cache Init", test_cache_init) == NULL) ||
        (CU_add_test(suite, "Cache Set/Get", test_cache_set_get) == NULL) ||
        (CU_add_test(suite, "Cache Delete", test_cache_delete) == NULL) ||
        (CU_add_test(suite, "Cache Capacity", test_cache_capacity) == NULL) ||
        (CU_add_test(suite, "Cache LRU", test_cache_lru) == NULL) ||
        (CU_add_test(suite, "Cache LFU", test_cache_lfu) == NULL) ||
        (CU_add_test(suite, "Cache Edge Cases", test_cache_edge_cases) == NULL) ||
        (CU_add_test(suite, "Cache Eviction Policy", test_cache_eviction_policy) == NULL) ||
        (CU_add_test(suite, "Cache Concurrent Access", test_cache_concurrent_access) == NULL)) {
        return -1;
    }

    return 0;
}
