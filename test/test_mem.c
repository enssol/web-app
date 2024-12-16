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

void test_mem_stress_allocation(void)
{
    void *ptrs[1000];
    size_t sizes[1000];
    size_t i, total_allocated = 0;

    /* Random allocations of varying sizes */
    for (i = 0; i < 1000; i++) {
        sizes[i] = (i % 128) + 1; /* 1 to 128 bytes */
        ptrs[i] = memAlloc(sizes[i]);
        if (ptrs[i] != NULL) {
            total_allocated += sizes[i];
            /* Write to memory to verify access */
            memset(ptrs[i], 0xFF, sizes[i]);
        }
    }

    /* Verify allocations */
    CU_ASSERT(total_allocated > 0);
    CU_ASSERT(memGetUsage() >= total_allocated);

    /* Random deallocation pattern */
    for (i = 0; i < 1000; i += 2) {
        memFree(ptrs[i]);
    }

    /* Reallocate freed blocks */
    for (i = 0; i < 1000; i += 2) {
        ptrs[i] = memAlloc(sizes[i]);
        CU_ASSERT_PTR_NOT_NULL(ptrs[i]);
    }

    /* Cleanup */
    for (i = 0; i < 1000; i++) {
        memFree(ptrs[i]);
    }
}

void test_mem_edge_cases(void)
{
    void *ptr1, *ptr2;
    size_t max_size = MEM_POOL_SIZE - sizeof(struct block_header);

    /* Test NULL and invalid sizes */
    ptr1 = memAlloc(0);
    CU_ASSERT_PTR_NULL(ptr1);
    CU_ASSERT_EQUAL(memGetStatus(), MEM_INVALID_PARAM);

    /* Test maximum allocation */
    ptr1 = memAlloc(max_size);
    CU_ASSERT_PTR_NOT_NULL(ptr1);

    /* Test allocation when pool is full */
    ptr2 = memAlloc(16);
    CU_ASSERT_PTR_NULL(ptr2);
    CU_ASSERT_EQUAL(memGetStatus(), MEM_OUT_OF_MEMORY);

    /* Test freeing NULL pointer */
    memFree(NULL);
    CU_ASSERT_EQUAL(memGetStatus(), MEM_INVALID_PARAM);

    /* Test double free */
    memFree(ptr1);
    memFree(ptr1);
    CU_ASSERT_EQUAL(memGetStatus(), MEM_INVALID_PARAM);
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

void test_mem_corruption_detection(void)
{
    void *ptr1, *ptr2;
    const size_t size = 128;
    unsigned char *buffer;

    /* Initialize memory system */
    CU_ASSERT_EQUAL(memInit(MEM_POOL_SIZE), 0);

    /* Allocate test memory */
    ptr1 = memAlloc(size);
    CU_ASSERT_PTR_NOT_NULL(ptr1);

    /* Attempt buffer overflow */
    buffer = (unsigned char *)ptr1;
    buffer[size] = 0xFF;  /* Write beyond allocated space */

    /* Allocate another block to trigger corruption check */
    ptr2 = memAlloc(size);
    CU_ASSERT_PTR_NULL(ptr2);
    CU_ASSERT_EQUAL(memGetStatus(), MEM_ERROR);

    memFree(ptr1);
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
        (CU_add_test(suite, "Memory Stress Allocation", test_mem_stress_allocation) == NULL) ||
        (CU_add_test(suite, "Memory Edge Cases", test_mem_edge_cases) == NULL) ||
        (CU_add_test(suite, "Memory Fragmentation", test_mem_fragmentation) == NULL) ||
        (CU_add_test(suite, "Memory Multi Pool", test_mem_multi_pool) == NULL) ||
        (CU_add_test(suite, "Memory Corruption Detection", test_mem_corruption_detection) == NULL)) {
        return -1;
    }

    return 0;
}
