/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* src/tcp.c */
#include "../include/tcp.h"
#include "../include/app_error.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <limits.h>
#include <arpa/inet.h>

/* Initialize TCP subsystem */
int
tcpInit(void)
{
    return netInit();
}

/* Connect to remote host */
int
tcpConnect(struct tcp_connection *conn, const char *host, int port)
{
    struct socket_info sock;
    uint16_t net_port;

    if (conn == NULL || host == NULL || port <= 0 || port > UINT16_MAX) {
        errno = EINVAL;
        return -1;
    }

    /* Convert port to network byte order safely */
    net_port = (uint16_t)port;

    /* Create socket */
    if (netCreateSocket(&sock, SOCKET_TCP) != 0) {
        return -1;
    }

    /* Set up address structure */
    memset(&conn->addr, 0, sizeof(conn->addr));
    conn->addr.sin_family = AF_INET;
    conn->addr.sin_port = htons(net_port);
    if (inet_pton(AF_INET, host, &conn->addr.sin_addr) != 1) {
        netClose(&sock);
        return -1;
    }

    /* Connect */
    if (connect(sock.fd, (struct sockaddr *)&conn->addr,
                sizeof(conn->addr)) == -1) {
        if (errno != EINPROGRESS) {
            netClose(&sock);
            return -1;
        }
    }

    /* Store connection info */
    conn->sockfd = sock.fd;
    conn->state = TCP_STATE_CONNECTED;
    conn->user_data = NULL;
    conn->ops = NULL;

    return 0;
}

/* Send data over TCP connection */
ssize_t
tcpSend(struct tcp_connection *conn, const void *data, size_t size)
{
    const unsigned char *ptr;
    size_t remaining;
    ssize_t sent;
    size_t total_sent;

    if (conn == NULL || data == NULL || size == 0) {
        errno = EINVAL;
        return -1;
    }

    /* Use operations if defined */
    if (conn->ops != NULL && conn->ops->send != NULL) {
        return conn->ops->send(conn, data, size);
    }

    ptr = (const unsigned char *)data;
    remaining = size;
    total_sent = 0;

    while (remaining > 0) {
        sent = send(conn->sockfd, ptr, remaining, 0);
        if (sent == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                continue;
            }
            return -1;
        }

        if (sent > 0) {
            ptr += sent;
            if ((size_t)sent <= remaining) {
                remaining -= (size_t)sent;
                total_sent += (size_t)sent;
            }
        }
    }

    return (ssize_t)total_sent;
}

/* Receive data from TCP connection */
ssize_t
tcpReceive(struct tcp_connection *conn, void *buffer, size_t size)
{
    ssize_t received;

    if (conn == NULL || buffer == NULL || size == 0) {
        errno = EINVAL;
        return -1;
    }

    /* Use operations if defined */
    if (conn->ops != NULL && conn->ops->receive != NULL) {
        return conn->ops->receive(conn, buffer, size);
    }

    received = recv(conn->sockfd, buffer, size, 0);
    if (received == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return 0;
        }
        return -1;
    }

    if (received == 0) {
        conn->state = TCP_STATE_CLOSED;
    }

    return received;
}

/* Close TCP connection */
void
tcpClose(struct tcp_connection *conn)
{
    if (conn == NULL) {
        return;
    }

    if (conn->sockfd >= 0) {
        close(conn->sockfd);
        conn->sockfd = -1;
    }
    conn->state = TCP_STATE_CLOSED;
    conn->ops = NULL;
    conn->user_data = NULL;
}

/* Clean up TCP subsystem */
void
tcpCleanup(void)
{
    netCleanup();
}
