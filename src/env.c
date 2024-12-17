/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* src/env.c */
#include "../include/env.h"
#include "../include/constants.h"
#include "../include/app_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* Static variables */
static char env_vars[MAX_ENV_VARS][MAX_ENV_VALUE];
static char env_names[MAX_ENV_VARS][MAX_ENV_NAME];
static size_t env_count = 0;
static enum env_status last_status = ENV_SUCCESS;

/* Internal helper functions */
static int parseEnvLine(const char *line, char *name, char *value);
static int findEnvVar(const char *name);

int
envInit(const char *env_path)
{
    FILE *env_file;
    char line[MAX_CONFIG_LINE];
    char name[MAX_ENV_NAME];
    char value[MAX_ENV_VALUE];
    mode_t old_umask;

    /* Reset environment storage */
    memset(env_vars, 0, sizeof(env_vars));
    memset(env_names, 0, sizeof(env_names));
    env_count = 0;

    if (env_path == NULL) {
        last_status = ENV_INVALID_PARAM;
        return -1;
    }

    /* Set restrictive permissions when reading env file */
    old_umask = umask(077);

    env_file = fopen(env_path, "r");
    if (env_file == NULL) {
        umask(old_umask);
        last_status = ENV_ERROR;
        return -1;
    }

    while (fgets(line, sizeof(line), env_file) != NULL && env_count < MAX_ENV_VARS) {
        /* Skip empty lines and comments */
        if (line[0] == '\n' || line[0] == '#') {
            continue;
        }

        /* Parse and store environment variable */
        if (parseEnvLine(line, name, value) == 0) {
            strncpy(env_names[env_count], name, MAX_ENV_NAME - 1);
            env_names[env_count][MAX_ENV_NAME - 1] = '\0';
            strncpy(env_vars[env_count], value, MAX_ENV_VALUE - 1);
            env_vars[env_count][MAX_ENV_VALUE - 1] = '\0';
            env_count++;
        }
    }

    fclose(env_file);
    umask(old_umask);
    last_status = ENV_SUCCESS;
    return 0;
}

int
envGet(const char *name, char *value, size_t size)
{
    int index;

    if (name == NULL || value == NULL || size == 0) {
        last_status = ENV_INVALID_PARAM;
        return -1;
    }

    index = findEnvVar(name);
    if (index == -1) {
        last_status = ENV_NOT_FOUND;
        return -1;
    }

    if (strlen(env_vars[index]) >= size) {
        last_status = ENV_ERROR;
        return -1;
    }

    strncpy(value, env_vars[index], size - 1);
    value[size - 1] = '\0';

    last_status = ENV_SUCCESS;
    return 0;
}

int
envSet(const char *name, const char *value)
{
    int index;

    if (name == NULL || value == NULL) {
        last_status = ENV_INVALID_PARAM;
        return -1;
    }

    if (strlen(name) >= MAX_ENV_NAME || strlen(value) >= MAX_ENV_VALUE) {
        last_status = ENV_INVALID_PARAM;
        return -1;
    }

    index = findEnvVar(name);
    if (index != -1) {
        /* Update existing variable */
        strncpy(env_vars[index], value, MAX_ENV_VALUE - 1);
        env_vars[index][MAX_ENV_VALUE - 1] = '\0';
    } else if (env_count < MAX_ENV_VARS) {
        /* Add new variable */
        strncpy(env_names[env_count], name, MAX_ENV_NAME - 1);
        env_names[env_count][MAX_ENV_NAME - 1] = '\0';
        strncpy(env_vars[env_count], value, MAX_ENV_VALUE - 1);
        env_vars[env_count][MAX_ENV_VALUE - 1] = '\0';
        env_count++;
    } else {
        last_status = ENV_ERROR;
        return -1;
    }

    last_status = ENV_SUCCESS;
    return 0;
}

void
envCleanup(void)
{
    memset(env_vars, 0, sizeof(env_vars));
    memset(env_names, 0, sizeof(env_names));
    env_count = 0;
    last_status = ENV_SUCCESS;
}

enum env_status
envGetStatus(void)
{
    return last_status;
}

static int
parseEnvLine(const char *line, char *name, char *value)
{
    const char *eq_pos;
    size_t name_len;

    if (line == NULL || name == NULL || value == NULL) {
        return -1;
    }

    /* Find equals sign */
    eq_pos = strchr(line, '=');
    if (eq_pos == NULL) {
        return -1;
    }

    /* Calculate name length safely */
    if (eq_pos < line) {
        return -1;
    }

    name_len = (size_t)(eq_pos - line); /* Safe cast since we verified eq_pos > line */
    if (name_len >= MAX_ENV_NAME) {
        return -1;
    }

    /* Copy name and add null terminator */
    memcpy(name, line, name_len);
    name[name_len] = '\0';

    /* Skip equals sign and copy value */
    eq_pos++; /* Move past '=' */
    strncpy(value, eq_pos, MAX_ENV_VALUE - 1);
    value[MAX_ENV_VALUE - 1] = '\0';

    /* Remove trailing newline if present */
    name_len = strlen(value);
    if (name_len > 0 && value[name_len - 1] == '\n') {
        value[name_len - 1] = '\0';
    }

    return 0;
}

static int
findEnvVar(const char *name)
{
    size_t i;

    for (i = 0; i < env_count; i++) {
        if (strcmp(env_names[i], name) == 0) {
            return (int)i;
        }
    }

    return -1;
}
