/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#ifndef AUDIT_H
#define AUDIT_H

#include <stdio.h>
#include <time.h>

void logAuditEntry(const char *username, const char *action);

#endif /* AUDIT_H */
