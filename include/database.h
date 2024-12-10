/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#ifndef DATABASE_H
#define DATABASE_H

void init_database ();
void add_user (const char *username, const char *password);
void update_user_profile (const char *username, const char *email,
                          const char *phone, const char *pronouns);

#endif
