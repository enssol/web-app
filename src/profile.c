/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include "profile.h"

int updateProfile(const char *username, const UserProfile *profile) {
    FILE *db_file;
    char line[MAX_BUFFER_SIZE];
    char temp_filename[] = "temp.rec";
    FILE *temp_file;
    int updated = 0;

    db_file = fopen("records.rec", "r");
    if (!db_file) {
        perror("Failed to open database");
        return 0;
    }

    temp_file = fopen(temp_filename, "w");
    if (!temp_file) {
        perror("Failed to create temp file");
        fclose(db_file);
        return 0;
    }

    while (fgets(line, sizeof(line), db_file)) {
        if (strstr(line, username)) {
            fprintf(temp_file, "Name: %s\nEmail: %s\nPhone: %s\nPronouns: %s\n",
                    username, profile->email, profile->phone, profile->pronouns);
            updated = 1;
        } else {
            fputs(line, temp_file);
        }
    }

    fclose(db_file);
    fclose(temp_file);

    if (updated) {
        rename(temp_filename, "records.rec");
    } else {
        remove(temp_filename);
    }

    return updated;
}
