/* filepath: /home/appuser/web-app/test/test_web_server.c */
/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include <CUnit/Basic.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include "../include/web_server.h"
#include "test_suites.h"

#define BUFFER_SIZE 1024
#define TEST_PORT 8081

/* Test fixture */
static int test_socket;
static const char *TEST_USERNAME = "testuser";
static const char *TEST_ACTION = "login";
static const char *TEST_QUERY = "username=john&password=doe123";

/* Setup and teardown */
static int
suite_setup(void)
{
    test_socket = setup_server(TEST_PORT);
    return (test_socket < 0) ? -1 : 0;
}

static int
suite_teardown(void)
{
    if (test_socket >= 0) {
        close(test_socket);
    }
    return 0;
}

/* Test cases */
static void
test_server_setup(void)
{
    int socket_fd;
    socket_fd = setup_server(TEST_PORT + 1);
    CU_ASSERT(socket_fd >= 0);
}

static void test_log_audit(void) {
    /* Test valid input */
    int result = log_audit(TEST_USERNAME, TEST_ACTION);
    CU_ASSERT_EQUAL(result, 0);
    
    /* Test invalid inputs */
    result = log_audit(NULL, TEST_ACTION);
    CU_ASSERT_EQUAL(result, -1);
    
    result = log_audit(TEST_USERNAME, NULL);
    CU_ASSERT_EQUAL(result, -1);
}

static void test_parse_query_string(void) {
    char username[256];
    char password[256];
    
    /* Test valid query string */
    parse_query_string(TEST_QUERY, username, password);
    CU_ASSERT_STRING_EQUAL(username, "john");
    CU_ASSERT_STRING_EQUAL(password, "doe123");
    
    /* Test empty query  */
    parse_query_string("", username, password);
    CU_ASSERT_STRING_EQUAL(username, "");
    CU_ASSERT_STRING_EQUAL(password, "");
    
    /* Test malformed query string */
    parse_query_string("username=&password=", username, password);
    CU_ASSERT_STRING_EQUAL(username, "");
    CU_ASSERT_STRING_EQUAL(password, "");
}

int init_web_server_suite(CU_pSuite suite) {
    suite->pInitializeFunc = suite_setup;
    suite->pCleanupFunc = suite_teardown;

    CU_add_test(suite, "Test Setup Server", test_server_setup);
    CU_add_test(suite, "Test Log Audit", test_log_audit);
    CU_add_test(suite, "Test Parse Query String", test_parse_query_string);
    
    return 0;
}