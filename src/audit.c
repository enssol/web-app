/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include "audit.h"

void logAuditEntry(const char *username, const char *action) {
    FILE *audit_file;

    audit_file = fopen("audit.log", "a");
    if (!audit_file) {
        perror("Failed to open audit log");
        return;
    }

    fprintf(audit_file, "User: %s, Action: %s, Timestamp: %ld\n",
            username, action, time(NULL));

    fclose(audit_file);
}
