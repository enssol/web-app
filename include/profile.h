/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#ifndef PROFILE_H
#define PROFILE_H

#include <stdio.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256

typedef struct {
    char email[MAX_BUFFER_SIZE];
    char phone[MAX_BUFFER_SIZE];
    char pronouns[MAX_BUFFER_SIZE];
} UserProfile;

int updateProfile(const char *username, const UserProfile *profile);

#endif /* PROFILE_H */
