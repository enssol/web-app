#ifndef SOCKET_HANDLER_H
#define SOCKET_HANDLER_H

#include "../standards.h"

/* Function prototypes */
int read_socket(int sockfd, char *buffer, size_t size);
int write_socket(int sockfd, const char *data, size_t size);

#endif /* SOCKET_HANDLER_H */
