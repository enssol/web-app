/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */
/* src/http_response.c */

#include "../include/http_response.h"
#include "../include/app_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* Static variables */
static int is_initialized = 0;
static enum response_error last_error = RESPONSE_OK;

/* Status code to reason phrase mapping */
static const struct {
    enum http_status status;
    const char *reason;
} status_reasons[] = {
    {HTTP_STATUS_OK, "OK"},
    {HTTP_STATUS_CREATED, "Created"},
    {HTTP_STATUS_ACCEPTED, "Accepted"},
    {HTTP_STATUS_NO_CONTENT, "No Content"},
    {HTTP_STATUS_MOVED_PERMANENTLY, "Moved Permanently"},
    {HTTP_STATUS_FOUND, "Found"},
    {HTTP_STATUS_NOT_MODIFIED, "Not Modified"},
    {HTTP_STATUS_BAD_REQUEST, "Bad Request"},
    {HTTP_STATUS_UNAUTHORIZED, "Unauthorized"},
    {HTTP_STATUS_FORBIDDEN, "Forbidden"},
    {HTTP_STATUS_NOT_FOUND, "Not Found"},
    {HTTP_STATUS_METHOD_NOT_ALLOWED, "Method Not Allowed"},
    {HTTP_STATUS_REQUEST_TIMEOUT, "Request Timeout"},
    {HTTP_STATUS_LENGTH_REQUIRED, "Length Required"},
    {HTTP_STATUS_PAYLOAD_TOO_LARGE, "Payload Too Large"},
    {HTTP_STATUS_URI_TOO_LONG, "URI Too Long"},
    {HTTP_STATUS_INTERNAL_ERROR, "Internal Server Error"},
    {HTTP_STATUS_NOT_IMPLEMENTED, "Not Implemented"},
    {HTTP_STATUS_BAD_GATEWAY, "Bad Gateway"},
    {HTTP_STATUS_SERVICE_UNAVAILABLE, "Service Unavailable"}
};

/* Internal function prototypes */
static int formatStatusLine(char *buffer, size_t size,
                          const struct http_response *response);
static const char *getReasonPhrase(enum http_status status);
static int sendHeaders(struct tcp_connection *conn,
                      const struct http_response *response);

int
httpResponseInit(void)
{
    if (is_initialized) {
        return 0;
    }
    is_initialized = 1;
    last_error = RESPONSE_OK;
    return 0;
}

int
httpResponseCreate(struct http_response *response, enum http_status status)
{
    const char *reason;

    if (!is_initialized || response == NULL) {
        last_error = RESPONSE_INVALID_PARAM;
        return -1;
    }

    memset(response, 0, sizeof(struct http_response));
    response->status = status;

    reason = getReasonPhrase(status);
    if (reason == NULL) {
        last_error = RESPONSE_INVALID_STATUS;
        return -1;
    }

    strncpy(response->reason, reason, MAX_REASON_PHRASE - 1);
    response->reason[MAX_REASON_PHRASE - 1] = '\0';

    return 0;
}

int
httpResponseAddHeader(struct http_response *response,
                     const char *name,
                     const char *value)
{
    if (response == NULL || name == NULL || value == NULL) {
        last_error = RESPONSE_INVALID_PARAM;
        return -1;
    }

    if (response->num_headers >= MAX_RESPONSE_HEADERS) {
        last_error = RESPONSE_TOO_MANY_HEADERS;
        return -1;
    }

    if (strlen(name) >= MAX_HEADER_NAME || strlen(value) >= MAX_HEADER_VALUE) {
        last_error = RESPONSE_HEADER_TOO_LARGE;
        return -1;
    }

    strncpy(response->headers[response->num_headers].name, name,
            MAX_HEADER_NAME - 1);
    response->headers[response->num_headers].name[MAX_HEADER_NAME - 1] = '\0';

    strncpy(response->headers[response->num_headers].value, value,
            MAX_HEADER_VALUE - 1);
    response->headers[response->num_headers].value[MAX_HEADER_VALUE - 1] = '\0';

    response->num_headers++;
    return 0;
}

int
httpResponseSetBody(struct http_response *response,
                   const void *body,
                   size_t length)
{
    if (response == NULL || (body == NULL && length > 0)) {
        last_error = RESPONSE_INVALID_PARAM;
        return -1;
    }

    if (length > CACHE_MAX_VALUE_SIZE) {
        last_error = RESPONSE_BODY_TOO_LARGE;
        return -1;
    }

    response->body = (void *)body;
    response->body_length = length;

    return 0;
}

int
httpResponseSend(struct tcp_connection *conn,
                const struct http_response *response)
{
    char buffer[MAX_STATUS_LINE];
    ssize_t sent;
    int status;

    if (conn == NULL || response == NULL) {
        last_error = RESPONSE_INVALID_PARAM;
        return -1;
    }

    /* Format and send status line */
    status = formatStatusLine(buffer, sizeof(buffer), response);
    if (status != 0) {
        return -1;
    }

    sent = tcpSend(conn, buffer, strlen(buffer));
    if (sent < 0) {
        last_error = RESPONSE_SEND_ERROR;
        return -1;
    }

    /* Send headers */
    status = sendHeaders(conn, response);
    if (status != 0) {
        return -1;
    }

    /* Send empty line after headers */
    sent = tcpSend(conn, "\r\n", 2);
    if (sent < 0) {
        last_error = RESPONSE_SEND_ERROR;
        return -1;
    }

    /* Send body if present */
    if (response->body != NULL && response->body_length > 0) {
        sent = tcpSend(conn, response->body, response->body_length);
        if (sent < 0 || (size_t)sent != response->body_length) {
            last_error = RESPONSE_SEND_ERROR;
            return -1;
        }
    }

    return 0;
}

const char *
httpStatusToString(enum http_status status)
{
    size_t i;
    size_t num_statuses;

    num_statuses = sizeof(status_reasons) / sizeof(status_reasons[0]);
    for (i = 0; i < num_statuses; i++) {
        if (status_reasons[i].status == status) {
            return status_reasons[i].reason;
        }
    }

    return NULL;
}

void
httpResponseCleanup(struct http_response *response)
{
    if (response != NULL) {
        response->body = NULL;
        response->body_length = 0;
        response->num_headers = 0;
    }
}

const char *
httpResponseGetError(enum response_error error)
{
    switch (error) {
        case RESPONSE_OK:
            return "Success";
        case RESPONSE_INVALID_STATUS:
            return "Invalid HTTP status code";
        case RESPONSE_HEADER_TOO_LARGE:
            return "Header too large";
        case RESPONSE_TOO_MANY_HEADERS:
            return "Too many headers";
        case RESPONSE_BODY_TOO_LARGE:
            return "Response body too large";
        case RESPONSE_SEND_ERROR:
            return "Error sending response";
        case RESPONSE_INVALID_PARAM:
            return "Invalid parameter";
        default:
            return "Unknown error";
    }
}

/* Internal function implementations */
static int
formatStatusLine(char *buffer, size_t size, const struct http_response *response)
{
    int written;

    written = snprintf(buffer, size, "%s %d %s\r\n",
                      DEFAULT_HTTP_VERSION,
                      response->status,
                      response->reason);

    if (written < 0 || (size_t)written >= size) {
        last_error = RESPONSE_INVALID_STATUS;
        return -1;
    }

    return 0;
}

static const char *
getReasonPhrase(enum http_status status)
{
    size_t i;
    size_t num_statuses;

    num_statuses = sizeof(status_reasons) / sizeof(status_reasons[0]);
    for (i = 0; i < num_statuses; i++) {
        if (status_reasons[i].status == status) {
            return status_reasons[i].reason;
        }
    }

    return NULL;
}

static int
sendHeaders(struct tcp_connection *conn, const struct http_response *response)
{
    char buffer[MAX_HEADER_NAME + MAX_HEADER_VALUE + 4];
    size_t i;
    ssize_t sent;
    int written;

    for (i = 0; i < response->num_headers; i++) {
        written = snprintf(buffer, sizeof(buffer), "%s: %s\r\n",
                         response->headers[i].name,
                         response->headers[i].value);

        if (written < 0 || (size_t)written >= sizeof(buffer)) {
            last_error = RESPONSE_HEADER_TOO_LARGE;
            return -1;
        }

        sent = tcpSend(conn, buffer, (size_t)written);
        if (sent < 0) {
            last_error = RESPONSE_SEND_ERROR;
            return -1;
        }
    }

    return 0;
}
