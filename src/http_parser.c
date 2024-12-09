/**
 * \file http_parser.c
 * \brief Implements functions for parsing HTTP requests.
 * \author Adrian Gallo
 * \license AGPL-3.0-or-later
 */

#include "../include/http_parser.h"
#include "../include/static_file_handler.h"
#include "../include/logger.h"
#include "../include/auth.h"  /* Ensure this is included */
#include <stdio.h>
#include <string.h>

void parseHttpRequest(const char *request, char *file_path)
{
    sscanf(request, "GET /%255s HTTP/1.1", file_path);
    if (strlen(file_path) == 0)
    {
        strcpy(file_path, "index.html");
    }
}

void routeRequest(int client_fd, const char *file_path)
{
    if (file_path == NULL)
    {
        logError("Error: file_path is NULL");
        return;
    }

    logInfo("Routing request for file: %s", file_path);

    if (strcmp(file_path, "login") == 0)
    {
        file_path = "login.html";
    }
    else if (strcmp(file_path, "logout") == 0)
    {
        file_path = "logout.html";
    }
    else if (strcmp(file_path, "welcome") == 0)
    {
        file_path = "welcome.html";
    }
    else if (strcmp(file_path, "authenticate") == 0)
    {
        handleLogin(client_fd);
        return;
    }

    serveStaticFile(client_fd, file_path);
}
