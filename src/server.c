/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include "../include/server.h"
#include "../include/hosts.h"
#include <ctype.h>
#include <strings.h> /* For strncasecmp */

#define BACKLOG 10
#define BUF_SIZE 8192
#define RECV_BUFFER_SIZE 8192

static int server_fd = -1;
static char doc_root[MAX_PATH];
static char access_log_path[MAX_PATH];
static char error_log_path[MAX_PATH];
static time_t last_request_time = 0;
static int request_count = 0;

static void
log_access(const char *client_ip, const char *method, const char *path, int status)
{
    FILE *log_file;
    time_t now;
    char time_str[32];

    now = time(NULL);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));

    log_file = fopen(access_log_path, "a");
    if (log_file != NULL) {
        fprintf(log_file, "[%s] %s - %s %s %d\n",
                time_str, client_ip, method, path, status);
        fclose(log_file);
    }
}

void
log_error(const char *message)
{
    FILE *log_file;
    time_t now;
    char time_str[26];

    time(&now);
    ctime_r(&now, time_str);
    time_str[24] = '\0';

    log_file = fopen(error_log_path, "a");
    if (log_file != NULL) {
        fprintf(log_file, "[%s] ERROR: %s\n", time_str, message);
        fclose(log_file);
    }
}

int
server_init(const struct server_config *config)
{
    struct sockaddr_in addr;
    int opt;
    uint16_t port;

    /* Validate port range */
    if (config->port <= 0 || config->port > 65535) {
        log_error("Invalid port number");
        return -1;
    }

    port = (uint16_t)config->port;

    /* Save paths */
    strncpy(doc_root, config->doc_root, sizeof(doc_root) - 1);
    strncpy(access_log_path, config->access_log, sizeof(access_log_path) - 1);
    strncpy(error_log_path, config->error_log, sizeof(error_log_path) - 1);

    /* Create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        log_error("Failed to create socket");
        return -1;
    }

    /* Set socket options */
    opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        log_error("Failed to set socket options");
        close(server_fd);
        return -1;
    }

    /* Bind to port */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        log_error("Failed to bind to port");
        close(server_fd);
        return -1;
    }

    /* Listen for connections */
    if (listen(server_fd, BACKLOG) < 0) {
        log_error("Failed to listen on socket");
        close(server_fd);
        return -1;
    }

    return 0;
}

static int validate_path(const char *path)
{
    if (path == NULL) {
        return 0;
    }

    /* Prevent directory traversal */
    if (strstr(path, "..") != NULL) {
        return 0;
    }

    /* Check for valid characters */
    while (*path) {
        if (!isprint((unsigned char)*path)) {
            return 0;
        }
        path++;
    }

    return 1;
}

int
handle_static_file(const char *path, struct http_response *resp)
{
    char full_path[MAX_PATH];
    struct stat st;
    FILE *fp;
    size_t bytes_read;
    time_t now;
    char date_buf[32];
    int ret;

    if (path == NULL || resp == NULL) {
        return -1;
    }

    /* Initialize response */
    memset(resp, 0, sizeof(*resp));
    resp->status_code = HTTP_OK;
    resp->content_type = "text/html";

    /* Handle root path */
    if (strcmp(path, "/") == 0) {
        path = "/index.html";
    }

    /* Build full path */
    ret = snprintf(full_path, sizeof(full_path), "%s%s", doc_root, path);
    if (ret < 0 || (size_t)ret >= sizeof(full_path)) {
        resp->status_code = HTTP_INTERNAL_ERROR;
        return -1;
    }

    /* Check file exists and readable */
    if (stat(full_path, &st) != 0 || !S_ISREG(st.st_mode)) {
        resp->status_code = HTTP_NOT_FOUND;
        return -1;
    }

    /* Open and read file */
    fp = fopen(full_path, "r");
    if (fp == NULL) {
        resp->status_code = HTTP_NOT_FOUND;
        return -1;
    }

    bytes_read = fread(resp->content, 1, sizeof(resp->content) - 1, fp);
    fclose(fp);

    resp->content[bytes_read] = '\0';
    resp->content_length = bytes_read;

    /* Get current time for headers */
    now = time(NULL);
    strftime(date_buf, sizeof(date_buf), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&now));

    /* Format response headers */
    ret = snprintf(resp->headers, sizeof(resp->headers),
            "HTTP/1.1 %d OK\r\n"
            "Content-Length: %zu\r\n"
            "Content-Type: %s\r\n"
            "Date: %s\r\n"
            "Connection: close\r\n"
            "\r\n",
            resp->status_code,
            resp->content_length,
            resp->content_type,
            date_buf);

    if (ret < 0 || (size_t)ret >= sizeof(resp->headers)) {
        resp->status_code = HTTP_INTERNAL_ERROR;
        return -1;
    }

    return 0;
}

static int check_rate_limit(void)
{
    time_t current_time = time(NULL);

    if (current_time - last_request_time >= 60) {
        last_request_time = current_time;
        request_count = 1;
        return 1;
    }

    if (request_count >= MAX_REQUESTS_PER_MINUTE) {
        return 0;
    }

    request_count++;
    return 1;
}

int
server_process(void)
{
    struct sockaddr_in client_addr;
    socklen_t client_len;
    int client_fd;
    char recv_buffer[RECV_BUFFER_SIZE];
    ssize_t bytes_received;
    char *line;
    char *method;
    char *path;
    char *protocol;
    char client_ip[INET_ADDRSTRLEN];
    struct http_response resp;
    char *saveptr;

    client_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

    if (client_fd < 0) {
        log_error("Failed to accept connection");
        return -1;
    }

    /* Get client IP */
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));

    /* Rate limiting */
    if (!check_rate_limit()) {
        dprintf(client_fd,
                "HTTP/1.1 429 Too Many Requests\r\n"
                "Content-Length: 20\r\n"
                "Content-Type: text/plain\r\n"
                "Connection: close\r\n"
                "\r\n"
                "Too many requests\r\n");
        close(client_fd);
        log_access(client_ip, "LIMIT", "/", 429);
        return 0;
    }

    /* Receive request */
    bytes_received = recv(client_fd, recv_buffer, sizeof(recv_buffer) - 1, 0);
    if (bytes_received <= 0) {
        close(client_fd);
        return 0;
    }
    recv_buffer[bytes_received] = '\0';

    /* Parse request line */
    line = strtok_r(recv_buffer, "\r\n", &saveptr);
    if (line == NULL) {
        close(client_fd);
        return 0;
    }

    method = strtok(line, " ");
    path = strtok(NULL, " ");
    protocol = strtok(NULL, " ");

    if (method == NULL || path == NULL || protocol == NULL) {
        dprintf(client_fd,
                "HTTP/1.1 400 Bad Request\r\n"
                "Content-Length: 11\r\n"
                "Content-Type: text/plain\r\n"
                "Connection: close\r\n"
                "\r\n"
                "Bad Request");
        close(client_fd);
        log_access(client_ip, "INVALID", "/", 400);
        return 0;
    }

    /* Validate path */
    if (!validate_path(path)) {
        dprintf(client_fd,
                "HTTP/1.1 403 Forbidden\r\n"
                "Content-Length: 9\r\n"
                "Content-Type: text/plain\r\n"
                "Connection: close\r\n"
                "\r\n"
                "Forbidden");
        close(client_fd);
        log_access(client_ip, method, path, 403);
        return 0;
    }

    /* Handle static file */
    memset(&resp, 0, sizeof(resp));
    if (handle_static_file(path, &resp) == 0) {
        dprintf(client_fd,
                "HTTP/1.1 %d OK\r\n"
                "Content-Length: %zu\r\n"
                "Content-Type: %s\r\n"
                "Connection: close\r\n"
                "\r\n"
                "%s",
                resp.status_code,
                resp.content_length,
                resp.content_type,
                resp.content);
        log_access(client_ip, method, path, resp.status_code);
    } else {
        dprintf(client_fd,
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Length: 9\r\n"
                "Content-Type: text/plain\r\n"
                "Connection: close\r\n"
                "\r\n"
                "Not Found");
        log_access(client_ip, method, path, 404);
    }

    close(client_fd);
    return 0;
}

void server_cleanup(void)
{
    if (server_fd >= 0) {
        close(server_fd);
        server_fd = -1;
    }
    hosts_cleanup();
}
