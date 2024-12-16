/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* src/init.c */
#include "../include/init.h"
#include "../include/app_error.h"
#include "../include/constants.h"
#include "../include/env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

/* Static variables */
static enum system_state current_state = STATE_STARTUP;

/* Production directories */
static const char *system_dirs[] = {
    "/etc/enssol/config",
    "/var/log/enssol",
    "/var/run/enssol",
    NULL
};

/* Required configuration files */
static const char *required_files[] = {
    "etc/config/system.conf",
    "etc/config/env",
    "var/log/app.log",
    NULL
};

static int createSystemFiles(void)
{
    const char **file;
    FILE *fp;
    char error_msg[MAX_ERROR_MSG];

    for (file = required_files; *file != NULL; file++) {
        fp = fopen(*file, "a");
        if (fp == NULL) {
            snprintf(error_msg, sizeof(error_msg),
                    "Failed to create file: %s", *file);
            errorLog(ERROR_WARNING, error_msg);
            return INIT_ERROR;
        }
        fclose(fp);
    }

    return INIT_SUCCESS;
}

int
initSystem(void)
{
    const char **dir;
    mode_t old_umask;

    old_umask = umask(022);

    for (dir = system_dirs; *dir != NULL; dir++) {
        if (mkdir(*dir, 0755) != 0 && errno != EEXIST) {
            errorLog(ERROR_WARNING, "Failed to create system directory");
            umask(old_umask);
            return INIT_ERROR;
        }
    }

    /* Create required files */
    if (createSystemFiles() != INIT_SUCCESS) {
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
    int status;

    if (config_path == NULL) {
        errno = EINVAL;
        return INIT_ERROR;
    }

    status = constants_init();
    if (status != SUCCESS) {
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
