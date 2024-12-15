/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

/* include/mem.h */
#ifndef MEM_H
#define MEM_H

#include <sys/types.h>

/* Constants */
#define MEM_POOL_SIZE (1024 * 1024)  /* 1MB default pool size */
#define MEM_BLOCK_MIN 16             /* Minimum block size */
#define MEM_MAX_POOLS 16            /* Maximum number of memory pools */

/* Memory allocation status codes */
enum mem_status {
    MEM_SUCCESS,
    MEM_ERROR,
    MEM_OUT_OF_MEMORY,
    MEM_INVALID_PARAM,
    MEM_POOL_FULL
};

/* Function prototypes */
int memInit(size_t pool_size);
void *memAlloc(size_t size);
void memFree(void *ptr);
size_t memGetUsage(void);
void memCleanup(void);
enum mem_status memGetStatus(void);

#endif /* MEM_H */
