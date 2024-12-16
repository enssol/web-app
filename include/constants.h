/* filepath: /devcontainer/web-app/include/constants.h */
/* Copyright 2024 Enveng Group */
#ifndef CONSTANTS_H
#define CONSTANTS_H

/* Dependencies */
#include "common.h"

/* System paths */
#define DEFAULT_CONFIG_PATH "/etc/config"
#define DEFAULT_LOG_PATH "/var/log/app.log"  /* Updated to be more specific */
#define DEFAULT_ENV_PATH "/etc/config/env"
#define DEFAULT_LOG_DIR "/var/log"

/* Configuration limits */
#define MAX_ENV_NAME 64
#define MAX_ENV_VALUE 1024
#define MAX_ENV_KEY 256
#define MAX_LINE_LENGTH (MAX_ENV_KEY + MAX_ENV_VALUE + 2)
#define MAX_CONFIG_LINE 2048
#define MAX_LOG_SIZE (10 * 1024 * 1024) /* 10MB */
#define MAX_CMD_LEN 1024      /* From shell.h */
#define MAX_PROCESSES 64      /* From process.h */
#define CACHE_MAX_ENTRIES 1024 /* From cache.h */

/* Exit codes */
#define EXIT_CONFIG_ERROR 78 /* EX_CONFIG from sysexits.h */
#define EXIT_ENV_ERROR 71    /* EX_OSERR from sysexits.h */

/* App constants */
#define APP_NAME "TestApp"
#define APP_VERSION "1.0.0"
#define APP_ENV "test"
#define APP_DEBUG 1
#define APP_PORT 8080
#define APP_HOST "localhost"

/* Constants missing from current implementation */
#define LOG_LEVEL "info"         /* Default log level */
#define LOG_PATH "/var/log/app.log"  /* Default log path */
#define LOG_FORMAT "text"        /* Default log format */

/* Log configuration */
#define DEFAULT_LOG_MODE 0644
#define LOG_MAX_FILES 10
#define LOG_MAX_SIZE (10 * 1024 * 1024) /* 10MB */
#define LOG_MAX_AGE (24 * 60 * 60)      /* 24 hours */

/* Function prototypes */
int constants_init(void);
void constants_cleanup(void);

#endif /* CONSTANTS_H */
