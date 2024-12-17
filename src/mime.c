/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */
/* src/mime.c */

#include "../include/mime.h"
#include "../include/app_error.h"
#include "../include/fs.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

/* Static variables */
static int is_initialized = 0;
static enum mime_error last_error = MIME_SUCCESS;
static struct mime_type mime_types[MIME_MAX_TYPES];
static size_t mime_type_count = 0;

/* Common MIME types table */
const struct mime_type default_mime_types[] = {
    {".html", "text/html"},
    {".htm", "text/html"},
    {".css", "text/css"},
    {".js", "application/javascript"},
    {".json", "application/json"},
    {".txt", "text/plain"},
    {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".png", "image/png"},
    {".gif", "image/gif"},
    {".svg", "image/svg+xml"},
    {".xml", "application/xml"},
    {".pdf", "application/pdf"},
    {".zip", "application/zip"},
    {".ico", "image/x-icon"},
    {".woff", "font/woff"},
    {".woff2", "font/woff2"},
    {".ttf", "font/ttf"},
    {".eot", "application/vnd.ms-fontobject"},
    {"", ""} /* Sentinel */
};

/* Internal function prototypes */
static const char *get_file_extension(const char *filename);
static int load_mime_type(const char *extension, const char *type);
static int validate_extension(const char *extension);
static int validate_type(const char *type);

int
mimeInit(void)
{
    if (is_initialized) {
        return 0;
    }

    mime_type_count = 0;
    is_initialized = 1;
    last_error = MIME_SUCCESS;
    return mimeLoadDefaults();
}

int
mimeAddType(const char *extension, const char *type)
{
    size_t i;

    if (!is_initialized) {
        last_error = MIME_ERROR;
        return -1;
    }

    if (extension == NULL || type == NULL) {
        last_error = MIME_INVALID_PARAM;
        return -1;
    }

    if (!validate_extension(extension) || !validate_type(type)) {
        last_error = MIME_INVALID_PARAM;
        return -1;
    }

    if (mime_type_count >= MIME_MAX_TYPES) {
        last_error = MIME_TOO_MANY_TYPES;
        return -1;
    }

    /* Check if extension already exists */
    for (i = 0; i < mime_type_count; i++) {
        if (strcmp(mime_types[i].extension, extension) == 0) {
            last_error = MIME_ALREADY_EXISTS;
            return -1;
        }
    }

    return load_mime_type(extension, type);
}

const char *
mimeGetType(const char *filename)
{
    size_t i;
    const char *extension;

    if (!is_initialized || filename == NULL) {
        last_error = MIME_INVALID_PARAM;
        return MIME_DEFAULT_TYPE;
    }

    extension = get_file_extension(filename);
    if (extension == NULL) {
        return MIME_DEFAULT_TYPE;
    }

    for (i = 0; i < mime_type_count; i++) {
        if (strcmp(mime_types[i].extension, extension) == 0) {
            return mime_types[i].type;
        }
    }

    last_error = MIME_NOT_FOUND;
    return MIME_DEFAULT_TYPE;
}

int
mimeRemoveType(const char *extension)
{
    size_t i;
    size_t j;

    if (!is_initialized || extension == NULL) {
        last_error = MIME_INVALID_PARAM;
        return -1;
    }

    for (i = 0; i < mime_type_count; i++) {
        if (strcmp(mime_types[i].extension, extension) == 0) {
            /* Shift remaining entries */
            for (j = i; j < mime_type_count - 1; j++) {
                strncpy(mime_types[j].extension, mime_types[j + 1].extension,
                        MIME_MAX_EXT_LEN - 1);
                mime_types[j].extension[MIME_MAX_EXT_LEN - 1] = '\0';

                strncpy(mime_types[j].type, mime_types[j + 1].type,
                        MIME_MAX_TYPE_LEN - 1);
                mime_types[j].type[MIME_MAX_TYPE_LEN - 1] = '\0';
            }
            mime_type_count--;
            return 0;
        }
    }

    last_error = MIME_NOT_FOUND;
    return -1;
}

void
mimeCleanup(void)
{
    mime_type_count = 0;
    is_initialized = 0;
    last_error = MIME_SUCCESS;
}

enum mime_error
mimeGetLastError(void)
{
    return last_error;
}

const char *
mimeGetErrorString(enum mime_error error)
{
    switch (error) {
        case MIME_SUCCESS:
            return "Success";
        case MIME_ERROR:
            return "General error";
        case MIME_NOT_FOUND:
            return "MIME type not found";
        case MIME_INVALID_PARAM:
            return "Invalid parameter";
        case MIME_TOO_MANY_TYPES:
            return "Too many MIME types";
        case MIME_ALREADY_EXISTS:
            return "MIME type already exists";
        default:
            return "Unknown error";
    }
}

int
mimeLoadFromFile(const char *path)
{
    char line[MIME_MAX_TYPE_LEN + MIME_MAX_EXT_LEN + 2];
    char extension[MIME_MAX_EXT_LEN];
    char type[MIME_MAX_TYPE_LEN];
    FILE *fp;
    char *token;
    char *saveptr;

    if (!is_initialized || path == NULL) {
        last_error = MIME_INVALID_PARAM;
        return -1;
    }

    fp = fopen(path, "r");
    if (fp == NULL) {
        last_error = MIME_ERROR;
        return -1;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        /* Skip comments and empty lines */
        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }

        /* Parse extension */
        token = strtok_r(line, " \t", &saveptr);
        if (token == NULL) {
            continue;
        }
        strncpy(extension, token, MIME_MAX_EXT_LEN - 1);
        extension[MIME_MAX_EXT_LEN - 1] = '\0';

        /* Parse type */
        token = strtok_r(NULL, " \t\n", &saveptr);
        if (token == NULL) {
            continue;
        }
        strncpy(type, token, MIME_MAX_TYPE_LEN - 1);
        type[MIME_MAX_TYPE_LEN - 1] = '\0';

        if (mimeAddType(extension, type) != 0) {
            fclose(fp);
            return -1;
        }
    }

    fclose(fp);
    return 0;
}

int
mimeLoadDefaults(void)
{
    size_t i;

    for (i = 0; default_mime_types[i].extension[0] != '\0'; i++) {
        if (load_mime_type(default_mime_types[i].extension,
                          default_mime_types[i].type) != 0) {
            return -1;
        }
    }

    return 0;
}

/* Internal function implementations */
static const char *
get_file_extension(const char *filename)
{
    const char *dot;

    if (filename == NULL) {
        return NULL;
    }

    dot = strrchr(filename, '.');
    if (dot == NULL || dot == filename) {
        return NULL;
    }

    return dot;
}

static int
load_mime_type(const char *extension, const char *type)
{
    if (mime_type_count >= MIME_MAX_TYPES) {
        last_error = MIME_TOO_MANY_TYPES;
        return -1;
    }

    strncpy(mime_types[mime_type_count].extension, extension,
            MIME_MAX_EXT_LEN - 1);
    mime_types[mime_type_count].extension[MIME_MAX_EXT_LEN - 1] = '\0';

    strncpy(mime_types[mime_type_count].type, type, MIME_MAX_TYPE_LEN - 1);
    mime_types[mime_type_count].type[MIME_MAX_TYPE_LEN - 1] = '\0';

    mime_type_count++;
    return 0;
}

static int
validate_extension(const char *extension)
{
    size_t len;

    if (extension == NULL) {
        return 0;
    }

    len = strlen(extension);
    return (len > 0 && len < MIME_MAX_EXT_LEN && extension[0] == '.');
}

static int
validate_type(const char *type)
{
    size_t len;

    if (type == NULL) {
        return 0;
    }

    len = strlen(type);
    return (len > 0 && len < MIME_MAX_TYPE_LEN);
}
