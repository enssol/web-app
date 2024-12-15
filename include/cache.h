/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* include/cache.h */
#ifndef CACHE_H
#define CACHE_H

#include <sys/types.h>

/* Constants */
#define CACHE_MAX_KEY_SIZE 128
#define CACHE_MAX_VALUE_SIZE 4096
#define CACHE_MAX_ENTRIES 1024

/* Cache types */
enum cache_type {
    CACHE_TYPE_LRU,
    CACHE_TYPE_LFU
};

/* Cache status codes */
enum cache_status {
    CACHE_SUCCESS,
    CACHE_ERROR,
    CACHE_NOT_FOUND,
    CACHE_INVALID_PARAM,
    CACHE_FULL
};

int cacheInit(enum cache_type type, size_t max_entries);
int cacheSet(const char *key, const void *value, size_t value_size, int ttl);
int cacheGet(const char *key, void *value, size_t *value_size);
int cacheDelete(const char *key);
void cacheCleanup(void);
size_t cacheGetSize(void);
enum cache_status cacheGetStatus(void);

#endif /* CACHE_H */
