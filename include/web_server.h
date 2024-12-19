/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* filepath: src/web_server.h */
#ifndef WEB_SERVER_H
#define WEB_SERVER_H

/* System headers */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

/* Constants */
#define MAX_METHOD_LEN 16        /* HTTP method length */
#define MAX_URI_LEN 1024         /* URI length */
#define MAX_PATH_LEN 1024        /* File path length */
#define MAX_BUFFER_SIZE 4096     /* General buffer size */
#define DEFAULT_PORT 8080
#define DEFAULT_ROOT "www"
#define WWW_ROOT DEFAULT_ROOT
#define BACKLOG 10
#define SERVER_STRING "Server: EnvEngWebServer/1.0\r\n"

/* Function prototypes */
int setup_server(int port);
ssize_t serve_file(int client_socket, const char *uri);
int handle_client(int client_socket, const char *root_dir);
void send_error(int client_socket, int status_code, const char *message);

#endif /* WEB_SERVER_H */
