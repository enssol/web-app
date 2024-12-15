/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include <CUnit/CUnit.h>
#include "../include/mem.h"
#include "test_suite.h"
#include <stdlib.h>
#include <string.h>

/* Setup and teardown */
static int setup(void)
{
    return memInit(MEM_POOL_SIZE);
}

static int teardown(void)
{
    memCleanup();
    return 0;
}

/* Test cases */
void test_mem_init(void)
{
    int result;

    /* Test valid initialization */
    result = memInit(1024);
    CU_ASSERT_EQUAL(result, 0);
    CU_ASSERT_EQUAL(memGetStatus(), MEM_SUCCESS);

    memCleanup();  /* Clean up before next test */

    /* Test invalid initialization */
    result = memInit(0);
    CU_ASSERT_EQUAL(result, -1);
    CU_ASSERT_EQUAL(memGetStatus(), MEM_ERROR);
}

void test_mem_alloc(void)
{
    void *ptr;
    ptr = memAlloc(128);
    CU_ASSERT_PTR_NOT_NULL(ptr);
    CU_ASSERT_EQUAL(memGetStatus(), MEM_SUCCESS);
}

void test_mem_free(void)
{
    void *ptr = memAlloc(128);
    memFree(ptr);
    CU_ASSERT_EQUAL(memGetStatus(), MEM_SUCCESS);
}

void test_mem_stress(void)
{
    void *ptrs[100];
    size_t i;

    for (i = 0; i < 100; i++) {
        ptrs[i] = memAlloc(128);
        CU_ASSERT_PTR_NOT_NULL(ptrs[i]);
    }

    for (i = 0; i < 100; i++) {
        memFree(ptrs[i]);
    }
}

void test_mem_edge_cases(void)
{
    void *ptr1, *ptr2, *ptr3;
    size_t min_size;
    size_t max_size;

    min_size = MEM_BLOCK_MIN;
    max_size = MEM_POOL_SIZE / 2; /* Use a reasonable max size */

    /* Test minimum allocation size */
    ptr1 = memAlloc(min_size);
    CU_ASSERT_PTR_NOT_NULL(ptr1);
    CU_ASSERT_EQUAL(memGetStatus(), MEM_SUCCESS);

    /* Test maximum allocation size */
    ptr2 = memAlloc(max_size);
    CU_ASSERT_PTR_NULL(ptr2);
    CU_ASSERT_EQUAL(memGetStatus(), MEM_OUT_OF_MEMORY);

    /* Test zero size allocation */
    ptr3 = memAlloc(0);
    CU_ASSERT_PTR_NULL(ptr3);
    CU_ASSERT_EQUAL(memGetStatus(), MEM_INVALID_PARAM);

    memFree(ptr1);
}

void test_mem_fragmentation(void)
{
    void *ptrs[10];
    void *large_ptr;
    int i;
    size_t alloc_size;

    alloc_size = 128;

    /* Allocate multiple blocks */
    for (i = 0; i < 10; i++) {
        ptrs[i] = memAlloc(alloc_size);
        CU_ASSERT_PTR_NOT_NULL(ptrs[i]);
    }

    /* Free alternate blocks to create fragmentation */
    for (i = 0; i < 10; i += 2) {
        memFree(ptrs[i]);
    }

    /* Try to allocate a large block */
    large_ptr = memAlloc(alloc_size * 3);
    CU_ASSERT_PTR_NULL(large_ptr);
    CU_ASSERT_EQUAL(memGetStatus(), MEM_OUT_OF_MEMORY);

    /* Free remaining blocks */
    for (i = 1; i < 10; i += 2) {
        memFree(ptrs[i]);
    }
}

void test_mem_multi_pool(void)
{
    int result;
    void *ptr1, *ptr2;

    /* Initialize second pool */
    result = memInit(1024);
    CU_ASSERT_EQUAL(result, 0);

    /* Allocate from first pool */
    ptr1 = memAlloc(256);
    CU_ASSERT_PTR_NOT_NULL(ptr1);

    /* Allocate from second pool */
    ptr2 = memAlloc(256);
    CU_ASSERT_PTR_NOT_NULL(ptr2);

    /* Free memory */
    memFree(ptr1);
    memFree(ptr2);
}

/* Test suite initialization */
int test_mem(void)
{
    CU_pSuite suite;

    suite = CU_add_suite("Memory Management Tests", setup, teardown);
    if (suite == NULL) {
        return -1;
    }

    if ((CU_add_test(suite, "Memory Init", test_mem_init) == NULL) ||
        (CU_add_test(suite, "Memory Alloc", test_mem_alloc) == NULL) ||
        (CU_add_test(suite, "Memory Free", test_mem_free) == NULL) ||
        (CU_add_test(suite, "Memory Stress", test_mem_stress) == NULL) ||
        (CU_add_test(suite, "Memory Edge Cases", test_mem_edge_cases) == NULL) ||
        (CU_add_test(suite, "Memory Fragmentation", test_mem_fragmentation) == NULL) ||
        (CU_add_test(suite, "Memory Multi Pool", test_mem_multi_pool) == NULL)) {
        return -1;
    }

    return 0;
}
