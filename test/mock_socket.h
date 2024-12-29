/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* filepath: test/mock_socket.h */
#ifndef MOCK_SOCKET_H
#define MOCK_SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>

/* Mock socket operations */
int mock_socket(int domain, int type, int protocol);
int mock_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int mock_listen(int sockfd, int backlog);
int mock_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
ssize_t mock_send(int sockfd, const void *buf, size_t len, int flags);
ssize_t mock_recv(int sockfd, void *buf, size_t len, int flags);
int mock_close(int fd);

/* Test helper functions */
void mock_socket_reset(void);
int mock_socket_get_last_fd(void);
const char *mock_socket_get_sent_data(void);
size_t mock_socket_get_sent_length(void);
void mock_socket_set_recv_data(const char *data, size_t len);

#endif /* MOCK_SOCKET_H */
