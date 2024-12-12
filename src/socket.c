/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include "socket.h"
#include <netinet/in.h>
#include <unistd.h>

int createServerSocket(int port)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) return -1;

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0 ||
        listen(server_fd, 10) < 0) {
        close(server_fd);
        return -1;
    }

    return server_fd;
}
