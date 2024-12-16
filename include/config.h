/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* include/config.h */
#ifndef CONFIG_H
#define CONFIG_H

/* Dependencies */
#include "common.h"
#include "cache.h"
#include "fs.h"

/* Constants */
#define MAX_ENV_VALUE 1024
#define MAX_ENV_KEY 256

/* Configuration status codes */
enum config_status {
    CONFIG_SUCCESS,
    CONFIG_FILE_ERROR,
    CONFIG_PARSE_ERROR,
    CONFIG_NOT_FOUND,
    CONFIG_ERROR
};

/* Function prototypes */
int configInit(const char *config_path);
int configLoad(void);
int configGet(const char *key, char *value, size_t size);
void configCleanup(void);
enum config_status configGetStatus(void);

#endif /* CONFIG_H */
