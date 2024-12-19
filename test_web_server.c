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
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>  /* Add this at the top with other includes */

#define TEST_PORT 8080
#define TEST_BUFFER_SIZE 1024
#define TEST_TIMEOUT_SEC 5
#define SERVER_STRING "Server: EnvEngWebServer/1.0\r\n"

/* Add missing constants to match web_server.c */
#define INDEX_FILE "test_index.html"
#define BACKLOG 10

/* Add after the existing #define statements */
#define MAX_RESPONSE_SIZE 4096

/* Add after other #define statements */
#define TEST_INDEX_CONTENT \
    "<!DOCTYPE html>\n" \
    "<html lang=\"en\">\n" \
    "<head>\n" \
    "  <meta charset=\"UTF-8\">\n" \
    "  <title>Hello World</title>\n" \
    "</head>\n" \
    "<body>\n" \
    "  <h1>Hello World</h1>\n" \
    "</body>\n" \
    "</html>\n"

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

/* Add these test function prototypes */
static void test_server_valid_get_request(void);
static void test_server_file_not_found(void);
static void test_server_internal_error(void);

static int
wait_for_server_start(void)
{
    struct timespec timeout;
    struct timeval now;
    int result;

    gettimeofday(&now, NULL);
    timeout.tv_sec = now.tv_sec + TEST_TIMEOUT_SEC;
    timeout.tv_nsec = now.tv_usec * 1000;

    pthread_mutex_lock(&server_mutex);
    while (!server_running) {
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
    struct sockaddr_in server_addr;
    int optval;
    FILE *index_file;

    /* Create server socket */
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        return -1;
    }

    optval = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR,
                   &optval, sizeof(optval)) < 0) {
        close(server_socket);
        return -1;
    }

    /* Initialize server address */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(TEST_PORT);

    /* Bind and listen */
    if (bind(server_socket, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0) {
        close(server_socket);
        return -1;
    }

    if (listen(server_socket, 1) < 0) {
        close(server_socket);
        return -1;
    }

    /* Create test_index.html with same content as index.html */
    index_file = fopen("test_index.html", "w");
    if (index_file == NULL) {
        close(server_socket);
        return -1;
    }
    fprintf(index_file, "%s", TEST_INDEX_CONTENT);
    fclose(index_file);

    /* Start server thread */
    server_running = 1;
    if (pthread_create(&server_thread, NULL, test_server_thread, NULL) != 0) {
        unlink("test_index.html");
        close(server_socket);
        return -1;
    }

    /* Wait for server to start */
    if (wait_for_server_start() < 0) {
        server_running = 0;
        pthread_join(server_thread, NULL);
        unlink("test_index.html");
        close(server_socket);
        return -1;
    }

    return 0;
}

static int
test_teardown(void)
{
    /* Signal thread to stop */
    pthread_mutex_lock(&server_mutex);
    server_running = 0;
    pthread_cond_signal(&server_cond);
    pthread_mutex_unlock(&server_mutex);

    /* Close socket to interrupt accept() */
    if (server_socket != -1) {
        shutdown(server_socket, SHUT_RDWR);
        close(server_socket);
        server_socket = -1;
    }

    /* Wait for thread to finish */
    pthread_join(server_thread, NULL);

    /* Clean up synchronization primitives */
    pthread_mutex_destroy(&server_mutex);
    pthread_cond_destroy(&server_cond);

    /* Remove test file */
    unlink("test_index.html");

    return 0;
}

static void *
test_server_thread(void *arg)
{
    struct sockaddr_in client_addr;
    socklen_t client_len;
    int client_socket;
    char buffer[TEST_BUFFER_SIZE];
    char response_header[TEST_BUFFER_SIZE];
    int bytes_read;
    int file_fd;
    struct stat file_stat;
    ssize_t bytes_written;

    /* Avoid unused parameter warning */
    (void)arg;

    /* Signal server is ready */
    pthread_mutex_lock(&server_mutex);
    pthread_cond_signal(&server_cond);
    pthread_mutex_unlock(&server_mutex);

    while (server_running) {
        client_len = sizeof(client_addr);
        client_socket = accept(server_socket,
                             (struct sockaddr *)&client_addr,
                             &client_len);

        if (client_socket < 0) {
            if (errno == EINTR || errno == EBADF) {
                break;
            }
            continue;
        }

        bytes_read = read(client_socket, buffer, TEST_BUFFER_SIZE - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';

            if (strncmp(buffer, "GET / ", 6) == 0) {
                file_fd = open("test_index.html", O_RDONLY);
                if (file_fd < 0 || fstat(file_fd, &file_stat) < 0) {
                    /* Internal Server Error - exact format from web_server.c */
                    snprintf(response_header, sizeof(response_header),
                            "HTTP/1.0 500 Internal Server Error\r\n"
                            "%s"
                            "Content-Type: text/plain\r\n"
                            "\r\n"
                            "500 Internal Server Error\r\n",
                            SERVER_STRING);
                    write(client_socket, response_header, strlen(response_header));
                    if (file_fd >= 0) {
                        close(file_fd);
                    }
                } else {
                    /* Success response - match exact header order from web_server.c */
                    snprintf(response_header, sizeof(response_header),
                            "HTTP/1.0 200 OK\r\n"
                            "%s"
                            "Content-Type: text/html\r\n"
                            "Content-Length: %ld\r\n"
                            "\r\n",
                            SERVER_STRING,
                            (long)file_stat.st_size);
                    bytes_written = write(client_socket, response_header,
                                        strlen(response_header));
                    if (bytes_written > 0) {
                        /* Send file content in chunks */
                        while ((bytes_read = read(file_fd, buffer,
                               TEST_BUFFER_SIZE)) > 0) {
                            bytes_written = write(client_socket, buffer,
                                               (size_t)bytes_read);
                            if (bytes_written < 0) {
                                break;
                            }
                        }
                    }
                    close(file_fd);
                }
            } else {
                /* Not Found response - exact format from web_server.c */
                snprintf(response_header, sizeof(response_header),
                        "HTTP/1.0 404 Not Found\r\n"
                        "%s"
                        "Content-Type: text/plain\r\n"
                        "\r\n"
                        "404 Not Found\r\n",
                        SERVER_STRING);
                write(client_socket, response_header, strlen(response_header));
            }
        }

        close(client_socket);
    }

    return NULL;
}

/* Test functions */
static void
test_server_invalid_request(void)
{
    struct sockaddr_in addr;
    int client_socket;
    int connect_result;
    char buffer[TEST_BUFFER_SIZE];
    const char *test_request = "INVALID / HTTP/1.0\r\n\r\n";

    /* Create client socket */
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    CU_ASSERT(client_socket >= 0);

    /* Setup connection address */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(TEST_PORT);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    /* Connect to server */
    connect_result = connect(client_socket,
                           (struct sockaddr *)&addr,
                           sizeof(addr));
    CU_ASSERT(connect_result >= 0);

    if (connect_result >= 0) {
        /* Send invalid request */
        write(client_socket, test_request, strlen(test_request));

        /* Read response */
        memset(buffer, 0, TEST_BUFFER_SIZE);
        read(client_socket, buffer, TEST_BUFFER_SIZE - 1);

        /* Verify response contains 404 Not Found (matching web_server.c behavior) */
        CU_ASSERT(strstr(buffer, "HTTP/1.0 404 Not Found") != NULL);
        CU_ASSERT(strstr(buffer, "Content-Type: text/plain") != NULL);
        CU_ASSERT(strstr(buffer, "404 Not Found") != NULL);
    }

    close(client_socket);
}

static void
test_server_valid_get_request(void)
{
    struct sockaddr_in addr;
    int client_socket;
    int connect_result;
    char buffer[TEST_BUFFER_SIZE];
    const char *test_request = "GET / HTTP/1.0\r\n\r\n";
    int bytes_read;
    char *response;

    /* Create and connect socket like in test_server_invalid_request */
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    CU_ASSERT(client_socket >= 0);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(TEST_PORT);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    connect_result = connect(client_socket,
                           (struct sockaddr *)&addr,
                           sizeof(addr));
    CU_ASSERT(connect_result >= 0);

    if (connect_result >= 0) {
        write(client_socket, test_request, strlen(test_request));

        memset(buffer, 0, TEST_BUFFER_SIZE);
        bytes_read = read(client_socket, buffer, TEST_BUFFER_SIZE - 1);
        buffer[bytes_read] = '\0';

        /* Verify response contains success indicators */
        CU_ASSERT(strstr(buffer, "HTTP/1.0 200 OK") != NULL);
        CU_ASSERT(strstr(buffer, "Content-Type: text/html") != NULL);

        /* Verify content */
        response = strstr(buffer, "\r\n\r\n");
        CU_ASSERT(response != NULL);
        if (response != NULL) {
            response += 4;  /* Skip double CRLF */
            CU_ASSERT(strstr(response, "<html") != NULL);
        }
    }

    close(client_socket);
}

static void
test_server_file_not_found(void)
{
    struct sockaddr_in addr;
    int client_socket;
    int connect_result;
    char buffer[TEST_BUFFER_SIZE];
    const char *test_request = "GET /nonexistent HTTP/1.0\r\n\r\n";
    int bytes_read;

    /* Create and connect socket */
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    CU_ASSERT(client_socket >= 0);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(TEST_PORT);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    connect_result = connect(client_socket,
                           (struct sockaddr *)&addr,
                           sizeof(addr));
    CU_ASSERT(connect_result >= 0);

    if (connect_result >= 0) {
        write(client_socket, test_request, strlen(test_request));

        memset(buffer, 0, TEST_BUFFER_SIZE);
        bytes_read = read(client_socket, buffer, TEST_BUFFER_SIZE - 1);
        buffer[bytes_read] = '\0';

        /* Verify 404 response */
        CU_ASSERT(strstr(buffer, "HTTP/1.0 404 Not Found") != NULL);
        CU_ASSERT(strstr(buffer, "Content-Type: text/plain") != NULL);
        CU_ASSERT(strstr(buffer, "404 Not Found") != NULL);
    }

    close(client_socket);
}

static void
test_server_internal_error(void)
{
    struct sockaddr_in addr;
    int client_socket;
    int connect_result;
    char buffer[TEST_BUFFER_SIZE];
    const char *test_request = "GET / HTTP/1.0\r\n\r\n";
    FILE *index_file;
    int bytes_read;

    /* Create client socket */
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    CU_ASSERT(client_socket >= 0);

    /* Setup connection address */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(TEST_PORT);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    /* Create index file without read permissions */
    index_file = fopen(INDEX_FILE, "w");
    if (index_file != NULL) {
        fclose(index_file);
        chmod(INDEX_FILE, 0000);

        /* Connect to server */
        connect_result = connect(client_socket,
                               (struct sockaddr *)&addr,
                               sizeof(addr));
        CU_ASSERT(connect_result >= 0);

        if (connect_result >= 0) {
            /* Send request */
            write(client_socket, test_request, strlen(test_request));

            /* Read response */
            memset(buffer, 0, TEST_BUFFER_SIZE);
            bytes_read = read(client_socket, buffer, TEST_BUFFER_SIZE - 1);
            buffer[bytes_read] = '\0';

            /* Verify 500 response */
            CU_ASSERT(strstr(buffer, "HTTP/1.0 500 Internal Server Error") != NULL);
            CU_ASSERT(strstr(buffer, "Content-Type: text/plain") != NULL);
            CU_ASSERT(strstr(buffer, "500 Internal Server Error") != NULL);
        }

        /* Reset permissions and cleanup */
        chmod(INDEX_FILE, 0644);
        unlink(INDEX_FILE);
    }

    close(client_socket);
}

/* Main test function */
int
main(void)
{
    CU_pSuite suite;

    /* Initialize CUnit test registry */
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    /* Create suite */
    suite = CU_add_suite("Web Server Tests", test_setup, test_teardown);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add tests to suite */
    if (NULL == CU_add_test(suite, "Test Server Invalid Request",
                           test_server_invalid_request) ||
        NULL == CU_add_test(suite, "Test Server Valid GET Request",
                           test_server_valid_get_request) ||
        NULL == CU_add_test(suite, "Test Server File Not Found",
                           test_server_file_not_found) ||
        NULL == CU_add_test(suite, "Test Server Internal Error",
                           test_server_internal_error)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run tests using basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    /* Clean up registry and return */
    CU_cleanup_registry();
    return CU_get_error();
}
