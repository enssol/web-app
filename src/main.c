/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

/* filepath: /devcontainer/web-app/src/main.c */

#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 500

#include "../include/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>

/* Global state */
static volatile sig_atomic_t keep_running = 1;
extern struct server_context ssl_ctx;

/* Signal handler prototype */
static void handle_signal(int sig);

int
main(void)
{
    int server_fd;
    int port;
    struct sigaction sa;
    int ret = EXIT_SUCCESS;
    struct server_context ctx;

    /* Setup signal handler */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1 ||
        sigaction(SIGTERM, &sa, NULL) == -1) {
        fprintf(stderr, "Failed to set signal handlers: %s\n",
                strerror(errno));
        return EXIT_FAILURE;
    }

    /* Initialize SSL context */
    if (init_ssl_ctx(&ctx) != 0) {
        return EXIT_FAILURE;
    }

    /* Load certificates */
    if (load_certificates(&ctx) != 0) {
        return EXIT_FAILURE;
    }

    /* Initialize server (includes SSL initialization) */
    server_fd = server_init();
    if (server_fd < 0) {
        fprintf(stderr, "Failed to initialize server\n");
        return EXIT_FAILURE;
    }

    port = server_get_port(server_fd);
    if (port < 0) {
        fprintf(stderr, "Failed to get server port\n");
        server_cleanup(server_fd);
        return EXIT_FAILURE;
    }

    printf("Server running on port %d\n", port);

    /* Main server loop */
    while (keep_running) {
        if (server_run(server_fd) < 0) {  /* Now properly declared */
            syslog(LOG_ERR, "Server run failed");
            ret = EXIT_FAILURE;
            break;
        }
    }

    printf("\nShutting down server on port %d\n", port);
    server_cleanup(server_fd);
    return ret;
}

static void
handle_signal(int sig)
{
    if (sig == SIGINT || sig == SIGTERM) {
        keep_running = 0;
    }
}
