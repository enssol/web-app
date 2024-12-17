/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* include/tcp.h */
#ifndef TCP_H
#define TCP_H

#include "net.h"

/* Constants */
#define TCP_BUFFER_SIZE 4096
#define TCP_TIMEOUT_SEC 30

/* TCP connection states */
enum tcp_state {
    TCP_STATE_CLOSED,
    TCP_STATE_LISTEN,
    TCP_STATE_CONNECTING,
    TCP_STATE_CONNECTED,
    TCP_STATE_ERROR
};

/* Forward declaration of tcp_operations */
struct tcp_operations;

/* TCP connection info */
struct tcp_connection {
    int sockfd;
    enum tcp_state state;
    struct sockaddr_in addr;
    const struct tcp_operations *ops;
    void *user_data;
};

/* TCP operations interface */
struct tcp_operations {
    ssize_t (*receive)(struct tcp_connection *conn, void *buffer, size_t size);
    ssize_t (*send)(struct tcp_connection *conn, const void *data, size_t size);
};

/* Function prototypes */
int tcpInit(void);
int tcpConnect(struct tcp_connection *conn, const char *host, int port);
ssize_t tcpSend(struct tcp_connection *conn, const void *data, size_t size);
ssize_t tcpReceive(struct tcp_connection *conn, void *buffer, size_t size);
void tcpClose(struct tcp_connection *conn);
void tcpCleanup(void);  /* Added missing prototype */

#endif /* TCP_H */
