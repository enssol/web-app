#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "http_request_header_parser.h"
#include "http_request_body_parser.h"

// Setup function
void
setup(void)
{
    // Initialize resources needed for tests
}

// Teardown function
void
teardown(void)
{
    // Clean up resources used in tests
}

// Test case for HTTP request header parsing
void
test_http_request_header_parser(void)
{
    char *request = "GET / HTTP/1.1\r\nHost: example.com\r\n\r\n";
    struct http_request_header header;

    int result = parse_http_request_header(request, &header);
    assert(result == 0);
    assert(strcmp(header.method, "GET") == 0);
    assert(strcmp(header.uri, "/") == 0);
    assert(strcmp(header.version, "HTTP/1.1") == 0);
    assert(strcmp(header.host, "example.com") == 0);
}

// Test case for HTTP request body parsing
void
test_http_request_body_parser(void)
{
    char *body = "key1=value1&key2=value2";
    struct http_request_body parsed_body;

    int result = parse_http_request_body(body, &parsed_body);
    assert(result == 0);
    assert(strcmp(parsed_body.key1, "value1") == 0);
    assert(strcmp(parsed_body.key2, "value2") == 0);
}

// Main function to run tests
int
main(void)
{
    setup();

    test_http_request_header_parser();
    test_http_request_body_parser();

    teardown();

    printf("All tests passed.\n");
    return 0;
}
