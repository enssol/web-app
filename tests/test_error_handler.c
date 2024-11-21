/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#include "config.h"
#include "gettext.h"
#include "unity.h"
#include "error_handler.h"
#include <stdio.h>
#include <stdlib.h>

static char error_message[256];

void custom_error_handler(const char *message) {
    strncpy(error_message, message, sizeof(error_message) - 1);
}

void setUp(void) {
    // Set up custom error handler
    set_error_handler(custom_error_handler);
}

void tearDown(void) {
    // Restore default error handler
    set_error_handler(NULL);
}

void test_handle_error(void) {
    handle_error("This is an error message");
    TEST_ASSERT_EQUAL_STRING("This is an error message", error_message);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_handle_error);
    return UNITY_END();
}
