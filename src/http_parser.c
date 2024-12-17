/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */
/* src/http_parser.c */

#include "../include/http_parser.h"
#include "../include/app_error.h"
#include "../include/cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>  /* For strcasecmp */
#include <errno.h>
#include <ctype.h>

/* Static variables */
static int is_initialized = 0;
static enum parser_error last_error;

/* Method strings must match enum http_method order */
static const char *http_methods[] = {
    "GET",
    "POST",
    "PUT",
    "DELETE",
    "HEAD",
    "OPTIONS",
    "UNKNOWN"
};

/* Version strings must match enum http_version order */
static const char *http_versions[] = {
    "HTTP/1.0",
    "HTTP/1.1",
    "UNKNOWN"
};

/* Internal function prototypes */
static int parseRequestLine(char *line, struct http_request *request);
static int parseHeaders(struct tcp_connection *conn, struct http_request *request);
static int parseBody(struct tcp_connection *conn, struct http_request *request);
static void cleanupRequest(struct http_request *request);
static char *skipWhitespace(char *str);
static int readLine(struct tcp_connection *conn, char *buffer, size_t size);

int
httpParserInit(void)
{
    if (is_initialized) {
        return 0;
    }
    is_initialized = 1;
    last_error = PARSER_OK;
    return 0;
}

int
httpParseRequest(struct tcp_connection *conn, struct http_request *request)
{
    char line[MAX_HEADER_SIZE];
    int status;
    int line_len;  /* Changed from size_t to int */

    if (!is_initialized || conn == NULL || request == NULL) {
        last_error = PARSER_MALFORMED_REQUEST;
        return -1;
    }

    /* Initialize request structure */
    memset(request, 0, sizeof(struct http_request));
    request->method = HTTP_UNKNOWN;
    request->version = HTTP_VERSION_UNKNOWN;

    /* Read and parse request line */
    line_len = readLine(conn, line, sizeof(line));
    if (line_len <= 0) {
        last_error = PARSER_MALFORMED_REQUEST;
        return -1;
    }

    /* Check for buffer overflow */
    if ((size_t)line_len >= sizeof(line)) {  /* Safe cast after validation */
        last_error = PARSER_MALFORMED_REQUEST;
        return -1;
    }

    /* Validate request line format */
    if (strstr(line, " ") == NULL || strstr(line, "HTTP/") == NULL) {
        last_error = PARSER_MALFORMED_REQUEST;
        return -1;
    }

    status = parseRequestLine(line, request);
    if (status != 0) {
        return -1;
    }

    /* Validate method and version */
    if (request->method == HTTP_UNKNOWN || request->version == HTTP_VERSION_UNKNOWN) {
        last_error = PARSER_MALFORMED_REQUEST;
        return -1;
    }

    /* Parse headers */
    status = parseHeaders(conn, request);
    if (status != 0) {
        cleanupRequest(request);
        return -1;
    }

    /* Parse body if present */
    status = parseBody(conn, request);
    if (status != 0) {
        cleanupRequest(request);
        return -1;
    }

    last_error = PARSER_OK;
    return 0;
}

const char *
httpMethodToString(enum http_method method)
{
    if (method < HTTP_GET || method > HTTP_UNKNOWN) {
        return http_methods[HTTP_UNKNOWN];
    }
    return http_methods[method];
}

enum http_method
httpStringToMethod(const char *method)
{
    size_t i;

    if (method == NULL) {
        return HTTP_UNKNOWN;
    }

    for (i = 0; i < (sizeof(http_methods) / sizeof(http_methods[0])) - 1; i++) {
        if (strcmp(method, http_methods[i]) == 0) {
            return (enum http_method)i;
        }
    }

    return HTTP_UNKNOWN;
}

const char *
httpVersionToString(enum http_version version)
{
    if (version < HTTP_1_0 || version > HTTP_VERSION_UNKNOWN) {
        return http_versions[HTTP_VERSION_UNKNOWN];
    }
    return http_versions[version];
}

enum http_version
httpStringToVersion(const char *version)
{
    size_t i;

    if (version == NULL) {
        return HTTP_VERSION_UNKNOWN;
    }

    for (i = 0; i < (sizeof(http_versions) / sizeof(http_versions[0])) - 1; i++) {
        if (strcmp(version, http_versions[i]) == 0) {
            return (enum http_version)i;
        }
    }

    return HTTP_VERSION_UNKNOWN;
}

const char *
httpParserErrorString(enum parser_error error)
{
    switch (error) {
        case PARSER_OK:
            return "Success";
        case PARSER_INVALID_METHOD:
            return "Invalid HTTP method";
        case PARSER_INVALID_PATH:
            return "Invalid request path";
        case PARSER_INVALID_VERSION:
            return "Invalid HTTP version";
        case PARSER_HEADER_TOO_LARGE:
            return "Header too large";
        case PARSER_TOO_MANY_HEADERS:
            return "Too many headers";
        case PARSER_MALFORMED_REQUEST:
            return "Malformed request";
        case PARSER_BODY_TOO_LARGE:
            return "Request body too large";
        default:
            return "Unknown error";
    }
}

void
httpParserCleanup(void)
{
    is_initialized = 0;
    last_error = PARSER_OK;
}

/* Internal function implementations */
static int
parseRequestLine(char *line, struct http_request *request)
{
    char *method;
    char *path;
    char *version;
    char *saveptr;

    /* Parse method */
    method = strtok_r(line, " ", &saveptr);
    if (method == NULL) {
        last_error = PARSER_INVALID_METHOD;
        return -1;
    }
    request->method = httpStringToMethod(method);

    /* Parse path */
    path = strtok_r(NULL, " ", &saveptr);
    if (path == NULL || strlen(path) >= HTTP_MAX_PATH_LEN) {
        last_error = PARSER_INVALID_PATH;
        return -1;
    }
    strncpy(request->path, path, HTTP_MAX_PATH_LEN - 1);
    request->path[HTTP_MAX_PATH_LEN - 1] = '\0';

    /* Parse version */
    version = strtok_r(NULL, "\r\n", &saveptr);
    if (version == NULL) {
        last_error = PARSER_INVALID_VERSION;
        return -1;
    }
    request->version = httpStringToVersion(version);

    return 0;
}

static int
parseHeaders(struct tcp_connection *conn, struct http_request *request)
{
    char line[MAX_HEADER_SIZE];
    char *name;
    char *value;
    char *saveptr;
    char *end;

    while (1) {
        /* Read header line */
        if (readLine(conn, line, sizeof(line)) <= 0) {
            last_error = PARSER_MALFORMED_REQUEST;
            return -1;
        }

        /* Empty line marks end of headers */
        if (line[0] == '\r' || line[0] == '\n' || line[0] == '\0') {
            break;
        }

        if (request->num_headers >= MAX_HEADERS) {
            last_error = PARSER_TOO_MANY_HEADERS;
            return -1;
        }

        /* Parse header name */
        name = strtok_r(line, ":", &saveptr);
        if (name == NULL) {
            last_error = PARSER_MALFORMED_REQUEST;
            return -1;
        }

        /* Parse and trim header value */
        value = skipWhitespace(saveptr);
        if (value == NULL) {
            last_error = PARSER_MALFORMED_REQUEST;
            return -1;
        }

        /* Remove trailing whitespace and CRLF */
        end = value + strlen(value) - 1;
        while (end > value && (isspace((unsigned char)*end) || *end == '\r' || *end == '\n')) {
            *end = '\0';
            end--;
        }

        /* Store header */
        strncpy(request->headers[request->num_headers].name, name,
                MAX_HEADER_NAME - 1);
        request->headers[request->num_headers].name[MAX_HEADER_NAME - 1] = '\0';

        strncpy(request->headers[request->num_headers].value, value,
                MAX_HEADER_VALUE - 1);
        request->headers[request->num_headers].value[MAX_HEADER_VALUE - 1] = '\0';

        request->num_headers++;
    }

    return 0;
}

static int
parseBody(struct tcp_connection *conn, struct http_request *request)
{
    size_t content_length = 0;
    size_t i;
    ssize_t received;

    /* Look for Content-Length header */
    for (i = 0; i < request->num_headers; i++) {
        if (strcasecmp(request->headers[i].name, "Content-Length") == 0) {
            content_length = (size_t)atol(request->headers[i].value);
            break;
        }
    }

    if (content_length == 0) {
        return 0;  /* No body */
    }

    if (content_length > CACHE_MAX_VALUE_SIZE) {
        last_error = PARSER_BODY_TOO_LARGE;
        return -1;
    }

    request->body = malloc(content_length + 1);
    if (request->body == NULL) {
        last_error = PARSER_ERROR;
        return -1;
    }

    received = tcpReceive(conn, request->body, content_length);
    if (received < 0 || (size_t)received != content_length) {
        free(request->body);
        request->body = NULL;
        last_error = PARSER_MALFORMED_REQUEST;
        return -1;
    }

    request->body[content_length] = '\0';
    request->body_length = content_length;

    return 0;
}

static void
cleanupRequest(struct http_request *request)
{
    if (request->body != NULL) {
        free(request->body);
        request->body = NULL;
    }
    request->body_length = 0;
    request->num_headers = 0;
}

static char *
skipWhitespace(char *str)
{
    if (str == NULL) {
        return NULL;
    }

    while (isspace((unsigned char)*str)) {
        str++;
    }

    return *str ? str : NULL;
}

static int
readLine(struct tcp_connection *conn, char *buffer, size_t size)
{
    size_t i = 0;
    ssize_t received;
    char c;

    if (conn == NULL || buffer == NULL || size == 0) {
        return -1;
    }

    while (i < size - 1) {
        received = tcpReceive(conn, &c, 1);
        if (received < 0) {
            return -1;
        }
        if (received == 0) {
            break;
        }

        buffer[i++] = c;
        if (c == '\n') {
            break;
        }
    }

    buffer[i] = '\0';
    return (int)i;
}
