/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* filepath: test/test_scheduler.c */
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <unistd.h>
#include <pthread.h>
#include "../include/scheduler.h"
#include "../include/app_error.h"
#include "test_suite.h"

/* Test fixture */
static int test_task_count = 0;
static int test_task_executed = 0;
static volatile int stress_counter = 0;
static pthread_mutex_t stress_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Test task function */
static void *test_task(void *arg)
{
    int *count = (int *)arg;
    (*count)++;
    test_task_executed = 1;
    return NULL;
}

static void *stress_task(void *arg)
{
    pthread_mutex_lock(&stress_mutex);
    stress_counter++;
    pthread_mutex_unlock(&stress_mutex);
    return NULL;
}

/* Setup and teardown */
static int setup(void)
{
    test_task_count = 0;
    test_task_executed = 0;
    return schedulerInit();
}

static int teardown(void)
{
    schedulerStop();
    schedulerCleanup();
    return 0;
}

/* Test cases */
void test_scheduler_init(void)
{
    int status;

    /* Test double initialization */
    status = schedulerInit();
    CU_ASSERT_EQUAL(status, SCHEDULER_ALREADY_INIT);

    /* Test scheduler state */
    CU_ASSERT_EQUAL(schedulerGetState(), SCHEDULER_STATE_INIT);
}

void test_scheduler_start_stop(void)
{
    int status;

    /* Test start */
    status = schedulerStart();
    CU_ASSERT_EQUAL(status, SCHEDULER_SUCCESS);
    CU_ASSERT_EQUAL(schedulerGetState(), SCHEDULER_STATE_RUNNING);

    /* Test double start */
    status = schedulerStart();
    CU_ASSERT_EQUAL(status, SCHEDULER_ALREADY_RUNNING);

    /* Test stop */
    status = schedulerStop();
    CU_ASSERT_EQUAL(status, SCHEDULER_SUCCESS);
    CU_ASSERT_EQUAL(schedulerGetState(), SCHEDULER_STATE_STOPPED);

    /* Test double stop */
    status = schedulerStop();
    CU_ASSERT_EQUAL(status, SCHEDULER_ALREADY_STOPPED);
}

void test_scheduler_add_task(void)
{
    struct process proc;
    int status;

    /* Initialize process */
    strncpy(proc.name, "test_task", MAX_PROCESS_NAME - 1);
    proc.state = PROCESS_READY;
    proc.priority = PRIORITY_NORMAL;
    proc.task = test_task;
    proc.arg = &test_task_count;

    /* Test adding task */
    status = schedulerAddTask(&proc);
    CU_ASSERT_EQUAL(status, SCHEDULER_SUCCESS);

    /* Test adding NULL task */
    status = schedulerAddTask(NULL);
    CU_ASSERT_EQUAL(status, SCHEDULER_INVALID_TASK);
}

void test_scheduler_remove_task(void)
{
    struct process proc;
    int status;

    /* Initialize process */
    strncpy(proc.name, "test_task", MAX_PROCESS_NAME - 1);
    proc.state = PROCESS_READY;
    proc.priority = PRIORITY_NORMAL;
    proc.task = test_task;
    proc.arg = &test_task_count;

    /* Add a task */
    status = schedulerAddTask(&proc);
    CU_ASSERT_EQUAL(status, SCHEDULER_SUCCESS);

    /* Test removing task */
    status = schedulerRemoveTask(proc.pid);
    CU_ASSERT_EQUAL(status, SCHEDULER_SUCCESS);

    /* Test removing non-existent task */
    status = schedulerRemoveTask(999);
    CU_ASSERT_EQUAL(status, SCHEDULER_TASK_NOT_FOUND);
}

void test_scheduler_execution(void)
{
    struct process proc;
    int status;

    /* Initialize process */
    strncpy(proc.name, "test_task", MAX_PROCESS_NAME - 1);
    proc.state = PROCESS_READY;
    proc.priority = PRIORITY_HIGH;
    proc.task = test_task;
    proc.arg = &test_task_count;

    /* Add and start task */
    status = schedulerAddTask(&proc);
    CU_ASSERT_EQUAL(status, SCHEDULER_SUCCESS);

    status = schedulerStart();
    CU_ASSERT_EQUAL(status, SCHEDULER_SUCCESS);

    /* Wait for task execution */
    sleep(1);

    CU_ASSERT_EQUAL(test_task_executed, 1);
    CU_ASSERT(test_task_count > 0);
}

void test_scheduler_state_transitions(void)
{
    /* Test invalid state transitions */
    CU_ASSERT_EQUAL(schedulerStop(), SCHEDULER_ALREADY_STOPPED);
    CU_ASSERT_EQUAL(schedulerPause(), SCHEDULER_ERROR);

    /* Start scheduler */
    CU_ASSERT_EQUAL(schedulerStart(), SCHEDULER_SUCCESS);

    /* Test invalid transitions from RUNNING state */
    CU_ASSERT_EQUAL(schedulerStart(), SCHEDULER_ALREADY_RUNNING);

    /* Test pause/resume */
    CU_ASSERT_EQUAL(schedulerPause(), SCHEDULER_SUCCESS);
    CU_ASSERT_EQUAL(schedulerPause(), SCHEDULER_ALREADY_STOPPED);
    CU_ASSERT_EQUAL(schedulerResume(), SCHEDULER_SUCCESS);

    /* Cleanup */
    CU_ASSERT_EQUAL(schedulerStop(), SCHEDULER_SUCCESS);
}

void test_scheduler_pause_resume(void)
{
    struct process proc;
    int status;

    /* Initialize test process */
    strncpy(proc.name, "test_task", MAX_PROCESS_NAME - 1);
    proc.state = PROCESS_READY;
    proc.priority = PRIORITY_NORMAL;
    proc.task = test_task;
    proc.arg = &test_task_count;

    /* Test pause/resume with tasks */
    CU_ASSERT_EQUAL(schedulerStart(), SCHEDULER_SUCCESS);
    CU_ASSERT_EQUAL(schedulerAddTask(&proc), SCHEDULER_SUCCESS);
    CU_ASSERT_EQUAL(schedulerPause(), SCHEDULER_SUCCESS);
    CU_ASSERT_EQUAL(schedulerGetState(), SCHEDULER_STATE_PAUSED);
    CU_ASSERT_EQUAL(schedulerResume(), SCHEDULER_SUCCESS);
    CU_ASSERT_EQUAL(schedulerGetState(), SCHEDULER_STATE_RUNNING);
}

void test_scheduler_stress(void)
{
    struct process procs[MAX_TASKS];
    int i, tasks_completed;

    /* Initialize multiple tasks */
    for (i = 0; i < MAX_TASKS; i++) {
        snprintf(procs[i].name, MAX_PROCESS_NAME, "stress_task_%d", i);
        procs[i].state = PROCESS_READY;
        procs[i].priority = i % 3; /* Mix of priorities */
        procs[i].task = stress_task;
        procs[i].arg = NULL;

        CU_ASSERT_EQUAL(schedulerAddTask(&procs[i]), SCHEDULER_SUCCESS);
    }

    /* Start scheduler */
    CU_ASSERT_EQUAL(schedulerStart(), SCHEDULER_SUCCESS);

    /* Wait for tasks to complete */
    sleep(2);

    /* Verify task execution */
    pthread_mutex_lock(&stress_mutex);
    tasks_completed = stress_counter;
    pthread_mutex_unlock(&stress_mutex);

    CU_ASSERT_EQUAL(tasks_completed, MAX_TASKS);
}

void test_scheduler_process_integration(void)
{
    struct process procs[3];
    int i, status;

    /* Initialize process and scheduler systems */
    CU_ASSERT_EQUAL(processInit(), 0);
    CU_ASSERT_EQUAL(schedulerInit(), SCHEDULER_SUCCESS);

    /* Create multiple processes with different priorities */
    for (i = 0; i < 3; i++) {
        snprintf(procs[i].name, MAX_PROCESS_NAME, "proc_%d", i);
        procs[i].state = PROCESS_READY;
        procs[i].priority = i; /* Different priorities */
        procs[i].task = test_task;
        procs[i].arg = NULL;

        CU_ASSERT_EQUAL(schedulerAddTask(&procs[i]), SCHEDULER_SUCCESS);
    }

    /* Start scheduler and verify execution */
    CU_ASSERT_EQUAL(schedulerStart(), SCHEDULER_SUCCESS);
    sleep(1);

    /* Verify process states */
    for (i = 0; i < 3; i++) {
        struct process *p = processGet(procs[i].pid);
        CU_ASSERT_PTR_NOT_NULL(p);
        CU_ASSERT_EQUAL(p->state, PROCESS_COMPLETED);
    }

    /* Cleanup */
    schedulerStop();
    schedulerCleanup();
    processCleanup();
}

void test_scheduler_high_concurrency(void)
{
    struct process procs[MAX_TASKS];
    pthread_t threads[MAX_TASKS];
    int i;
    size_t active_count;

    /* Initialize scheduler with high concurrency */
    CU_ASSERT_EQUAL(schedulerInit(), SCHEDULER_SUCCESS);
    CU_ASSERT_EQUAL(schedulerStart(), SCHEDULER_SUCCESS);

    /* Create multiple concurrent tasks */
    for (i = 0; i < MAX_TASKS; i++) {
        snprintf(procs[i].name, MAX_PROCESS_NAME, "concurrent_task_%d", i);
        procs[i].state = PROCESS_READY;
        procs[i].priority = i % 3;
        procs[i].task = stress_task;
        procs[i].arg = NULL;
        CU_ASSERT_EQUAL(schedulerAddTask(&procs[i]), SCHEDULER_SUCCESS);
    }

    /* Verify task execution */
    active_count = 0;
    for (i = 0; i < MAX_TASKS; i++) {
        if (procs[i].state == PROCESS_RUNNING) {
            active_count++;
        }
    }
    CU_ASSERT(active_count > 0);
}
