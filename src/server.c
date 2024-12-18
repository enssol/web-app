/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* filepath: src/server.c */
#include "../include/server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

static const char *response_header =
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "\r\n";

int
server_init(void)
{
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t len = sizeof(opt);

    /* Create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        return -1;
    }

    /* Set socket options */
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                   &opt, len) < 0) {
        perror("setsockopt failed");
        close(server_fd);
        return -1;
    }

    /* Clear address structure */
    memset(&address, 0, sizeof(address));

    /* Setup address structure */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons((unsigned short)SERVER_PORT);

    /* Bind */
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        return -1;
    }

    /* Listen */
    if (listen(server_fd, MAX_CONNECTIONS) < 0) {
        perror("listen failed");
        close(server_fd);
        return -1;
    }

    return server_fd;
}

int
server_run(int server_fd)
{
    int client_fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int html_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len;

    client_len = sizeof(client_addr);

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
                      &client_len);
    if (client_fd < 0) {
        return -1;
    }

    /* Read request */
    bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
    if (bytes_read < 0) {
        close(client_fd);
        return -1;
    }
    buffer[bytes_read] = '\0';

    /* Send response header */
    write(client_fd, response_header, strlen(response_header));

    /* Send file content */
    html_fd = open("www/index.html", O_RDONLY);
    if (html_fd >= 0) {
        while ((bytes_read = read(html_fd, buffer, BUFFER_SIZE)) > 0) {
            write(client_fd, buffer, (size_t)bytes_read);
        }
        close(html_fd);
    }

    close(client_fd);
    return 0;
}

void
server_cleanup(int server_fd)
{
    if (server_fd >= 0) {
        close(server_fd);
    }
}
