/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

/* filepath: include/web_server.h */
#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <stddef.h>
#include "struct.h"

/* Function prototypes */
int check_auth(const char *username, const char *password);
void parse_query_string(const char *query, char *username, char *password);
int handle_client(int client_socket, const char *www_root);
int setup_server(int port);
int handle_update_user(int client_socket, const char *username, const char *fullname, const char *email, const char *project);
int handle_users_request(int client_socket);
int parse_auth_file(const char *filename, struct user_entry *entries, size_t max_entries);
int log_audit(const char *username, const char *action);

#endif /* WEB_SERVER_H */
