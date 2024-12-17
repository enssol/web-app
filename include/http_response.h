/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */
/* filepath: include/http_response.h */
#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

/* Dependencies */
#include "common.h"
#include "tcp.h"
#include "cache.h"

/* Constants */
#define MAX_RESPONSE_HEADERS 50
#define MAX_STATUS_LINE 256
#define MAX_REASON_PHRASE 64
#define MAX_HEADER_NAME 128
#define MAX_HEADER_VALUE 1024
#define DEFAULT_HTTP_VERSION "HTTP/1.1"

/* HTTP status codes */
enum http_status {
    HTTP_STATUS_OK = 200,
    HTTP_STATUS_CREATED = 201,
    HTTP_STATUS_ACCEPTED = 202,
    HTTP_STATUS_NO_CONTENT = 204,
    HTTP_STATUS_MOVED_PERMANENTLY = 301,
    HTTP_STATUS_FOUND = 302,
    HTTP_STATUS_NOT_MODIFIED = 304,
    HTTP_STATUS_BAD_REQUEST = 400,
    HTTP_STATUS_UNAUTHORIZED = 401,
    HTTP_STATUS_FORBIDDEN = 403,
    HTTP_STATUS_NOT_FOUND = 404,
    HTTP_STATUS_METHOD_NOT_ALLOWED = 405,
    HTTP_STATUS_REQUEST_TIMEOUT = 408,
    HTTP_STATUS_LENGTH_REQUIRED = 411,
    HTTP_STATUS_PAYLOAD_TOO_LARGE = 413,
    HTTP_STATUS_URI_TOO_LONG = 414,
    HTTP_STATUS_INTERNAL_ERROR = 500,
    HTTP_STATUS_NOT_IMPLEMENTED = 501,
    HTTP_STATUS_BAD_GATEWAY = 502,
    HTTP_STATUS_SERVICE_UNAVAILABLE = 503
};

/* Response header structure */
struct http_response_header {
    char name[MAX_HEADER_NAME];
    char value[MAX_HEADER_VALUE];
};

/* HTTP response structure */
struct http_response {
    enum http_status status;
    char reason[MAX_REASON_PHRASE];
    struct http_response_header headers[MAX_RESPONSE_HEADERS];
    size_t num_headers;
    void *body;
    size_t body_length;
};

/* Response error codes */
enum response_error {
    RESPONSE_OK = 0,
    RESPONSE_INVALID_STATUS = -1,
    RESPONSE_HEADER_TOO_LARGE = -2,
    RESPONSE_TOO_MANY_HEADERS = -3,
    RESPONSE_BODY_TOO_LARGE = -4,
    RESPONSE_SEND_ERROR = -5,
    RESPONSE_INVALID_PARAM = -6
};

/* Function prototypes */
int httpResponseInit(void);
int httpResponseCreate(struct http_response *response, enum http_status status);
int httpResponseAddHeader(struct http_response *response,
                         const char *name,
                         const char *value);
int httpResponseSetBody(struct http_response *response,
                       const void *body,
                       size_t length);
int httpResponseSend(struct tcp_connection *conn,
                    const struct http_response *response);
const char *httpStatusToString(enum http_status status);
void httpResponseCleanup(struct http_response *response);
const char *httpResponseGetError(enum response_error error);

#endif /* HTTP_RESPONSE_H */
