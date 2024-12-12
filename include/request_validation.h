/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#ifndef REQUEST_VALIDATION_H
#define REQUEST_VALIDATION_H

#include <stdio.h>
#include <stdlib.h>

// Request validation functions
int validateHttpRequest(const char *method, const char *path);

#endif /* REQUEST_VALIDATION_H */
