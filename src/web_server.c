/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* filepath: web_server.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include "../include/web_server.h"

/* Function prototypes */
static int setup_socket(int port);
static int construct_file_path(char *dest, size_t dest_size, const char *uri);
void send_error(int client_socket, int status_code, const char *message);


static int
construct_file_path(char *dest, size_t dest_size, const char *uri)
{
    if (dest == NULL || uri == NULL) {
        return -1;
    }

    /* Remove leading '/' if present */
    if (uri[0] == '/') {
        uri++;
    }

    /* Use default file if empty URI */
    if (uri[0] == '\0') {
        uri = "index.html";
    }

    /* Basic path sanitization */
    if (strstr(uri, "..") != NULL) {
        return -1;
    }

    /* Construct path */
    if (snprintf(dest, dest_size, "%s/%s", WWW_ROOT, uri) >= (int)dest_size) {
        return -1;
    }

    return 0;
}

ssize_t
serve_file(int client_socket, const char *uri)
{
    char filepath[MAX_PATH_LEN];
    char buffer[MAX_BUFFER_SIZE];
    FILE *file;
    struct stat st;
    size_t bytes_read;
    ssize_t total_bytes = 0;
    ssize_t write_ret;

    /* Add uri validation */
    if (uri == NULL) {
        send_error(client_socket, 400, "Bad Request");
        return -1;
    }

    /* Construct file path */
    if (construct_file_path(filepath, sizeof(filepath), uri) < 0) {
        send_error(client_socket, 400, "Bad Request");
        return -1;
    }

    /* Check if file exists and is readable */
    if (stat(filepath, &st) < 0 || !S_ISREG(st.st_mode)) {
        send_error(client_socket, 404, "Not Found");
        return -1;
    }

    file = fopen(filepath, "rb");
    if (file == NULL) {
        send_error(client_socket, 500, "Internal Server Error");
        return -1;
    }

    /* Send HTTP headers */
    dprintf(client_socket,
            "HTTP/1.0 200 OK\r\n"
            "%s"
            "Content-Type: text/html\r\n"  /* Changed from text/plain */
            "Content-Length: %ld\r\n"
            "\r\n",
            SERVER_STRING,
            (long)st.st_size);

    /* Read and send file content */
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        write_ret = write(client_socket, buffer, bytes_read);
        if (write_ret < 0) {
            fclose(file);
            return -1;
        }
        total_bytes += write_ret;
    }

    fclose(file);
    return total_bytes;
}

static int
setup_socket(int port)
{
    struct sockaddr_in server_addr;
    int sock_fd;
    int optval = 1;

    /* Validate port number */
    if (port <= 0 || port > UINT16_MAX) {
        return -1;
    }

    /* Create socket */
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        return -1;
    }

    /* Set socket options */
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR,
                   &optval, sizeof(optval)) < 0) {
        close(sock_fd);
        return -1;
    }

    /* Setup server address structure */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((uint16_t)port);

    /* Bind socket */
    if (bind(sock_fd, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0) {
        close(sock_fd);
        return -1;
    }

    return sock_fd;
}

void
send_error(int client_socket, int status_code, const char *message)
{
    dprintf(client_socket, "HTTP/1.0 %d %s\r\n%s"
            "Content-Type: text/plain\r\n\r\n%s\n",
            status_code, message, SERVER_STRING, message);
}

int
setup_server(int port)
{
    int sock_fd;

    /* Initialize socket */
    sock_fd = setup_socket(port);
    if (sock_fd < 0) {
        return -1;
    }

    /* Start listening */
    if (listen(sock_fd, SOMAXCONN) < 0) {
        close(sock_fd);
        return -1;
    }

    /* Store socket descriptor in global variable */
    return sock_fd;
}

int
handle_client(int client_socket, const char *root_dir)
{
    char method[MAX_METHOD_LEN];
    char uri[MAX_URI_LEN];
    char line[MAX_BUFFER_SIZE];
    ssize_t ret;
    FILE *client_stream;

    (void)root_dir; /* Explicitly mark parameter as unused */

    client_stream = fdopen(client_socket, "r+");
    if (client_stream == NULL) {
        return -1;
    }

    /* Read first line of HTTP request */
    if (fgets(line, sizeof(line), client_stream) == NULL) {
        fclose(client_stream);
        return -1;
    }

    /* Parse HTTP request line */
    if (sscanf(line, "%15s %1023s", method, uri) != 2) {
        send_error(client_socket, 400, "Bad Request");
        fclose(client_stream);
        return -1;
    }

    /* Only support GET method */
    if (strcmp(method, "GET") != 0) {
        send_error(client_socket, 405, "Method Not Allowed");
        fclose(client_stream);
        return -1;
    }

    ret = serve_file(client_socket, uri);
    fclose(client_stream);
    return (ret < 0) ? -1 : 0;
}
