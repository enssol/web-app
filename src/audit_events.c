/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "../include/session.h"
#include "../include/audit.h"
#include "../include/audit_events.h"

static void logSessionEvent(const char* session_id, uid_t uid,
                          const char* username, AuditEventType event_type)
{
    struct AuditEntry entry;
    char msg[AUDIT_MAX_MSG_LEN];

    entry.timestamp = time(NULL);
    entry.user_id = uid;
    strncpy(entry.username, username, sizeof(entry.username) - 1);
    entry.event_type = event_type;

    switch(event_type) {
        case AUDIT_SESSION_CREATED:
            snprintf(msg, sizeof(msg), "Session created: %s", session_id);
            break;
        case AUDIT_SESSION_VALIDATED:
            snprintf(msg, sizeof(msg), "Session validated: %s", session_id);
            break;
        case AUDIT_SESSION_EXPIRED:
            snprintf(msg, sizeof(msg), "Session expired: %s", session_id);
            break;
        case AUDIT_SESSION_DESTROYED:
            snprintf(msg, sizeof(msg), "Session destroyed: %s", session_id);
            break;
        default:
            snprintf(msg, sizeof(msg), "Unknown event for session: %s", session_id);
    }

    strncpy(entry.message, msg, sizeof(entry.message) - 1);
    auditLog(&entry);
}

char* sessionCreate(uid_t uid, const char* username, int permissions)
{
    struct Session* session;
    static char session_id[SESSION_ID_LEN];

    pthread_mutex_lock(&session_mutex);

    if (store.count >= MAX_SESSIONS) {
        pthread_mutex_unlock(&session_mutex);
        return NULL;
    }

    generateSessionId(session_id);
    session = &store.sessions[store.count++];

    /* Initialize session */
    session->uid = uid;
    session->created = time(NULL);
    session->last_access = session->created;
    session->permissions = permissions;
    strncpy(session->id, session_id, SESSION_ID_LEN);

    pthread_mutex_unlock(&session_mutex);

    /* Log session creation */
    logSessionEvent(session_id, uid, username, AUDIT_SESSION_CREATED);

    return session_id;
}
