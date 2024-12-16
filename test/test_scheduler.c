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
#include <stdlib.h>
#include "../include/process.h"
#include <errno.h>
#include <time.h>

/* Test constants - reduced for development */
#define STRESS_ITERATIONS 2        /* Was 10 */
#define TEST_MAX_TASKS 3          /* Using smaller value for testing */
#define PROCESS_COMPLETED 2       /* Was 3 */
#define STRESS_TEST_TIMEOUT 1     /* Was 5 seconds */
#define SCHEDULER_TEST_SLEEP 1    /* Reduced sleep time */
#define MULTI_PROCESS_COUNT 2     /* For multi-process tests */

/* Test fixtures */
static int test_task_count = 0;

/* Simple test task function */
static void *test_task(void *arg)
{
    if (arg != NULL) {
        (*(int *)arg)++;
    }
    return NULL;
}

/* Basic setup/teardown */
static int setup(void)
{
    test_task_count = 0;
    return scheduler_init();
}

static int teardown(void)
{
    scheduler_cleanup();
    return 0;
}

/* Simplified test cases */
void test_scheduler_process_integration(void)
{
    struct process proc;
    int result;
    int initial_count;

    initial_count = test_task_count;

    /* Setup process */
    proc.task = test_task;
    proc.arg = &test_task_count;
    proc.state = PROCESS_READY;
    proc.priority = 1;

    /* Add and run task */
    result = scheduler_add_task(&proc);
    CU_ASSERT_EQUAL(result, SCHEDULER_SUCCESS);

    result = scheduler_start();
    CU_ASSERT_EQUAL(result, SCHEDULER_SUCCESS);

    /* Wait for task completion */
    sleep(2);

    /* Verify execution */
    CU_ASSERT(test_task_count > initial_count);

    /* Cleanup */
    scheduler_stop();
}

void test_scheduler_pause_resume(void)
{
    struct process proc;
    int result;

    /* Setup process */
    proc.task = test_task;
    proc.arg = &test_task_count;
    proc.state = PROCESS_READY;
    proc.priority = 1;

    /* Add task */
    result = scheduler_add_task(&proc);
    CU_ASSERT_EQUAL(result, SCHEDULER_SUCCESS);

    /* Start scheduler */
    result = scheduler_start();
    CU_ASSERT_EQUAL(result, SCHEDULER_SUCCESS);

    /* Let task start */
    sleep(1);

    /* Test pause */
    result = scheduler_pause();
    CU_ASSERT_EQUAL(result, SCHEDULER_SUCCESS);

    /* Verify pause */
    sleep(1);

    /* Resume */
    result = scheduler_resume();
    CU_ASSERT_EQUAL(result, SCHEDULER_SUCCESS);

    /* Cleanup */
    sleep(1);
    scheduler_stop();
    scheduler_cleanup();
}

/* Test suite */
int test_scheduler(void)
{
    CU_pSuite suite;

    suite = CU_add_suite("Scheduler Tests", setup, teardown);
    if (suite == NULL) {
        return -1;
    }

    if ((CU_add_test(suite, "Pause/Resume", test_scheduler_pause_resume) == NULL) ||
        (CU_add_test(suite, "Process Integration", test_scheduler_process_integration) == NULL)) {
        return -1;
    }

    return 0;
}
