/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* src/constants.c */
#include "../include/constants.h"
#include "../include/env.h"
#include "../include/app_error.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>  /* Added for INT_MAX and INT_MIN */

/* Static variables */
static char app_name[MAX_ENV_VALUE] = APP_NAME;
static char app_version[MAX_ENV_VALUE] = APP_VERSION;
static char app_env[MAX_ENV_VALUE] = APP_ENV;
static int app_debug = APP_DEBUG;
static int app_port = APP_PORT;
static char app_host[MAX_ENV_VALUE] = APP_HOST;

static char log_level[MAX_ENV_VALUE] = LOG_LEVEL;     /* Will be "debug" */
static char log_path[MAX_ENV_VALUE] = LOG_PATH;       /* Will be "/var/log" */
static char log_format[MAX_ENV_VALUE] = LOG_FORMAT;   /* Already "text" */
static long log_max_size = LOG_MAX_SIZE;             /* Will be 1048576 */

/* Database settings */
static char db_host[MAX_ENV_VALUE] = "localhost";
static int db_port = 5432;
static char db_name[MAX_ENV_VALUE] = "webapp";
static char db_user[MAX_ENV_VALUE] = "postgres";
static char db_password[MAX_ENV_VALUE] = "";

/* Cache settings */
static char cache_driver[MAX_ENV_VALUE] = "memory";
static char cache_prefix[MAX_ENV_VALUE] = "app";
static int cache_ttl = 3600;

/* Helper function prototypes */
static int loadStringConstant(const char *name, char *value, size_t size);
static int loadIntConstant(const char *name, int *value);
static int loadLongConstant(const char *name, long *value);
static int loadBoolConstant(const char *name, int *value);

int
constants_init(void)
{
    /* Initialize environment first */
    if (envInit(DEFAULT_ENV_PATH) != 0) {
        errorLog(ERROR_WARNING, "Failed to initialize environment, using defaults");
        return 0; /* Continue with defaults */
    }

    /* Load application settings */
    loadStringConstant("APP_NAME", app_name, sizeof(app_name));
    loadStringConstant("APP_VERSION", app_version, sizeof(app_version));
    loadStringConstant("APP_ENV", app_env, sizeof(app_env));
    loadBoolConstant("APP_DEBUG", &app_debug);
    loadIntConstant("APP_PORT", &app_port);
    loadStringConstant("APP_HOST", app_host, sizeof(app_host));

    /* Load logging settings */
    loadStringConstant("LOG_LEVEL", log_level, sizeof(log_level));
    loadStringConstant("LOG_PATH", log_path, sizeof(log_path));
    loadStringConstant("LOG_FORMAT", log_format, sizeof(log_format));
    loadLongConstant("LOG_MAX_SIZE", &log_max_size);

    /* Load database settings */
    loadStringConstant("DB_HOST", db_host, sizeof(db_host));
    loadIntConstant("DB_PORT", &db_port);
    loadStringConstant("DB_NAME", db_name, sizeof(db_name));
    loadStringConstant("DB_USER", db_user, sizeof(db_user));
    loadStringConstant("DB_PASSWORD", db_password, sizeof(db_password));

    /* Load cache settings */
    loadStringConstant("CACHE_DRIVER", cache_driver, sizeof(cache_driver));
    loadStringConstant("CACHE_PREFIX", cache_prefix, sizeof(cache_prefix));
    loadIntConstant("CACHE_TTL", &cache_ttl);

    return 0;
}

/* Getter functions implementation */
const char *get_app_name(void) { return app_name; }
const char *get_app_version(void) { return app_version; }
const char *get_app_env(void) { return app_env; }
int get_app_debug(void) { return app_debug; }
int get_app_port(void) { return app_port; }
const char *get_app_host(void) { return app_host; }
const char *get_log_level(void) { return log_level; }
const char *get_log_path(void) { return log_path; }
const char *
get_log_format(void)
{
    return DEFAULT_LOG_FORMAT;
}
long get_log_max_size(void) { return log_max_size; }

/* Add new getter functions for db and cache settings */
const char *get_db_host(void) { return db_host; }
int get_db_port(void) { return db_port; }
const char *get_db_name(void) { return db_name; }
const char *get_db_user(void) { return db_user; }
const char *get_db_password(void) { return db_password; }
const char *
get_cache_driver(void)
{
    return DEFAULT_CACHE_DRIVER;
}
const char *get_cache_prefix(void) { return cache_prefix; }
int get_cache_ttl(void) { return cache_ttl; }

/* Helper function implementations */
static int
loadStringConstant(const char *name, char *value, size_t size)
{
    char temp[MAX_ENV_VALUE];
    if (envGet(name, temp, sizeof(temp)) == 0) {
        strncpy(value, temp, size - 1);
        value[size - 1] = '\0';
        return 0;
    }
    return -1;
}

static int
loadIntConstant(const char *name, int *value)
{
    char temp[MAX_ENV_VALUE];
    if (envGet(name, temp, sizeof(temp)) == 0) {
        *value = atoi(temp);
        return 0;
    }
    return -1;
}

static int
loadLongConstant(const char *name, long *value)
{
    char temp[MAX_ENV_VALUE];
    if (envGet(name, temp, sizeof(temp)) == 0) {
        *value = atol(temp);
        return 0;
    }
    return -1;
}

static int
loadBoolConstant(const char *name, int *value)
{
    char temp[MAX_ENV_VALUE];
    if (envGet(name, temp, sizeof(temp)) == 0) {
        *value = (strcmp(temp, "1") == 0 ||
                 strcmp(temp, "true") == 0 ||
                 strcmp(temp, "yes") == 0);
        return 0;
    }
    return -1;
}

void
constants_cleanup(void)
{
    /* Reset all values to defaults */
    strncpy(app_name, APP_NAME, sizeof(app_name) - 1);
    strncpy(app_version, APP_VERSION, sizeof(app_version) - 1);
    strncpy(app_env, APP_ENV, sizeof(app_env) - 1);
    app_debug = APP_DEBUG;
    app_port = APP_PORT;
    strncpy(app_host, APP_HOST, sizeof(app_host) - 1);

    strncpy(log_level, LOG_LEVEL, sizeof(log_level) - 1);
    strncpy(log_path, LOG_PATH, sizeof(log_path) - 1);
    strncpy(log_format, LOG_FORMAT, sizeof(log_format) - 1);
    log_max_size = LOG_MAX_SIZE;
}
