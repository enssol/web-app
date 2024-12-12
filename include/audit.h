/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#ifndef AUDIT_H
#define AUDIT_H

#include <time.h>
#include <sys/types.h>

/* Audit event types */
enum AuditEventType {
    AUDIT_AUTH_LOGIN = 1,
    AUDIT_AUTH_LOGOUT,
    AUDIT_PROFILE_VIEW,
    AUDIT_PROFILE_EDIT,
    AUDIT_PROJECT_VIEW,
    AUDIT_PROJECT_EDIT,
    AUDIT_USER_CREATE,
    AUDIT_USER_DELETE
};

/* Audit entry structure */
struct AuditEntry {
    time_t timestamp;
    uid_t user_id;
    char username[32];
    enum AuditEventType event_type;
    char details[256];
    char ip_address[46];
};

/* Function prototypes */
int auditInit(const char *audit_path);
int auditLog(const struct AuditEntry *entry);
int auditGetUserHistory(const char *username, struct AuditEntry **entries, size_t *count);
void auditCleanup(void);

#endif /* AUDIT_H */
