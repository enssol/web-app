#include "http_request_mock.h"
#include <string.h>

// Mock function for HTTP GET request
int
mock_http_get(const char *url, char *response, size_t response_size)
{
    // ...existing code...
    strncpy(response, "mocked_response", response_size);
    return 0;
}

// Mock function for HTTP POST request
int
mock_http_post(const char *url, const char *data, char *response, size_t response_size)
{
    // ...existing code...
    strncpy(response, "mocked_response", response_size);
    return 0;
}
