/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#include "config.h"
#include "unity.h"
#include "garbage_collector.h"

void setUp(void) {
    // Initialize the garbage collector
    init_garbage_collector();
}

void tearDown(void) {
    // Clean up the garbage collector
    cleanup_garbage_collector();
}

void test_init_garbage_collector(void) {
    // Test initialization logic
    TEST_ASSERT_TRUE(is_garbage_collector_initialized());
}

void test_cleanup_garbage_collector(void) {
    // Test cleanup logic
    cleanup_garbage_collector();
    TEST_ASSERT_FALSE(is_garbage_collector_initialized());
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_init_garbage_collector);
    RUN_TEST(test_cleanup_garbage_collector);
    return UNITY_END();
}