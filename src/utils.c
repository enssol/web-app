/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#include "../include/utils.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

void
load_env ()
{
    const char *vault_key = getenv ("DOTENV_VAULT_KEY");
    if (vault_key == NULL)
        {
            fprintf (stderr, "DOTENV_VAULT_KEY not set\n");
            exit (EXIT_FAILURE);
        }

    setlocale (LC_ALL, getenv ("LOCALE"));
}
