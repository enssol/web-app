/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>

#include "../include/server.h"
#include "../include/config.h"
#include "../include/logger.h"
#include "../include/http.h"
#include "../include/data_structures.h"
#include "../include/ssl_module.h"
#include "../include/quic_module.h"

#define MAX_CONNECTIONS 100
#define BUFFER_SIZE 1024

static volatile int server_running = 1;
static int server_socket = -1;
static SSL_CTX *ssl_context = NULL;

struct ServerState {
    SSL_CTX *ssl_ctx;
    struct QuicContext *quic_ctx; // Add QUIC context
};

static struct ServerState server_state = {0};

static void handle_signal(int signal)
{
    if (signal == SIGINT || signal == SIGTERM)
    {
        server_running = 0;
        if (server_socket != -1)
        {
            close(server_socket);
        }
    }
}

int initServer(const struct ServerConfig *config)
{
    SSL_CTX *ssl_ctx;
    struct sigaction sa;
    struct sockaddr_in server_addr;
    int optval = 1;

    /* Create SSL context */
    ssl_ctx = createSslContext(config->ssl_cert, config->ssl_key);
    if (!ssl_ctx) {
        logError("Failed to initialize SSL");
        return -1;
    }

    /* Initialize HTTP module with SSL context */
    httpInit(ssl_ctx);

    /* Initialize signal handling */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    /* Create socket */
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        logError("Failed to create socket");
        return -1;
    }

    /* Set socket options */
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    {
        logError("Failed to set socket options");
        close(server_socket);
        return -1;
    }

    /* Configure server address */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(config->port);

    /* Bind socket */
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        logError("Failed to bind socket");
        close(server_socket);
        return -1;
    }

    /* Listen for connections */
    if (listen(server_socket, MAX_CONNECTIONS) == -1)
    {
        logError("Failed to listen on socket");
        close(server_socket);
        return -1;
    }

    // Initialize QUIC context
    server_state.quic_ctx = initializeQuicContext(server_socket,
                                                ssl_ctx,
                                                config->max_streams,
                                                config->quic_timeout);
    if (!server_state.quic_ctx) {
        logError("Failed to initialize QUIC context");
        cleanupSslContext(ssl_ctx);
        return -1;
    }

    server_state.ssl_ctx = ssl_ctx;
    return 0;
}

/* server.c cleanup needs */
void stopServer(void) {
    if (server_state.quic_ctx) {
        cleanupQuicContext(server_state.quic_ctx);
    }
    if (server_state.ssl_ctx) {
        cleanupSslContext(server_state.ssl_ctx);
    }
    if (server_socket != -1) {
        close(server_socket);
    }
}

static int handleClientRequest(SSL *ssl)
{
    struct Request *request;
    struct Response *response;
    int result;

    request = dsCreateRequest();
    if (!request) {
        return -1;
    }

    /* Handle the request */
    result = httpHandleConnection(ssl, request);
    if (result != 0) {
        response = dsCreateResponse(HTTP_INTERNAL_ERROR);
        if (response) {
            dsAddResponseHeader(response, "Content-Type", "text/plain");
            httpSendResponse(ssl, response);
            dsFreeResponse(response);
        }
    }

    dsFreeRequest(request);
    return result;
}
