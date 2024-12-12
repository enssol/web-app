/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#ifndef SERVER_H
#define SERVER_H

struct ServerConfig
{
    int port;
    const char *ssl_cert;
    const char *ssl_key;
    const char *db_file;
    const char *env_file;
    int max_streams;     // Add for QUIC
    int quic_timeout;    // Add for QUIC
};

struct ServerState {
    SSL_CTX *ssl_ctx;
    struct QuicContext *quic_ctx;
};

extern struct ServerState server_state;

int initServer(const struct ServerConfig *config);
int runServer(void);

/**
 * Stop the server and cleanup resources including SSL
 */
void stopServer(void);

#endif /* SERVER_H */
