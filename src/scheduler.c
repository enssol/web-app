/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* src/scheduler.c */
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/scheduler.h"

/* Static variables */
static struct process *task_queue[MAX_TASKS];
static size_t task_count = 0;
static int is_initialized = 0;
static int is_running = 0;
static pthread_mutex_t scheduler_mutex = PTHREAD_MUTEX_INITIALIZER;
static enum scheduler_state current_state = SCHEDULER_STATE_STOPPED;
static pthread_t worker_thread;

/* Forward declarations */
static int scheduler_process_tasks(void);
static void *scheduler_worker(void *arg);

/* Initialize the scheduler */
int
scheduler_init(void)
{
    int result;

    pthread_mutex_lock(&scheduler_mutex);

    if (is_initialized) {
        pthread_mutex_unlock(&scheduler_mutex);
        return SCHEDULER_ERROR_ALREADY_INITIALIZED;
    }

    /* Initialize task queue */
    memset(task_queue, 0, sizeof(task_queue));
    task_count = 0;
    current_state = SCHEDULER_STATE_READY;
    is_initialized = 1;

    /* Start worker thread */
    result = pthread_create(&worker_thread, NULL, scheduler_worker, NULL);
    if (result != 0) {
        is_initialized = 0;
        pthread_mutex_unlock(&scheduler_mutex);
        return SCHEDULER_ERROR_THREAD_CREATE;
    }

    is_running = 1;
    pthread_mutex_unlock(&scheduler_mutex);
    return SCHEDULER_SUCCESS;
}

/* Add a process to the scheduler */
int
scheduler_add_task(struct process *task)
{
    if (!is_initialized || !is_running) {
        return SCHEDULER_ERROR_NOT_INITIALIZED;
    }

    if (task == NULL) {
        return SCHEDULER_ERROR_INVALID_ARGS;
    }

    pthread_mutex_lock(&scheduler_mutex);

    if (task_count >= MAX_TASKS) {
        pthread_mutex_unlock(&scheduler_mutex);
        return SCHEDULER_ERROR_QUEUE_FULL;
    }

    task_queue[task_count++] = task;

    pthread_mutex_unlock(&scheduler_mutex);
    return SCHEDULER_SUCCESS;
}

/* Worker thread implementation */
static void *
scheduler_worker(void *arg)
{
    int result;

    (void)arg; /* Unused parameter */

    while (is_running) {
        pthread_mutex_lock(&scheduler_mutex);

        if (current_state == SCHEDULER_STATE_READY && task_count > 0) {
            current_state = SCHEDULER_STATE_RUNNING;
            result = scheduler_process_tasks();
            if (result != SCHEDULER_SUCCESS) {
                current_state = SCHEDULER_STATE_ERROR;
            }
            current_state = SCHEDULER_STATE_READY;
        }

        pthread_mutex_unlock(&scheduler_mutex);

        /* Sleep briefly to prevent busy-waiting */
        usleep(10000); /* 10ms */
    }

    return NULL;
}

/* Process tasks in the queue */
static int
scheduler_process_tasks(void)
{
    size_t i;
    struct process *current_task;

    for (i = 0; i < task_count; i++) {
        current_task = task_queue[i];
        if (current_task != NULL && current_task->task != NULL) {
            current_task->task(current_task->arg);
            /* Clear completed task */
            task_queue[i] = NULL;
        }
    }

    /* Compact queue */
    task_count = 0;
    return SCHEDULER_SUCCESS;
}

/* Shutdown the scheduler */
int
scheduler_shutdown(void)
{
    if (!is_initialized) {
        return SCHEDULER_ERROR_NOT_INITIALIZED;
    }

    pthread_mutex_lock(&scheduler_mutex);
    is_running = 0;
    pthread_mutex_unlock(&scheduler_mutex);

    /* Wait for worker thread to finish */
    pthread_join(worker_thread, NULL);

    /* Cleanup */
    memset(task_queue, 0, sizeof(task_queue));
    task_count = 0;
    is_initialized = 0;
    current_state = SCHEDULER_STATE_STOPPED;

    return SCHEDULER_SUCCESS;
}

int
scheduler_cleanup(void)
{
    int result;

    result = scheduler_shutdown();
    if (result != SCHEDULER_SUCCESS) {
        return result;
    }

    return SCHEDULER_SUCCESS;
}

int
scheduler_start(void)
{
    if (!is_initialized) {
        return SCHEDULER_ERROR_NOT_INITIALIZED;
    }

    pthread_mutex_lock(&scheduler_mutex);
    current_state = SCHEDULER_STATE_READY;
    pthread_mutex_unlock(&scheduler_mutex);

    return SCHEDULER_SUCCESS;
}

int
scheduler_stop(void)
{
    if (!is_initialized) {
        return SCHEDULER_ERROR_NOT_INITIALIZED;
    }

    pthread_mutex_lock(&scheduler_mutex);
    current_state = SCHEDULER_STATE_STOPPED;
    pthread_mutex_unlock(&scheduler_mutex);

    return SCHEDULER_SUCCESS;
}

int
scheduler_pause(void)
{
    if (!is_initialized) {
        return SCHEDULER_ERROR_NOT_INITIALIZED;
    }

    pthread_mutex_lock(&scheduler_mutex);
    current_state = SCHEDULER_STATE_PAUSED;
    pthread_mutex_unlock(&scheduler_mutex);

    return SCHEDULER_SUCCESS;
}

int
scheduler_resume(void)
{
    if (!is_initialized) {
        return SCHEDULER_ERROR_NOT_INITIALIZED;
    }

    pthread_mutex_lock(&scheduler_mutex);
    current_state = SCHEDULER_STATE_READY;
    pthread_mutex_unlock(&scheduler_mutex);

    return SCHEDULER_SUCCESS;
}
