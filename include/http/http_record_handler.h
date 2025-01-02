/* filepath: /home/appuser/web-app/include/http/http_record_handler.h */
#ifndef HTTP_RECORD_HANDLER_H
#define HTTP_RECORD_HANDLER_H

#include "../standards.h"

/* HTTP record handling functions */
int handle_create_record(int client_socket, const char *data);
int handle_update_record(int client_socket, const char *data);
int handle_next_number(int client_socket);

#endif /* HTTP_RECORD_HANDLER_H */
