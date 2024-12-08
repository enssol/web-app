/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stdlib.h>

/* Dictionary structure for headers */
typedef struct
{
    char **keys;
    char **values;
    size_t size;
} Dictionary;

/* Request structure */
typedef struct
{
    Dictionary headers;
    char *url;
} Request;

/* Response structure */
typedef struct
{
    int status_code;
    char *body;
} Response;

/* UserCredentials structure */
typedef struct
{
    char *username;
    char *password;
} UserCredentials;

/* Function declarations */
void initDictionary (Dictionary *dict);
void freeDictionary (Dictionary *dict);
void addHeader (Dictionary *dict, const char *key, const char *value);

#endif /* DATA_STRUCTURES_H */
