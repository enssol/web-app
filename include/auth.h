/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#ifndef AUTH_H
#define AUTH_H

#include <stdio.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256

int authenticateUser(const char *username, const char *password);

#endif /* AUTH_H */
