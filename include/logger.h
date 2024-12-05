/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>

void logError(const char *format, ...);
void log_error(const char *message);
void init_logger(void);
void log_info(const char *format, ...);

#endif // LOGGER_H
