/**
 * \file http_response.h
 * \brief Declares functions for handling HTTP requests and responses.
 * \author Adrian Gallo
 * \copyright 2024 Enveng Group
 * \license AGPL-3.0-or-later
 */

#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <stdbool.h>
#include "../include/http_parser.h"

#define RESPONSE_BUFFER_SIZE 16384

void *handleClient(void *client_fd_ptr);
void sendResponse(int client_fd, const char *file_path);
char *getFileExtension(const char *file_name);
char *urlDecode(const char *src);

#endif /* HTTP_RESPONSE_H */
