/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#include "../include/database.h"
#include <rec.h>
#include <stdio.h>
#include <stdlib.h>

void
init_database ()
{
    const char *database_path = getenv ("DATABASE_PATH");
    if (database_path == NULL)
        {
            fprintf (stderr, "DATABASE_PATH not set\n");
            exit (EXIT_FAILURE);
        }
    // Initialize the database using database_path
    printf ("Database initialized at %s.\n", database_path);
}

void
add_user (const char *username, const char *password)
{
    printf ("User %s added.\n", username);
}

void
update_user_profile (const char *username, const char *email,
                     const char *phone, const char *pronouns)
{
    printf ("User %s profile updated.\n", username);
}
