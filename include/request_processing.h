/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#ifndef REQUEST_PROCESSING_H
#define REQUEST_PROCESSING_H

#include <stdio.h>
#include <stdlib.h>

// Request processing functions
int processHttpRequest(const char *method, const char *path);

#endif /* REQUEST_PROCESSING_H */
