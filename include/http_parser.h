/**
 * \file http_parser.h
 * \brief Declares functions and structures for parsing HTTP requests.
 * \author Adrian Gallo
 * \copyright 2024 Enveng Group
 * \license AGPL-3.0-or-later
 */

#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include <stdbool.h>

#define MAX_HEADER_COUNT 100
#define MAX_HEADER_NAME_LENGTH 256
#define MAX_HEADER_VALUE_LENGTH 256

typedef struct
{
    char method[8];
    char uri[256];
    char version[16];
    struct
    {
        char name[MAX_HEADER_NAME_LENGTH];
        char value[MAX_HEADER_VALUE_LENGTH];
    } headers[MAX_HEADER_COUNT];
    int header_count;
    char body[1024];
} HttpRequest;

typedef struct
{
    int status_code;
    char status_message[64];
    struct
    {
        char name[MAX_HEADER_NAME_LENGTH];
        char value[MAX_HEADER_VALUE_LENGTH];
    } headers[MAX_HEADER_COUNT];
    int header_count;
    char body[1024];
} HttpResponse;

#endif /* HTTP_PARSER_H */
