/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* src/mem.c */
#include "../include/mem.h"
#include "../include/app_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#ifdef DEBUG
#define DEBUG_LOG(x) fprintf(stderr, "[DEBUG][%s:%d] %s\n", __FILE__, __LINE__, x)
#else
#define DEBUG_LOG(x) ((void)0)
#endif

struct block_header {
    size_t size;
    int free;
    struct block_header *next;
};

struct memory_pool {
    void *base;
    size_t size;
    struct block_header *first_block;
    size_t used;
};

static struct memory_pool pools[MEM_MAX_POOLS];
static size_t pool_count = 0;
static enum mem_status last_status = MEM_SUCCESS;
static pthread_mutex_t mem_mutex = PTHREAD_MUTEX_INITIALIZER;

#define ALIGN_SIZE(s) (((s) + sizeof(void*) - 1) & ~(sizeof(void*) - 1))
#define MIN_BLOCK_SIZE (sizeof(struct block_header) + ALIGN_SIZE(MEM_BLOCK_MIN))
#define TOTAL_SIZE(s) (sizeof(struct block_header) + ALIGN_SIZE(s))

int
memInit(size_t pool_size)
{
    /* Add API version check */
    if (MEM_API_VERSION != MEM_API_VERSION_REQUIRED) {
        last_status = MEM_ERROR;
        return -1;
    }

    void *pool_base;
    struct block_header *first_block;
    size_t min_size;
    size_t aligned_pool_size;
    size_t max_allowed_size;

    /* Calculate minimum pool size */
    min_size = sizeof(struct block_header) + /* Initial header */
               (sizeof(struct block_header) + ALIGN_SIZE(128)) * 100 + /* 100 allocations */
               ALIGN_SIZE(MEM_BLOCK_MIN); /* Minimum extra space */

    /* Calculate maximum allowed size to prevent overflow */
    max_allowed_size = (size_t)-1 - sizeof(struct block_header);

    /* Validate input size */
    if (pool_size == 0) {
        last_status = MEM_ERROR;
        return -1;
    }

    if (pool_count >= MEM_MAX_POOLS) {
        last_status = MEM_POOL_FULL;
        return -1;
    }

    /* Ensure size is within valid range */
    if (pool_size < min_size) {
        pool_size = min_size;
    } else if (pool_size > max_allowed_size) {
        last_status = MEM_ERROR;
        return -1;
    }

    /* Align pool size and check for overflow */
    aligned_pool_size = ALIGN_SIZE(pool_size);
    if (aligned_pool_size < pool_size) { /* Check for overflow */
        last_status = MEM_ERROR;
        return -1;
    }

    pthread_mutex_lock(&mem_mutex);

    pool_base = malloc(aligned_pool_size);
    if (pool_base == NULL) {
        pthread_mutex_unlock(&mem_mutex);
        last_status = MEM_OUT_OF_MEMORY;
        return -1;
    }

    /* Initialize first block */
    first_block = (struct block_header *)pool_base;
    first_block->size = aligned_pool_size - sizeof(struct block_header);
    first_block->free = 1;
    first_block->next = NULL;

    /* Initialize pool */
    pools[pool_count].base = pool_base;
    pools[pool_count].size = aligned_pool_size;
    pools[pool_count].first_block = first_block;
    pools[pool_count].used = sizeof(struct block_header);

    pool_count++;
    last_status = MEM_SUCCESS;
    pthread_mutex_unlock(&mem_mutex);
    return 0;
}

void *
memAlloc(size_t size)
{
    size_t i;
    struct block_header *block;
    struct block_header *new_block;
    size_t aligned_size;
    size_t block_size;
    size_t remaining_size;

    DEBUG_LOG("Attempting allocation");

    if (size == 0) {
        last_status = MEM_INVALID_PARAM;
        return NULL;
    }

    if (pool_count == 0) {
        last_status = MEM_ERROR;
        return NULL;
    }

    aligned_size = ALIGN_SIZE(size);
    block_size = sizeof(struct block_header) + aligned_size;

    pthread_mutex_lock(&mem_mutex);

    /* Search pools for suitable block */
    for (i = 0; i < pool_count; i++) {
        block = pools[i].first_block;

        while (block != NULL) {
            if (block->free && block->size >= aligned_size) {
                remaining_size = block->size - aligned_size;

                /* Split block if enough space remains */
                if (remaining_size >= MIN_BLOCK_SIZE) {
                    new_block = (struct block_header *)((char *)block + block_size);
                    new_block->size = remaining_size - sizeof(struct block_header);
                    new_block->free = 1;
                    new_block->next = block->next;

                    block->size = aligned_size;
                    block->next = new_block;
                }

                block->free = 0;
                pools[i].used += block_size;

                pthread_mutex_unlock(&mem_mutex);
                last_status = MEM_SUCCESS;
                return (void *)((char *)block + sizeof(struct block_header));
            }
            block = block->next;
        }
    }

    pthread_mutex_unlock(&mem_mutex);
    last_status = MEM_OUT_OF_MEMORY;
    return NULL;
}

void
memFree(void *ptr)
{
    size_t i;
    struct block_header *block;
    struct block_header *next;

    if (ptr == NULL) {
        last_status = MEM_INVALID_PARAM;
        return;
    }

    pthread_mutex_lock(&mem_mutex);

    /* Find block in pools */
    for (i = 0; i < pool_count; i++) {
        if ((char *)ptr > (char *)pools[i].base &&
            (char *)ptr < (char *)pools[i].base + pools[i].size) {

            block = (struct block_header *)((char *)ptr - sizeof(struct block_header));
            block->free = 1;
            pools[i].used -= sizeof(struct block_header) + block->size;

            /* Coalesce with next block if free */
            next = block->next;
            if (next != NULL && next->free) {
                block->size += sizeof(struct block_header) + next->size;
                block->next = next->next;
            }

            pthread_mutex_unlock(&mem_mutex);
            last_status = MEM_SUCCESS;
            return;
        }
    }

    pthread_mutex_unlock(&mem_mutex);
    last_status = MEM_ERROR;
}

size_t
memGetUsage(void)
{
    size_t total_used;
    size_t i;

    pthread_mutex_lock(&mem_mutex);

    total_used = 0;
    for (i = 0; i < pool_count; i++) {
        total_used += pools[i].used;
    }

    pthread_mutex_unlock(&mem_mutex);
    return total_used;
}

void
memCleanup(void)
{
    size_t i;

    pthread_mutex_lock(&mem_mutex);

    for (i = 0; i < pool_count; i++) {
        free(pools[i].base);
        pools[i].base = NULL;
        pools[i].size = 0;
        pools[i].first_block = NULL;
        pools[i].used = 0;
    }
    pool_count = 0;
    last_status = MEM_SUCCESS;

    pthread_mutex_unlock(&mem_mutex);
}

enum mem_status
memGetStatus(void)
{
    return last_status;
}

void memDumpStats(void)
{
    size_t i;
    struct block_header *block;

    for (i = 0; i < pool_count; i++) {
        printf("Pool %lu:\n", (unsigned long)i);
        printf("  Base: %p\n", pools[i].base);
        printf("  Size: %lu\n", (unsigned long)pools[i].size);
        printf("  Used: %lu\n", (unsigned long)pools[i].used);

        block = pools[i].first_block;
        while (block != NULL) {
            printf("  Block %p:\n", (void*)block);
            printf("    Size: %lu\n", (unsigned long)block->size);
            printf("    Free: %d\n", block->free);
            block = block->next;
        }
    }
}

void memVisualizeBlocks(void)
{
    size_t i;
    struct block_header *block;

    for (i = 0; i < pool_count; i++) {
        printf("Pool %lu: ", (unsigned long)i);
        block = pools[i].first_block;
        while (block != NULL) {
            printf("[%c|%lu]->", block->free ? 'F' : 'U',
                   (unsigned long)block->size);
            block = block->next;
        }
        printf("NULL\n");
    }
}
