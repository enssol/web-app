/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#include "config.h"
#include "unity.h"
#include "hello.h"
#include <stdio.h>
#include <stdlib.h>

static FILE *output;
static char buffer[256];

void setUp(void) {
    // Redirect stdout to a buffer
    output = freopen("/dev/null", "w", stdout);
    setvbuf(stdout, buffer, _IOFBF, sizeof(buffer));
}

void tearDown(void) {
    // Restore stdout
    freopen("/dev/tty", "w", stdout);
}

void test_print_hello(void) {
    print_hello();
    fflush(stdout);
    TEST_ASSERT_EQUAL_STRING("Hello, World!\n", buffer);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_print_hello);
    return UNITY_END();
}