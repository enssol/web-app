/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#include "config.h"
#include "unity.h"
#include "config_loader.h"
#include <stdio.h>

void setUp(void) {
    // Create temporary configuration files
    FILE *file = fopen("config/config.ini", "w");
    fprintf(file, "key=value\n");
    fclose(file);

    file = fopen("config/config.conf", "w");
    fprintf(file, "key=value\n");
    fclose(file);
}

void tearDown(void) {
    // Delete temporary configuration files
    remove("config/config.ini");
    remove("config/config.conf");
}

void test_load_ini_config(void) {
    int result = load_ini_config("config/config.ini");
    TEST_ASSERT_EQUAL(0, result);
}

void test_load_conf_config(void) {
    int result = load_conf_config("config/config.conf");
    TEST_ASSERT_EQUAL(0, result);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_load_ini_config);
    RUN_TEST(test_load_conf_config);
    return UNITY_END();
}