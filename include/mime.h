/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */
/* filepath: include/mime.h */
#ifndef MIME_H
#define MIME_H

/* Dependencies */
#include "common.h"

/* Constants */
#define MIME_MAX_TYPE_LEN 128
#define MIME_MAX_EXT_LEN 16
#define MIME_MAX_TYPES 50
#define MIME_DEFAULT_TYPE "application/octet-stream"

/* Error codes */
enum mime_error {
    MIME_SUCCESS = 0,
    MIME_ERROR = -1,
    MIME_NOT_FOUND = -2,
    MIME_INVALID_PARAM = -3,
    MIME_TOO_MANY_TYPES = -4,
    MIME_ALREADY_EXISTS = -5
};

/* MIME type structure */
struct mime_type {
    char extension[MIME_MAX_EXT_LEN];
    char type[MIME_MAX_TYPE_LEN];
};

/* Common MIME types - defined in mime.c */
extern const struct mime_type default_mime_types[];

/* Function prototypes */
int mimeInit(void);
int mimeAddType(const char *extension, const char *type);
const char *mimeGetType(const char *filename);
int mimeRemoveType(const char *extension);
void mimeCleanup(void);
enum mime_error mimeGetLastError(void);
const char *mimeGetErrorString(enum mime_error error);

/* Custom MIME type registration */
int mimeLoadFromFile(const char *path);
int mimeLoadDefaults(void);

#endif /* MIME_H */
