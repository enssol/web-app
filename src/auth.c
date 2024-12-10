/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#include "../include/auth.h"
#include <crypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
authenticate (const char *username, const char *password)
{
    const char *auth_method = getenv ("AUTH_METHOD");
    if (auth_method == NULL)
        {
            fprintf (stderr, "AUTH_METHOD not set\n");
            exit (EXIT_FAILURE);
        }

    if (strcmp (auth_method, "passwd") == 0)
        {
            // Simple authentication logic
            if (strcmp (username, "admin") == 0 && strcmp (password, "password") == 0)
                {
                    return 1;
                }
        }
    return 0;
}
