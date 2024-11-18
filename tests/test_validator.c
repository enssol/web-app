/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#include "config.h"
#include "unity.h"
#include "validator.h"

void setUp(void) {
    // No setup required for these tests
}

void tearDown(void) {
    // No teardown required for these tests
}

void test_validate_integer(void) {
    TEST_ASSERT_EQUAL(0, validate_integer("123"));
    TEST_ASSERT_EQUAL(-1, validate_integer("12a3"));
}

void test_validate_double(void) {
    TEST_ASSERT_EQUAL(0, validate_double("123.45"));
    TEST_ASSERT_EQUAL(-1, validate_double("123.45.67"));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_validate_integer);
    RUN_TEST(test_validate_double);
    return UNITY_END();
}