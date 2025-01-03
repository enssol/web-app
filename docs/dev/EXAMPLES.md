# Code Examples

## Example 1: Basic HTTP Server

### Header File
```c
/* filepath: include/http_server.h */
#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_CONNECTIONS 10

int startServer(int port);

#endif /* HTTP_SERVER_H */
```

### Source File
```c
/* filepath: src/http_server.c */
#include "http_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int startServer(int port)
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CONNECTIONS) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    while ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) >= 0) {
        char buffer[1024] = {0};
        read(new_socket, buffer, 1024);
        printf("Received: %s\n", buffer);
        close(new_socket);
    }

    close(server_fd);
    return 0;
}
```

## Example 2: Simple String Manipulation

### Header File
```c
/* filepath: include/string_utils.h */
#ifndef STRING_UTILS_H
#define STRING_UTILS_H

char *toUpperCase(char *str);

#endif /* STRING_UTILS_H */
```

### Source File
```c
/* filepath: src/string_utils.c */
#include "string_utils.h"
#include <ctype.h>

char *toUpperCase(char *str)
{
    char *p = str;
    while (*p) {
        *p = toupper((unsigned char)*p);
        p++;
    }
    return str;
}
```
