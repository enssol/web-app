/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#include "config.h"
#include "gettext.h"
#include "unity.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>

static FILE *log_file;
static char log_buffer[256];

void setUp(void) {
    // Redirect log output to a buffer
    log_file = freopen("/dev/null", "w", stdout);
    setvbuf(stdout, log_buffer, _IOFBF, sizeof(log_buffer));
    init_logger();
}

void tearDown(void) {
    // Restore stdout
    freopen("/dev/tty", "w", stdout);
}

void test_log_info(void) {
    log_info("This is an info message");
    fflush(stdout);
    TEST_ASSERT_TRUE(strstr(log_buffer, "This is an info message") != NULL);
}

void test_log_error(void) {
    log_error("This is an error message");
    fflush(stdout);
    TEST_ASSERT_TRUE(strstr(log_buffer, "This is an error message") != NULL);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_log_info);
    RUN_TEST(test_log_error);
    return UNITY_END();
}
