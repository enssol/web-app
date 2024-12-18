/* filepath: /devcontainer/web-app/src/main.c */
/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include "../include/server.h"

#define DEFAULT_WWW_ROOT WWW_DIR

static volatile sig_atomic_t keep_running = 1;

static void
signal_handler(int signo)
{
    if (signo == SIGINT || signo == SIGTERM) {
        keep_running = 0;
    }
}

static void
print_usage(const char *program_name)
{
    fprintf(stderr, "Usage: %s [-p port] [-s ssl_port] [-d www_root] [-c cert] "
            "[-k key] [-l access_log] [-e error_log] [-h domain] [-w www_domain]\n",
            program_name);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -p port        HTTP port (default: %d)\n", DEFAULT_PORT);
    fprintf(stderr, "  -s ssl_port    HTTPS port (default: %d)\n", DEFAULT_SSL_PORT);
    fprintf(stderr, "  -d www_root    Web root directory (default: %s)\n", WWW_DIR);
    fprintf(stderr, "  -c cert        SSL certificate file (default: %s)\n", SSL_CERT_FILE);
    fprintf(stderr, "  -k key         SSL private key file (default: %s)\n", SSL_KEY_FILE);
    fprintf(stderr, "  -l access_log  Access log file (default: %s)\n", ACCESS_LOG);
    fprintf(stderr, "  -e error_log   Error log file (default: %s)\n", ERROR_LOG);
    fprintf(stderr, "  -h domain      Primary domain (default: %s)\n", DEFAULT_DOMAIN);
    fprintf(stderr, "  -w www_domain  WWW domain (default: %s)\n", DEFAULT_WWW_DOMAIN);
}

int
main(int argc, char *argv[])
{
    struct server_config config;
    struct sigaction sa;
    int opt;
    int ret = EXIT_SUCCESS;

    /* Set default configuration */
    config.port = DEFAULT_PORT;
    config.ssl_port = DEFAULT_SSL_PORT;
    config.doc_root = WWW_DIR;
    config.cert_file = SSL_CERT_FILE;
    config.key_file = SSL_KEY_FILE;
    config.access_log = ACCESS_LOG;
    config.error_log = ERROR_LOG;
    config.domain = DEFAULT_DOMAIN;
    config.www_domain = DEFAULT_WWW_DOMAIN;
    config.use_ssl = 1;

    /* Rest of the main function remains the same */
    while ((opt = getopt(argc, argv, "p:s:d:c:k:l:e:h:w:")) != -1) {
        switch (opt) {
        case 'p':
            config.port = atoi(optarg);
            if (config.port <= 0 || config.port > 65535) {
                fprintf(stderr, "Invalid port number\n");
                return EXIT_FAILURE;
            }
            break;
        case 's':
            config.ssl_port = atoi(optarg);
            if (config.ssl_port <= 0 || config.ssl_port > 65535) {
                fprintf(stderr, "Invalid SSL port number\n");
                return EXIT_FAILURE;
            }
            break;
        case 'd':
            config.doc_root = optarg;
            break;
        case 'c':
            config.cert_file = optarg;
            break;
        case 'k':
            config.key_file = optarg;
            break;
        case 'l':
            config.access_log = optarg;
            break;
        case 'e':
            config.error_log = optarg;
            break;
        case 'h':
            config.domain = optarg;
            break;
        case 'w':
            config.www_domain = optarg;
            break;
        default:
            print_usage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    /* Set up signal handling */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    if (sigaction(SIGINT, &sa, NULL) == -1 ||
        sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("Failed to set up signal handlers");
        return EXIT_FAILURE;
    }

    /* Initialize server */
    if (server_init(&config) != 0) {
        fprintf(stderr, "Failed to initialize server: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    printf("Server starting on port %d (HTTP) and %d (HTTPS)\n",
           config.port, config.ssl_port);
    printf("Serving files from: %s\n", config.doc_root);
    printf("Primary domain: %s\n", config.domain);
    printf("WWW domain: %s\n", config.www_domain);

    /* Main server loop */
    while (keep_running) {
        ret = server_process();
        if (ret != 0 && keep_running) {
            fprintf(stderr, "Server error: %s\n", strerror(errno));
            ret = EXIT_FAILURE;
            break;
        }
    }

    /* Cleanup */
    printf("\nShutting down server...\n");
    server_cleanup();

    return ret;
}
