#ifndef HTTP_REQUEST_HANDLER_H
#define HTTP_REQUEST_HANDLER_H

#include "../standards.h"

/* HTTP request handling declarations */
int handle_client(int client_socket, const char *www_root);
int handle_users_request(int client_socket);
int handle_update_user(int client_socket, const char *username,
                      const char *fullname, const char *email,
                      const char *project);

#endif /* HTTP_REQUEST_HANDLER_H */
