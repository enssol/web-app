#ifndef HTTP_USER_HANDLER_H
#define HTTP_USER_HANDLER_H

/* Function prototypes */
int handle_users_request(int client_socket);
int handle_update_user(int client_socket, const char *username,
                      const char *fullname, const char *email,
                      const char *project);

#endif /* HTTP_USER_HANDLER_H */
