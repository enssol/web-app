/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

/* filepath: src/main.c */
#include "../include/server.h"
#include <stdio.h>
#include <signal.h>

static volatile int keep_running = 1;

static void
handle_signal(int sig)
{
    (void)sig;
    keep_running = 0;
}

int
main(void)
{
    int server_fd;
    struct sigaction sa;

    /* Setup signal handler */
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    server_fd = server_init();
    if (server_fd < 0) {
        fprintf(stderr, "Failed to initialize server\n");
        return 1;
    }

    printf("Server running on port %d\n", SERVER_PORT);

    while (keep_running) {
        server_run(server_fd);
    }

    server_cleanup(server_fd);
    return 0;
}
