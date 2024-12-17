/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */
/* filepath: test/test_http.c */

#include <CUnit/CUnit.h>
#include "../include/http_parser.h"
#include "../include/http_response.h"
#include "../include/mime.h"
#include "../include/tcp.h"
#include "test_suite.h"
#include <stdio.h>
#include <string.h>

/* Test data */
#define TEST_REQUEST "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n"
#define TEST_RESPONSE "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
#define TEST_BODY "Hello, World!"

/* Mock TCP connection and buffers */
static struct tcp_connection mock_conn;
static char mock_recv_buffer[4096];
static size_t mock_recv_pos;
static size_t mock_recv_len;

/* Function pointer types for mock operations */
typedef ssize_t (*tcp_receive_fn)(struct tcp_connection *, void *, size_t);
typedef ssize_t (*tcp_send_fn)(struct tcp_connection *, const void *, size_t);

/* Mock TCP operations */
static ssize_t
mock_tcp_receive(struct tcp_connection *conn, void *buffer, size_t size)
{
    size_t remaining;
    size_t to_copy;

    (void)conn; /* Unused */

    if (mock_recv_pos >= mock_recv_len) {
        return 0;
    }

    remaining = mock_recv_len - mock_recv_pos;
    to_copy = size < remaining ? size : remaining;
    memcpy(buffer, mock_recv_buffer + mock_recv_pos, to_copy);
    mock_recv_pos += to_copy;

    return (ssize_t)to_copy;
}

static ssize_t
mock_tcp_send(struct tcp_connection *conn, const void *data, size_t size)
{
    (void)conn; /* Unused */
    (void)data; /* Unused */
    return (ssize_t)size;
}

static const struct tcp_operations mock_tcp_ops = {
    mock_tcp_receive,
    mock_tcp_send
};

/* Setup and teardown */
static int
test_setup(void)
{
    int status;

    /* Initialize all components */
    status = httpParserInit();
    if (status != 0) {
        return -1;
    }

    status = httpResponseInit();
    if (status != 0) {
        return -1;
    }

    status = mimeInit();
    if (status != 0) {
        return -1;
    }

    /* Setup mock TCP connection */
    memset(&mock_conn, 0, sizeof(mock_conn));
    mock_conn.ops = &mock_tcp_ops;
    mock_conn.state = TCP_STATE_CONNECTED;
    mock_recv_pos = 0;
    mock_recv_len = 0;

    return 0;
}

static int
test_teardown(void)
{
    httpParserCleanup();
    mimeCleanup();
    return 0;
}

/* Test cases */
void test_http_parser_init(void)
{
    CU_ASSERT_EQUAL(httpParserInit(), 0);
    httpParserCleanup();
    CU_ASSERT_EQUAL(httpParserInit(), 0);
}

void
test_http_request_parsing(void)
{
    struct tcp_connection conn;
    struct http_request request;
    const char *test_data = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";

    /* Setup mock data */
    memcpy(mock_recv_buffer, test_data, strlen(test_data));
    mock_recv_len = strlen(test_data);
    mock_recv_pos = 0;

    /* Setup connection */
    memset(&conn, 0, sizeof(conn));
    conn.ops = &mock_tcp_ops;
    conn.state = TCP_STATE_CONNECTED;

    /* Test parsing */
    CU_ASSERT_EQUAL(httpParseRequest(&conn, &request), 0);
    CU_ASSERT_EQUAL(request.method, HTTP_GET);
    CU_ASSERT_STRING_EQUAL(request.path, "/index.html");
    CU_ASSERT_EQUAL(request.version, HTTP_1_1);
    CU_ASSERT_EQUAL(request.num_headers, 1);
    CU_ASSERT_STRING_EQUAL(request.headers[0].name, "Host");
    CU_ASSERT_STRING_EQUAL(request.headers[0].value, "localhost");
}

void test_http_response_create(void)
{
    struct http_response response;

    CU_ASSERT_EQUAL(httpResponseCreate(&response, HTTP_STATUS_OK), 0);
    CU_ASSERT_EQUAL(response.status, HTTP_STATUS_OK);
    CU_ASSERT_STRING_EQUAL(response.reason, "OK");
    CU_ASSERT_EQUAL(response.num_headers, 0);
    CU_ASSERT_PTR_NULL(response.body);
    CU_ASSERT_EQUAL(response.body_length, 0);
}

void test_http_response_headers(void)
{
    struct http_response response;

    httpResponseCreate(&response, HTTP_STATUS_OK);

    CU_ASSERT_EQUAL(httpResponseAddHeader(&response, "Content-Type", "text/html"), 0);
    CU_ASSERT_EQUAL(response.num_headers, 1);
    CU_ASSERT_STRING_EQUAL(response.headers[0].name, "Content-Type");
    CU_ASSERT_STRING_EQUAL(response.headers[0].value, "text/html");
}

void test_mime_type_handling(void)
{
    const char *mime_type;

    /* Test default MIME types */
    mime_type = mimeGetType("test.html");
    CU_ASSERT_STRING_EQUAL(mime_type, "text/html");

    mime_type = mimeGetType("test.css");
    CU_ASSERT_STRING_EQUAL(mime_type, "text/css");

    mime_type = mimeGetType("test.unknown");
    CU_ASSERT_STRING_EQUAL(mime_type, MIME_DEFAULT_TYPE);

    /* Test custom MIME type */
    CU_ASSERT_EQUAL(mimeAddType(".custom", "application/custom"), 0);
    mime_type = mimeGetType("test.custom");
    CU_ASSERT_STRING_EQUAL(mime_type, "application/custom");
}

void
test_http_error_handling(void)
{
    struct http_request request;
    struct http_response response;
    const char *invalid_request = "INVALID / HTTP/1.1\r\n\r\n";

    /* Test invalid request */
    strcpy(mock_recv_buffer, invalid_request);
    mock_recv_len = strlen(invalid_request);
    mock_recv_pos = 0;

    CU_ASSERT_EQUAL(httpParseRequest(&mock_conn, &request), -1);

    /* Test response error handling */
    CU_ASSERT_EQUAL(httpResponseCreate(NULL, HTTP_STATUS_OK), -1);
    CU_ASSERT_EQUAL(httpResponseCreate(&response, 999), -1);
}

/* Replace the test_http function with this updated version */
int
test_http(void)
{
    CU_pSuite suite = NULL;

    suite = CU_add_suite("HTTP Module Tests", test_setup, test_teardown);
    if (suite == NULL) {
        return -1;
    }

    if ((CU_add_test(suite, "HTTP Parser Initialization", test_http_parser_init) == NULL) ||
        (CU_add_test(suite, "HTTP Request Parsing", test_http_request_parsing) == NULL) ||
        (CU_add_test(suite, "HTTP Response Creation", test_http_response_create) == NULL) ||
        (CU_add_test(suite, "HTTP Response Headers", test_http_response_headers) == NULL) ||
        (CU_add_test(suite, "MIME Type Handling", test_mime_type_handling) == NULL) ||
        (CU_add_test(suite, "HTTP Error Handling", test_http_error_handling) == NULL)) {
        return -1;
    }

    return 0;
}
