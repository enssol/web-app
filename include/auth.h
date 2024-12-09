/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#ifndef AUTH_H
#define AUTH_H

#include <sys/socket.h>

/* Function declarations */
void handleLogin(int client_fd);
int authenticateUser(const char *username, const char *password);

#endif /* AUTH_H */
