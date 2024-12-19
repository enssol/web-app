/* filepath: test_web_server.c */
/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>     /* Add this for clock_gettime */
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "../include/web_server.h"

#define TEST_PORT 8080
#define TEST_BUFFER_SIZE 1024
#define TEST_TIMEOUT_SEC 5
#define SERVER_STRING "Server: EnvEngWebServer/1.0\r\n"

/* Add missing constants to match web_server.c */
#define INDEX_FILE "www/test_index.html"
#define BACKLOG 10

/* Add after the existing #define statements */
#define MAX_RESPONSE_SIZE 4096
#define TEST_INDEX_CONTENT "<html>\n</html>\n"

/* Test server state */
static int server_socket = -1;
static pthread_t server_thread;
static volatile sig_atomic_t server_running = 0;
static pthread_mutex_t server_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t server_cond = PTHREAD_COND_INITIALIZER;

/* Function prototypes */
static void *test_server_thread(void *arg);
static int test_setup(void);
static int test_teardown(void);
static void test_server_invalid_request(void);
static int wait_for_server_start(void);
int main(void);

/* Add these test function prototypes */
static void test_server_valid_get_request(void);
static void test_server_file_not_found(void);
static void test_server_internal_error(void);

static int
create_test_directory(void)
{
    FILE *index_file;

    /* Create www directory if it doesn't exist */
    if (mkdir(WWW_ROOT, 0755) < 0 && errno != EEXIST) {
        return -1;
    }

    /* Create test index file */
    index_file = fopen(INDEX_FILE, "w");
    if (index_file == NULL) {
        return -1;
    }

    if (fprintf(index_file, "%s", TEST_INDEX_CONTENT) < 0) {
        fclose(index_file);
        return -1;
    }

    fclose(index_file);
    return 0;
}

static int
wait_for_server_start(void)
{
    struct timespec timeout;
    int result;

    pthread_mutex_lock(&server_mutex);
    if (!server_running) {
        clock_gettime(CLOCK_REALTIME, &timeout);
        timeout.tv_sec += TEST_TIMEOUT_SEC;
        result = pthread_cond_timedwait(&server_cond, &server_mutex, &timeout);
        if (result == ETIMEDOUT) {
            pthread_mutex_unlock(&server_mutex);
            return -1;
        }
    }
    pthread_mutex_unlock(&server_mutex);
    return 0;
}

static int
test_setup(void)
{
    FILE *index_file;

    if (create_test_directory() < 0) {
        return -1;
    }

    /* Create test index file */
    index_file = fopen("www/index.html", "w");
    if (index_file == NULL) {
        return -1;
    }
    fprintf(index_file, "%s", TEST_INDEX_CONTENT);
    fclose(index_file);

    /* Start test server thread */
    server_running = 0;
    if (pthread_create(&server_thread, NULL, test_server_thread, NULL) != 0) {
        return -1;
    }

    /* Wait for server to start */
    if (wait_for_server_start() < 0) {
        return -1;
    }

    return 0;
}

static int
test_teardown(void)
{
    /* Stop server thread */
    server_running = 0;
    if (server_socket != -1) {
        close(server_socket);
        server_socket = -1;
    }

    pthread_join(server_thread, NULL);

    /* Cleanup test files */
    unlink("www/index.html");
    rmdir(WWW_ROOT);

    return 0;
}

static void *
test_server_thread(void *arg)
{
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t client_len;
    int server_fd;
    (void)arg;

    server_fd = setup_server(TEST_PORT);
    if (server_fd < 0) {
        return NULL;
    }

    server_socket = server_fd;
    pthread_mutex_lock(&server_mutex);
    server_running = 1;
    pthread_cond_signal(&server_cond);
    pthread_mutex_unlock(&server_mutex);

    while (server_running) {
        client_len = sizeof(client_addr);
        client_socket = accept(server_socket,
                             (struct sockaddr *)&client_addr,
                             &client_len);
        if (client_socket < 0) {
            if (errno == EINTR) {
                continue;
            }
            break;
        }
        handle_client(client_socket, WWW_ROOT);
        close(client_socket);
    }

    return NULL;
}

/* Test functions */
static void
test_server_invalid_request(void)
{
    int client_fd;
    char response[MAX_RESPONSE_SIZE];
    ssize_t bytes_received;
    struct sockaddr_in addr;
    const char *request = "INVALID REQUEST\r\n\r\n";

    /* Create client socket */
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    CU_ASSERT(client_fd >= 0);

    /* Setup connection */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(TEST_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    /* Connect to server */
    CU_ASSERT(connect(client_fd, (struct sockaddr *)&addr, sizeof(addr)) >= 0);

    /* Send invalid request */
    CU_ASSERT(write(client_fd, request, strlen(request)) == (ssize_t)strlen(request));

    /* Read response */
    memset(response, 0, sizeof(response));
    bytes_received = read(client_fd, response, sizeof(response) - 1);
    CU_ASSERT(bytes_received > 0);

    /* Verify error response */
    CU_ASSERT(strstr(response, "HTTP/1.0 400 Bad Request") != NULL);

    close(client_fd);
}

static void
test_server_valid_get_request(void)
{
    int client_fd;
    char response[MAX_RESPONSE_SIZE];
    ssize_t bytes_received;
    struct sockaddr_in addr;
    const char *request = "GET /index.html HTTP/1.0\r\n\r\n";

    /* Create client socket */
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    CU_ASSERT(client_fd >= 0);

    /* Setup connection */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(TEST_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    /* Connect to server */
    CU_ASSERT(connect(client_fd, (struct sockaddr *)&addr, sizeof(addr)) >= 0);

    /* Send valid GET request */
    CU_ASSERT(write(client_fd, request, strlen(request)) == (ssize_t)strlen(request));

    /* Read response */
    memset(response, 0, sizeof(response));
    bytes_received = read(client_fd, response, sizeof(response) - 1);
    CU_ASSERT(bytes_received > 0);

    /* Only verify HTTP response code */
    CU_ASSERT(strstr(response, "HTTP/1.0 200 OK") != NULL);
    /* Verify it's HTML but don't check specific content */
    CU_ASSERT(strstr(response, "<html>") != NULL);

    close(client_fd);
}

static void
test_server_file_not_found(void)
{
    int client_fd;
    char response[MAX_RESPONSE_SIZE];
    ssize_t bytes_received;
    struct sockaddr_in addr;
    const char *request = "GET /nonexistent.html HTTP/1.0\r\n\r\n";

    /* Create client socket */
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    CU_ASSERT(client_fd >= 0);

    /* Setup connection */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(TEST_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    /* Connect to server */
    CU_ASSERT(connect(client_fd, (struct sockaddr *)&addr, sizeof(addr)) >= 0);

    /* Send request for non-existent file */
    CU_ASSERT(write(client_fd, request, strlen(request)) == (ssize_t)strlen(request));

    /* Read response */
    memset(response, 0, sizeof(response));
    bytes_received = read(client_fd, response, sizeof(response) - 1);
    CU_ASSERT(bytes_received > 0);

    /* Verify 404 response */
    CU_ASSERT(strstr(response, "HTTP/1.0 404 Not Found") != NULL);

    close(client_fd);
}

static void
test_server_internal_error(void)
{
    int client_fd;
    char response[MAX_RESPONSE_SIZE];
    ssize_t bytes_received;
    struct sockaddr_in addr;
    /* Request with malformed URI to trigger internal error */
    const char *request = "GET /../../etc/passwd HTTP/1.0\r\n\r\n";

    /* Create client socket */
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    CU_ASSERT(client_fd >= 0);

    /* Setup connection */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(TEST_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    /* Connect to server */
    CU_ASSERT(connect(client_fd, (struct sockaddr *)&addr, sizeof(addr)) >= 0);

    /* Send request with path traversal attempt */
    CU_ASSERT(write(client_fd, request, strlen(request)) == (ssize_t)strlen(request));

    /* Read response */
    memset(response, 0, sizeof(response));
    bytes_received = read(client_fd, response, sizeof(response) - 1);
    CU_ASSERT(bytes_received > 0);

    /* Verify 500 response */
    CU_ASSERT(strstr(response, "HTTP/1.0 500 Internal Server Error") != NULL);

    close(client_fd);
}

/* main test function */
int
main(void)
{
    CU_pSuite suite = NULL;

    /* Initialize CUnit test registry */
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    /* Add suite to registry */
    suite = CU_add_suite("Web Server Tests", test_setup, test_teardown);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add tests to suite */
    if ((CU_add_test(suite, "Test Valid GET Request",
                     test_server_valid_get_request) == NULL) ||
        (CU_add_test(suite, "Test Invalid Request",
                     test_server_invalid_request) == NULL) ||
        (CU_add_test(suite, "Test File Not Found",
                     test_server_file_not_found) == NULL) ||
        (CU_add_test(suite, "Test Internal Error",
                     test_server_internal_error) == NULL)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run tests */
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
