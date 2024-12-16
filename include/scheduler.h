/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#ifndef SCHEDULER_H
#define SCHEDULER_H

/* Dependencies */
#include <unistd.h> /* For usleep() */
#include "common.h"
#include "process.h"
#include "app_error.h"

/* Documentation for dependent modules:
 * - Core scheduling facility
 * - Used by init.h for system task management
 * - Manages process lifecycle and scheduling
 */

/* Constants */
#define SCHEDULER_SUCCESS 0
#define SCHEDULER_ERROR -1
#define MAX_TASKS 128

/* Error codes */
#define SCHEDULER_ERROR_ALREADY_INITIALIZED -2
#define SCHEDULER_ERROR_NOT_INITIALIZED -3
#define SCHEDULER_ERROR_THREAD_CREATE -4
#define SCHEDULER_ERROR_INVALID_ARGS -5
#define SCHEDULER_ERROR_QUEUE_FULL -6

/* Scheduler states */
enum scheduler_state {
    SCHEDULER_STATE_INIT,
    SCHEDULER_STATE_STOPPED,
    SCHEDULER_STATE_READY,
    SCHEDULER_STATE_RUNNING,
    SCHEDULER_STATE_ERROR,
    SCHEDULER_STATE_PAUSED
};

/* Function prototypes */
int scheduler_init(void);
int scheduler_add_task(struct process *task);
int scheduler_shutdown(void);
int scheduler_cleanup(void);
int scheduler_start(void);
int scheduler_stop(void);
int scheduler_pause(void);
int scheduler_resume(void);
enum scheduler_state scheduler_get_state(void);

#endif /* SCHEDULER_H */
