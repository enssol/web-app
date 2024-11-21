/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#include "config.h"
#include "gettext.h"
#include "unity.h"
#include "main.c" // Include the main source file to test its functions

void setUp(void) {
    // Setup code if needed
}

void tearDown(void) {
    // Cleanup code if needed
}

void test_initialize(void) {
    // Test the initialize function
    initialize();
    // Add assertions to verify the expected state after initialization
    TEST_ASSERT_EQUAL_STRING("INFO", config.log_level);
    TEST_ASSERT_EQUAL(3, config.max_retries);
}

void test_cleanup(void) {
    // Test the cleanup function
    cleanup();
    // Add assertions to verify the expected state after cleanup
    // (e.g., check if resources are released)
}

void test_validate_integer(void) {
    // Test the validate_integer function
    TEST_ASSERT_EQUAL(0, validate_integer("123"));
    TEST_ASSERT_EQUAL(-1, validate_integer("12a3"));
}

void test_validate_double(void) {
    // Test the validate_double function
    TEST_ASSERT_EQUAL(0, validate_double("123.45"));
    TEST_ASSERT_EQUAL(-1, validate_double("123.45.67"));
}

void test_validate_string(void) {
    // Test the validate_string function
    TEST_ASSERT_EQUAL(0, validate_string("valid_string"));
    TEST_ASSERT_EQUAL(-1, validate_string(""));
}

void test_validate_boolean(void) {
    // Test the validate_boolean function
    TEST_ASSERT_EQUAL(0, validate_boolean("true"));
    TEST_ASSERT_EQUAL(0, validate_boolean("false"));
    TEST_ASSERT_EQUAL(-1, validate_boolean("invalid"));
}

void test_validate_config(void) {
    // Test the validate_config function
    TEST_ASSERT_EQUAL(0, validate_config("app_name", "MyApp"));
    TEST_ASSERT_EQUAL(0, validate_config("version", "2.0"));
    TEST_ASSERT_EQUAL(0, validate_config("log_level", "DEBUG"));
    TEST_ASSERT_EQUAL(-1, validate_config("log_level", "INVALID"));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_cleanup);
    RUN_TEST(test_validate_integer);
    RUN_TEST(test_validate_double);
    RUN_TEST(test_validate_string);
    RUN_TEST(test_validate_boolean);
    RUN_TEST(test_validate_config);
    return UNITY_END();
}
