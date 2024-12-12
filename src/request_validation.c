/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include "request_validation.h"
#include <string.h>

int validateHttpRequest(const char *method, const char *path)
{
    if (!method || !path) return -1;
    if (strcmp(method, "GET") && strcmp(method, "POST") &&
        strcmp(method, "PUT") && strcmp(method, "DELETE")) {
        return -1;
    }
    return 0;
}
