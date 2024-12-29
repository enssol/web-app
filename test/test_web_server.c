/* filepath: test/test_web_server.c */
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../include/web_server.h"
#include "mock_socket.h"
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

/* Test timeouts and constants */
#define TEST_TIMEOUT_MS 1000

/* Function declarations */
int init_web_server_suite(void);
int clean_web_server_suite(void);
void test_handle_request(void);
void test_handle_request_not_found(void);
void test_handle_invalid_request(void);

/* Test suite setup/teardown */
int
init_web_server_suite(void)
{
    mock_socket_reset();
    return 0;
}

int
clean_web_server_suite(void)
{
    return 0;
}

void
test_handle_request(void)
{
    int sock_fd;
    const char *request;
    const char *response;

    request = "GET / HTTP/1.1\r\n"
             "Host: localhost\r\n"
             "Connection: close\r\n"
             "\r\n";
    response = NULL;

    /* Setup mock socket */
    mock_socket_reset();
    sock_fd = mock_socket(AF_INET, SOCK_STREAM, 0);
    CU_ASSERT(sock_fd >= 0);

    /* Setup mock receive data */
    mock_socket_set_recv_data(request, strlen(request));

    /* Test handle_client */
    CU_ASSERT_EQUAL_FATAL(handle_client(sock_fd, WWW_ROOT), 0); /* Success expected */

    response = mock_socket_get_sent_data();
    CU_ASSERT_PTR_NOT_NULL_FATAL(response);
    CU_ASSERT_PTR_NOT_NULL(strstr(response, "HTTP/1.0 200 OK"));
}

void
test_handle_request_not_found(void)
{
    int sock_fd;
    const char *request;
    const char *response;

    request = "GET /nonexistent.html HTTP/1.1\r\n"
             "Host: localhost\r\n"
             "Connection: close\r\n"
             "\r\n";
    response = NULL;

    /* Setup mock socket */
    mock_socket_reset();
    sock_fd = mock_socket(AF_INET, SOCK_STREAM, 0);
    CU_ASSERT(sock_fd >= 0);

    /* Setup mock receive data */
    mock_socket_set_recv_data(request, strlen(request));

    /* Test handle_client */
    CU_ASSERT_EQUAL_FATAL(handle_client(sock_fd, WWW_ROOT), -1); /* Error expected */

    response = mock_socket_get_sent_data();
    CU_ASSERT_PTR_NOT_NULL_FATAL(response);
    CU_ASSERT_PTR_NOT_NULL(strstr(response, "HTTP/1.0 404 Not Found"));
}

void
test_handle_invalid_request(void)
{
    int sock_fd;
    const char *invalid_request;
    const char *path_traversal;
    const char *response;

    invalid_request = "INVALID / HTTP/1.1\r\n"
                     "Host: localhost\r\n"
                     "\r\n";

    path_traversal = "GET /../illegal/path HTTP/1.1\r\n"
                    "Host: localhost\r\n"
                    "\r\n";
    response = NULL;

    /* Test invalid method */
    mock_socket_reset();
    sock_fd = mock_socket(AF_INET, SOCK_STREAM, 0);
    CU_ASSERT(sock_fd >= 0);

    mock_socket_set_recv_data(invalid_request, strlen(invalid_request));
    CU_ASSERT_EQUAL_FATAL(handle_client(sock_fd, WWW_ROOT), -1);

    response = mock_socket_get_sent_data();
    CU_ASSERT_PTR_NOT_NULL_FATAL(response);
    CU_ASSERT_PTR_NOT_NULL(strstr(response, "HTTP/1.0 400 Bad Request"));

    /* Test path traversal */
    mock_socket_reset();
    sock_fd = mock_socket(AF_INET, SOCK_STREAM, 0);
    CU_ASSERT(sock_fd >= 0);

    mock_socket_set_recv_data(path_traversal, strlen(path_traversal));
    CU_ASSERT_EQUAL_FATAL(handle_client(sock_fd, WWW_ROOT), -1);

    response = mock_socket_get_sent_data();
    CU_ASSERT_PTR_NOT_NULL_FATAL(response);
    CU_ASSERT_PTR_NOT_NULL(strstr(response, "HTTP/1.0 403 Forbidden"));
}
