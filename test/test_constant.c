#include <CUnit/Basic.h>
#include "../include/constant.h"
#include "test_suites.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>


/* Test fixtures */
static int test_client[2];
static const char *test_www_root = "test";

/* Setup and teardown */
static int suite_setup(void) {
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, test_client) != 0) {
        return -1;
    }
    return 0;
}

static int suite_teardown(void) {
    close(test_client[0]);
    close(test_client[1]);
    return 0;
}

/* Test cases */

static void test_handle_users_request(void) {
    char response[1024];
    ssize_t bytes;
    int result;
    
    result = handle_users_request(test_client[1]);
    CU_ASSERT_EQUAL(result, 0);
    bytes = read(test_client[0], response, sizeof(response) - 1);
    CU_ASSERT(bytes > 0);
    response[bytes] = '\0';
    CU_ASSERT(strncmp(response, "HTTP/1.0 200 OK", 15) == 0);
}


static void test_handle_auth_request(void) {
    const char *valid_user = "john";
    const char *valid_pass = "smith";
    
    int result = check_auth(valid_user, valid_pass);
    CU_ASSERT_EQUAL(result, 1);
    
    result = check_auth("invalid", "invalid");
    CU_ASSERT_EQUAL(result, 0);
}

static void test_parse_query_string(void) {
    char username[256];
    char password[256];
    const char *query = "username=test&password=pass123";
    
    parse_query_string(query, username, password);
    
    CU_ASSERT_STRING_EQUAL(username, "test");
    CU_ASSERT_STRING_EQUAL(password, "pass123");
}

static void test_handle_update_user(void) {
    const char *username = "testuser";
    const char *fullname = "Test User";
    const char *email = "test@example.com";
    const char *project = "testproject";
    
    int result = handle_update_user(test_client[1], username, fullname, email, project);
    CU_ASSERT_EQUAL(result, 0);
}

static void test_serve_file_found(void) {
    const char *test_uri = "/test_index.html";
    char buf[MAX_BUFFER_SIZE];
    int result;
    
    snprintf(buf, sizeof(buf), "GET %s HTTP/1.0\r\n\r\n", test_uri);
    write(test_client[0], buf, strlen(buf));
    
    result = handle_client(test_client[1], test_www_root);
    
    CU_ASSERT(result >= 0);
}

static void test_serve_file_not_found(void) {
    const char *test_uri = "/invalid_uri";
    char buf[MAX_BUFFER_SIZE];
    int result;
    
    snprintf(buf, sizeof(buf), "GET %s HTTP/1.0\r\n\r\n", test_uri);
    write(test_client[0], buf, strlen(buf));

    result = handle_client(test_client[1], test_www_root);
    
    CU_ASSERT(result < 0);
}

/* Test suite initialization */
int init_constant_suite(CU_pSuite suite) {
    suite->pInitializeFunc = suite_setup;
    suite->pCleanupFunc = suite_teardown;
    
    CU_add_test(suite, "Test Handle Users Request", test_handle_users_request);
    CU_add_test(suite, "Test Handle Auth Request", test_handle_auth_request);
    CU_add_test(suite, "Test Parse Query String", test_parse_query_string);
    CU_add_test(suite, "Test Handle Update User", test_handle_update_user);
    CU_add_test(suite, "Test Handle Client (Valid File)", test_serve_file_found);
    CU_add_test(suite, "Test Handle Client (Invalid File)", test_serve_file_not_found);
    
    return 0;
}
