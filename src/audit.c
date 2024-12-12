/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../include/audit.h"
#include "../include/logger.h"

static FILE *audit_file = NULL;
static pthread_mutex_t audit_mutex = PTHREAD_MUTEX_INITIALIZER;
static char audit_file_path[PATH_MAX];

int auditInit(const char *path)
{
    struct stat st;

    if (!path) {
        return -1;
    }

    strncpy(audit_file_path, path, sizeof(audit_file_path) - 1);

    /* Create directory if needed */
    char *last_slash = strrchr(audit_file_path, '/');
    if (last_slash) {
        *last_slash = '\0';
        mkdir(audit_file_path, 0750);
        *last_slash = '/';
    }

    /* Open audit file */
    audit_file = fopen(audit_file_path, "a");
    if (!audit_file) {
        return -1;
    }

    /* Set permissions */
    if (fstat(fileno(audit_file), &st) == 0) {
        if (fchmod(fileno(audit_file), 0640) != 0) {
            fclose(audit_file);
            return -1;
        }
    }

    return 0;
}

int auditLog(const struct AuditEntry *entry)
{
    char timestamp[32];
    struct tm *tm_info;
    int ret = 0;

    if (!entry || !audit_file) {
        return -1;
    }

    pthread_mutex_lock(&audit_mutex);

    tm_info = localtime(&entry->timestamp);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

    if (fprintf(audit_file, "%s|%d|%s|%d|%s|%s\n",
                timestamp,
                (int)entry->user_id,
                entry->username,
                entry->event_type,
                entry->details,
                entry->ip_address) < 0) {
        ret = -1;
    }

    fflush(audit_file);
    pthread_mutex_unlock(&audit_mutex);

    return ret;
}

void auditCleanup(void)
{
    pthread_mutex_lock(&audit_mutex);

    if (audit_file != NULL)
    {
        fflush(audit_file);
        fclose(audit_file);
        audit_file = NULL;
    }

    pthread_mutex_unlock(&audit_mutex);
    pthread_mutex_destroy(&audit_mutex);
}
