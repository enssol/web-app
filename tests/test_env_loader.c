/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#include "config.h"
#include "gettext.h"
#include "unity.h"
#include "env_loader.h"
#include <stdlib.h>

void setUp(void) {
    // Set up environment variables
    setenv("TEST_ENV_VAR", "test_value", 1);
}

void tearDown(void) {
    // Clear environment variables
    unsetenv("TEST_ENV_VAR");
}

void test_load_env_config(void) {
    int result = load_env_config("config/.env");
    TEST_ASSERT_EQUAL(0, result);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_load_env_config);
    return UNITY_END();
}
