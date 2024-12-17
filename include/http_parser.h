/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */
/* filepath: include/http_parser.h */
#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

/* Dependencies */
#include "common.h"
#include "tcp.h"

/* Constants */
#define MAX_HEADER_SIZE 8192
#define MAX_HEADERS 50
#define MAX_METHOD_LEN 8
#define HTTP_MAX_PATH_LEN 2048  /* Changed from MAX_PATH_LEN */
#define MAX_HEADER_NAME 128
#define MAX_HEADER_VALUE 1024

/* HTTP Methods */
enum http_method {
    HTTP_GET,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_HEAD,
    HTTP_OPTIONS,
    HTTP_UNKNOWN
};

/* HTTP Version */
enum http_version {
    HTTP_1_0,
    HTTP_1_1,
    HTTP_VERSION_UNKNOWN
};

/* Parser Error Codes */
enum parser_error {
    /* Success States */
    PARSER_OK = 0,
    PARSER_START,
    PARSER_METHOD,
    PARSER_PATH,
    PARSER_VERSION,
    PARSER_HEADER_NAME,
    PARSER_HEADER_VALUE,
    PARSER_BODY,
    PARSER_DONE,

    /* Error States */
    PARSER_ERROR = -1,
    PARSER_INVALID_METHOD = -2,
    PARSER_INVALID_PATH = -3,
    PARSER_INVALID_VERSION = -4,
    PARSER_HEADER_TOO_LARGE = -5,
    PARSER_TOO_MANY_HEADERS = -6,
    PARSER_MALFORMED_REQUEST = -7,
    PARSER_BODY_TOO_LARGE = -8
};

/* HTTP Header Structure */
struct http_header {
    char name[MAX_HEADER_NAME];
    char value[MAX_HEADER_VALUE];
};

/* HTTP Request Structure */
struct http_request {
    enum http_method method;
    char path[HTTP_MAX_PATH_LEN];  /* Use new constant */
    enum http_version version;
    struct http_header headers[MAX_HEADERS];
    size_t num_headers;
    char *body;
    size_t body_length;
};

/* Function Prototypes */
int httpParserInit(void);
int httpParseRequest(struct tcp_connection *conn, struct http_request *request);
const char *httpMethodToString(enum http_method method);
enum http_method httpStringToMethod(const char *method);
const char *httpVersionToString(enum http_version version);
enum http_version httpStringToVersion(const char *version);
const char *httpParserErrorString(enum parser_error error);
void httpParserCleanup(void);

#endif /* HTTP_PARSER_H */
