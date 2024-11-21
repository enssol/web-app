/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#include "config.h"
#include "gettext.h"
#include <stdio.h>
#include <stdarg.h>
#include "logger.h"

void init_logger() {
    // Initialize logging system
}

void log_info(const char *message, ...) {
    va_list args;
    va_start(args, message);
    vprintf(message, args);
    printf("\n");
    va_end(args);
}

void log_error(const char *message, ...) {
    va_list args;
    va_start(args, message);
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");
    va_end(args);
}
