/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include "auth.h"

int authenticateUser(const char *username, const char *password) {
    FILE *passwd_file;
    char line[MAX_BUFFER_SIZE];
    char stored_username[MAX_BUFFER_SIZE];
    char stored_password[MAX_BUFFER_SIZE];
    int authenticated = 0;

    passwd_file = fopen("/etc/passwd", "r");
    if (!passwd_file) {
        perror("Failed to open passwd file");
        return 0;
    }

    while (fgets(line, sizeof(line), passwd_file)) {
        if (sscanf(line, "%[^:]:%[^:]", stored_username, stored_password) == 2) {
            if (strcmp(username, stored_username) == 0 &&
                strcmp(password, stored_password) == 0) {
                authenticated = 1;
                break;
            }
        }
    }

    fclose(passwd_file);
    return authenticated;
}
