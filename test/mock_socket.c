/* filepath: test/mock_socket.c */
/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include "mock_socket.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

/* Mock state */
#define MOCK_BUF_SIZE 4096
#define MOCK_BASE_FD 1000

static struct {
    int initialized;
    int last_fd;
    char send_buffer[MOCK_BUF_SIZE];
    char recv_buffer[MOCK_BUF_SIZE];
    size_t send_len;
    size_t recv_len;
    size_t recv_pos;
} mock_state = {0};

/* Reset mock state */
void
mock_socket_reset(void)
{
    mock_state.initialized = 1;
    mock_state.last_fd = MOCK_BASE_FD;
    memset(mock_state.send_buffer, 0, MOCK_BUF_SIZE);
    memset(mock_state.recv_buffer, 0, MOCK_BUF_SIZE);
    mock_state.send_len = 0;
    mock_state.recv_len = 0;
    mock_state.recv_pos = 0;
}

/* Mock implementations */
int
mock_socket(int domain __attribute__((unused)),
           int type __attribute__((unused)),
           int protocol __attribute__((unused)))
{
    if (!mock_state.initialized) {
        errno = EINVAL;
        return -1;
    }
    return mock_state.last_fd++;
}

int
mock_bind(int sockfd,
         const struct sockaddr *addr __attribute__((unused)),
         socklen_t addrlen __attribute__((unused)))
{
    if (!mock_state.initialized || sockfd < MOCK_BASE_FD) {
        errno = EBADF;
        return -1;
    }
    return 0;
}

int
mock_listen(int sockfd,
           int backlog __attribute__((unused)))
{
    if (!mock_state.initialized || sockfd < MOCK_BASE_FD) {
        errno = EBADF;
        return -1;
    }
    return 0;
}

int
mock_accept(int sockfd,
           struct sockaddr *addr __attribute__((unused)),
           socklen_t *addrlen __attribute__((unused)))
{
    if (!mock_state.initialized || sockfd < MOCK_BASE_FD) {
        errno = EBADF;
        return -1;
    }
    return mock_state.last_fd++;
}

ssize_t
mock_send(int sockfd,
         const void *buf,
         size_t len,
         int flags __attribute__((unused)))
{
    if (!mock_state.initialized || sockfd < MOCK_BASE_FD || buf == NULL) {
        errno = EBADF;
        return -1;
    }

    if (len == 0) {
        return 0;
    }

    if (mock_state.send_len + len > MOCK_BUF_SIZE) {
        len = MOCK_BUF_SIZE - mock_state.send_len;
    }

    memcpy(mock_state.send_buffer + mock_state.send_len, buf, len);
    mock_state.send_len += len;

    return (ssize_t)len;
}

ssize_t
mock_recv(int sockfd,
         void *buf,
         size_t len,
         int flags __attribute__((unused)))
{
    size_t remaining;
    size_t copy_len;

    if (!mock_state.initialized || sockfd < MOCK_BASE_FD || buf == NULL) {
        errno = EBADF;
        return -1;
    }

    if (len == 0) {
        return 0;
    }

    remaining = mock_state.recv_len - mock_state.recv_pos;
    if (remaining == 0) {
        return 0; /* EOF */
    }

    copy_len = len < remaining ? len : remaining;
    memcpy(buf, mock_state.recv_buffer + mock_state.recv_pos, copy_len);
    mock_state.recv_pos += copy_len;

    return (ssize_t)copy_len;
}

int
mock_close(int fd)
{
    if (!mock_state.initialized || fd < MOCK_BASE_FD) {
        errno = EBADF;
        return -1;
    }
    return 0;
}

/* Test helper implementations */
int
mock_socket_get_last_fd(void)
{
    return mock_state.last_fd;
}

const char *
mock_socket_get_sent_data(void)
{
    return mock_state.send_buffer;
}

size_t
mock_socket_get_sent_length(void)
{
    return mock_state.send_len;
}

void
mock_socket_set_recv_data(const char *data, size_t len)
{
    if (len > MOCK_BUF_SIZE) {
        len = MOCK_BUF_SIZE;
    }
    memcpy(mock_state.recv_buffer, data, len);
    mock_state.recv_len = len;
    mock_state.recv_pos = 0;
}
