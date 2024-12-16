/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#ifndef INIT_H
#define INIT_H

/* Dependencies */
#include "common.h"
#include "scheduler.h"
#include "shell.h"

/* Documentation for dependent modules:
 * - Core initialization system
 * - Used by main.c for system startup/shutdown
 * - Controls system state transitions
 */

/* Constants */
#define INIT_SUCCESS 0
#define INIT_ERROR -1
#define INIT_MAX_RETRIES 3
#define INIT_TIMEOUT 30     /* Seconds */

/* Error codes */
#define INIT_ERR_CONFIG -2
#define INIT_ERR_PERMISSION -3
#define INIT_ERR_TIMEOUT -4
#define INIT_ERR_ALREADY_RUNNING -5

/* System states */
enum system_state {
    STATE_STARTUP,     /* System is starting up */
    STATE_RUNNING,     /* System is operational */
    STATE_SHUTDOWN,    /* System is shutting down */
    STATE_ERROR,       /* System encountered error */
    STATE_REBOOT,      /* System is rebooting */
    STATE_MAINTENANCE  /* System in maintenance mode */
};

/* Function prototypes */
int initSystem(void);
int shutdownSystem(void);
int rebootSystem(void);
enum system_state getSystemState(void);
int loadConfiguration(const char *config_path);

/* Timeout management */
int initSetTimeout(int seconds);
int initGetTimeout(void);

/* System state management */
int initIsReady(void);
const char *initGetStateString(enum system_state state);

/* Shutdown handler registration */
typedef void (*shutdown_handler_t)(void);
int initRegisterShutdownHandler(shutdown_handler_t handler);

#endif /* INIT_H */
