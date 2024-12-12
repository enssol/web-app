/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include "request_processing.h"
#include "request_validation.h"

int processHttpRequest(const char *method, const char *path)
{
    if (validateHttpRequest(method, path) < 0) {
        printf("Invalid HTTP request.\n");
        return -1;
    }
    printf("Processing %s request for %s\n", method, path);
    return 0;
}
