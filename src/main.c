/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#include "../include/auth.h"
#include "../include/database.h"
#include "../include/server.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>



int
main ()
{
    load_env ();
    printf ("Starting %s on %s (%s)\n", getenv ("APP_NAME"), getenv ("DOMAIN"), getenv ("PUBLIC_IP"));
    init_database ();
    start_server ();
    return 0;
}
