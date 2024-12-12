/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <shadow.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../include/auth.h"
#include "../include/logger.h"
#include "../include/data_structures.h"

static struct UserSession *sessions = NULL;
static size_t session_count = 0;
static char passwd_file_path[PATH_MAX];

int authInit(const char *passwd_file)
{
    struct stat st;
    FILE *fp;

    if (!passwd_file) {
        logError("No password file specified");
        return -1;
    }

    /* Store passwd file path */
    strncpy(passwd_file_path, passwd_file, PATH_MAX - 1);
    passwd_file_path[PATH_MAX - 1] = '\0';

    /* Check file permissions */
    if (stat(passwd_file, &st) != 0) {
        logError("Cannot stat password file");
        return -1;
    }

    if ((st.st_mode & 077) != 0) {
        logError("Password file has incorrect permissions");
        return -1;
    }

    fp = fopen(passwd_file, "r");
    if (!fp) {
        logError("Cannot open password file");
        return -1;
    }

    fclose(fp);
    return 0;
}

int authLoadPasswdEntry(const char *username, struct PasswdEntry *entry)
{
    FILE *fp;
    char line[PASSWD_LINE_MAX];
    char *token;

    if (!username || !entry) {
        return -1;
    }

    fp = fopen(passwd_file_path, "r");
    if (!fp) {
        return -1;
    }

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#') continue;

        token = strtok(line, PASSWD_DELIM);
        if (token && strcmp(token, username) == 0) {
            strncpy(entry->username, token, MAX_USERNAME - 1);

            token = strtok(NULL, PASSWD_DELIM);
            if (token) strncpy(entry->password_hash, token, 127);

            token = strtok(NULL, PASSWD_DELIM);
            if (token) entry->uid = (uid_t)atoi(token);

            token = strtok(NULL, PASSWD_DELIM);
            if (token) entry->gid = (gid_t)atoi(token);

            token = strtok(NULL, PASSWD_DELIM);
            if (token) strncpy(entry->full_name, token, 255);

            token = strtok(NULL, PASSWD_DELIM);
            if (token) strncpy(entry->home_dir, token, PATH_MAX - 1);

            token = strtok(NULL, PASSWD_DELIM);
            if (token) {
                char *newline = strchr(token, '\n');
                if (newline) *newline = '\0';
                strncpy(entry->shell, token, 127);
            }

            fclose(fp);
            return 0;
        }
    }

    fclose(fp);
    return -1;
}

int authValidateUser(const char *username, const char *password)
{
    int result;
    struct AuditEntry audit;

    result = authVerifyPassword(password, entry.password_hash);

    /* Log authentication attempt */
    memset(&audit, 0, sizeof(audit));
    audit.timestamp = time(NULL);
    audit.user_id = getUserId(username);
    strncpy(audit.username, username, sizeof(audit.username) - 1);
    audit.event_type = result == 0 ? AUDIT_AUTH_LOGIN : AUDIT_AUTH_LOGOUT;
    snprintf(audit.details, sizeof(audit.details),
             "Login attempt %s", result == 0 ? "successful" : "failed");
    auditLog(&audit);

    return result;
}

int authGetUserProfile(const char *username, struct UserProfile *profile)
{
    struct PasswdEntry entry;
    int result;

    if (!username || !profile) {
        return -1;
    }

    result = authLoadPasswdEntry(username, &entry);
    if (result != 0) {
        return result;
    }

    /* Initialize profile with passwd data */
    memset(profile, 0, sizeof(struct UserProfile));
    strncpy(profile->username, entry.username, MAX_USERNAME - 1);
    strncpy(profile->responsibility, entry.full_name, sizeof(profile->responsibility) - 1);
    profile->last_login = time(NULL);

    return 0;
}

int authUpdateUserProfile(const char *username, const struct UserProfile *profile)
{
    struct AuditEntry audit;
    time_t now;

    if (!username || !profile) {
        return -1;
    }

    now = time(NULL);

    /* Log profile update */
    memset(&audit, 0, sizeof(audit));
    audit.timestamp = now;
    audit.user_id = getUserId(username);
    strncpy(audit.username, username, sizeof(audit.username) - 1);
    audit.event_type = AUDIT_PROFILE_EDIT;
    snprintf(audit.message, sizeof(audit.message),
            "Profile updated: email=%s, phone=%s",
            profile->email, profile->phone);

    if (auditLog(&audit) != 0) {
        return -1;
    }

    return authSaveUserProfile(profile);
}

int authSaveUserProfile(const struct UserProfile *profile)
{
    struct PasswdEntry entry;
    int result;

    if (!profile) {
        return -1;
    }

    /* Load existing entry */
    result = authLoadPasswdEntry(profile->username, &entry);
    if (result != 0) {
        return result;
    }

    /* Update relevant fields */
    strncpy(entry.full_name, profile->responsibility, sizeof(entry.full_name) - 1);

    return authSavePasswdEntry(&entry);
}

int setupAuthUsers(struct DBHandle *db)
{
    struct ProjectRecord *records;
    size_t count;
    FILE *passwd_file;
    char *responsibilities[1000];
    int unique_count = 0;

    /* Get all projects */
    if (dbListProjects(db, &records, &count) != 0) {
        return -1;
    }

    /* Extract unique responsibilities */
    for (size_t i = 0; i < count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(responsibilities[j], records[i].responsibility) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            responsibilities[unique_count++] = strdup(records[i].responsibility);
        }
    }

    /* Create passwd file entries */
    passwd_file = fopen("/devcontainer/web-app/etc/auth/passwd", "w");
    if (!passwd_file) {
        return -1;
    }

    fprintf(passwd_file, "# username:password_hash:uid:gid:full_name:home_dir:shell\n");
    fprintf(passwd_file, "admin:$6$xyz123...:1000:1000:Admin User:/home/admin:/bin/false\n");

    /* Add entries for each responsibility */
    for (int i = 0; i < unique_count; i++) {
        char username[32];
        /* Convert responsibility to username format */
        snprintf(username, sizeof(username), "%s", responsibilities[i]);
        /* Replace spaces with underscores */
        for (char *p = username; *p; p++) {
            if (*p == ' ') *p = '_';
        }
        fprintf(passwd_file, "%s:$6$abc456...:100%d:100%d:%s:/home/%s:/bin/false\n",
                username, i+1, i+1, responsibilities[i], username);
    }

    fclose(passwd_file);
    return 0;
}
