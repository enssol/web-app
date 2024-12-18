/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* filepath: include/server.h */
#ifndef SERVER_H
#define SERVER_H

#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 500

#include <sys/types.h>

#ifdef TEST_BUILD
#define SERVER_PORT 8888
#else
#define SERVER_PORT 80
#endif

#define MAX_CONNECTIONS 10
#define BUFFER_SIZE 1024

/* Function prototypes */
int server_init(void);
int server_run(int server_fd);
void server_cleanup(int server_fd);

#endif /* SERVER_H */
