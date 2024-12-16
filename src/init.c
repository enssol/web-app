/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* src/init.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include "../include/init.h"
#include "../include/app_error.h"
#include "../include/constants.h"
#include "../include/env.h"

/* Static variables */
static enum system_state current_state = STATE_STARTUP;
static char base_path[FILENAME_MAX];

/* Static function prototypes */
static int create_system_files(void);
static int create_system_directories(void);
static int ensure_directory(const char *path);
static int initialize_base_path(void);

/* Production directories */
static const char *system_dirs[] = {
    "/etc/config",
    "/var/log",
    "/var/run",
    NULL
};

/* Required configuration files */
static const char *required_files[] = {
    "etc/config/system.conf",
    "etc/config/env",
    "var/log/app.log",
    NULL
};

static int
initialize_base_path(void)
{
    const char *env_path;

    env_path = getenv("APP_BASE_PATH");
    if (env_path != NULL) {
        if (strlen(env_path) >= FILENAME_MAX) {
            errno = ENAMETOOLONG;
            return INIT_ERROR;
        }
        strncpy(base_path, env_path, FILENAME_MAX - 1);
        base_path[FILENAME_MAX - 1] = '\0';
    } else {
        /* Default to test path for test environment */
        if (getenv("APP_ENV") != NULL && strcmp(getenv("APP_ENV"), "test") == 0) {
            strncpy(base_path, "test_tmp", FILENAME_MAX - 1);
        } else {
            strncpy(base_path, "", FILENAME_MAX - 1);
        }
        base_path[FILENAME_MAX - 1] = '\0';
    }
    return INIT_SUCCESS;
}

static int
ensure_directory(const char *path)
{
    char full_path[FILENAME_MAX];
    char *p;
    mode_t mode;
    size_t len;

    len = strlen(base_path) + strlen(path) + 1; /* +1 for null terminator */
    if (len >= FILENAME_MAX) {
        errno = ENAMETOOLONG;
        return INIT_ERROR;
    }

    strcpy(full_path, base_path);
    strcat(full_path, path);

    mode = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH; /* 0755 */

    for (p = full_path + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            if (mkdir(full_path, mode) != 0 && errno != EEXIST) {
                return INIT_ERROR;
            }
            *p = '/';
        }
    }
    if (mkdir(full_path, mode) != 0 && errno != EEXIST) {
        return INIT_ERROR;
    }

    return INIT_SUCCESS;
}

static int
create_system_directories(void)
{
    const char **dir;
    int status;

    for (dir = system_dirs; *dir != NULL; dir++) {
        status = ensure_directory(*dir);
        if (status != INIT_SUCCESS) {
            errorLog(ERROR_WARNING, "Failed to create system directory");
            return INIT_ERROR;
        }
    }

    return INIT_SUCCESS;
}

static int
create_system_files(void)
{
    const char **file;
    char full_path[FILENAME_MAX];
    FILE *fp;
    size_t len;

    for (file = required_files; *file != NULL; file++) {
        len = strlen(base_path) + strlen(*file) + 2; /* +2 for '/' and null terminator */
        if (len >= FILENAME_MAX) {
            errno = ENAMETOOLONG;
            return INIT_ERROR;
        }

        strcpy(full_path, base_path);
        strcat(full_path, "/");
        strcat(full_path, *file);

        fp = fopen(full_path, "a");
        if (fp == NULL) {
            errorLog(ERROR_WARNING, "Failed to create file");
            return INIT_ERROR;
        }
        fclose(fp);
    }

    return INIT_SUCCESS;
}

int
initSystem(void)
{
    mode_t old_umask;
    int status;

    old_umask = umask(022);

    status = initialize_base_path();
    if (status != INIT_SUCCESS) {
        umask(old_umask);
        return INIT_ERROR;
    }

    status = create_system_directories();
    if (status != INIT_SUCCESS) {
        umask(old_umask);
        return INIT_ERROR;
    }

    status = create_system_files();
    if (status != INIT_SUCCESS) {
        umask(old_umask);
        return INIT_ERROR;
    }

    umask(old_umask);
    current_state = STATE_RUNNING;
    return INIT_SUCCESS;
}

int
shutdownSystem(void)
{
    current_state = STATE_SHUTDOWN;
    return INIT_SUCCESS;
}

int
rebootSystem(void)
{
    int status;

    status = shutdownSystem();
    if (status != INIT_SUCCESS) {
        return INIT_ERROR;
    }

    current_state = STATE_REBOOT;
    return INIT_SUCCESS;
}

int
loadConfiguration(const char *config_path)
{
    struct stat st;

    /* Validate input */
    if (config_path == NULL) {
        errno = EINVAL;
        return INIT_ERROR;
    }

    /* Check if file exists and is readable */
    if (stat(config_path, &st) != 0 || !S_ISREG(st.st_mode)) {
        errno = ENOENT;
        return INIT_ERROR;
    }

    /* Check read permissions */
    if (access(config_path, R_OK) != 0) {
        errno = EACCES;
        return INIT_ERROR;
    }

    /* Initialize constants */
    if (constants_init() != 0) {
        errorLog(ERROR_CRITICAL, "Failed to initialize constants");
        return INIT_ERROR;
    }

    return INIT_SUCCESS;
}

enum system_state
getSystemState(void)
{
    return current_state;
}
