/**
 * \file http_parser.c
 * \brief Implements functions for parsing HTTP requests.
 * \author Adrian Gallo
 * \license AGPL-3.0-or-later
 */

#include "../include/http_parser.h"
#include "../include/http_response.h"
#include "../include/static_file_handler.h"
#include "../include/logger.h"
#include <stdio.h>
#include <string.h>

#define MAX_PATH_LENGTH 256

typedef struct {
    const char *route;
    const char *file_path;
} RouteMapping;

RouteMapping routeMappings[] = {
    {"/", "index.html"},
    {"/login", "login.html"},
    {"/logout", "logout.html"},
    {"/welcome", "welcome.html"}
};

const char *getFilePath(const char *route)
{
    size_t i;
    for (i = 0; i < sizeof(routeMappings) / sizeof(RouteMapping); ++i)
    {
        if (strcmp(route, routeMappings[i].route) == 0)
        {
            logInfo("Route found: %s -> %s", route, routeMappings[i].file_path);
            return routeMappings[i].file_path;
        }
    }
    logWarning("Route not found: %s", route);
    return NULL;
}

void parseHttpRequest(const char *request, char *route, char *method)
{
    sscanf(request, "%s %255s HTTP/1.1", method, route);
    logInfo("Parsed HTTP request: Method=%s, Route=%s", method, route);
}

void routeRequest(int client_fd, const char *route, const char *method)
{
    const char *file_path = getFilePath(route);
    if (file_path)
    {
        logInfo("Serving file: %s", file_path);
        serveStaticFile(client_fd, file_path);
    }
    else
    {
        const char *not_found_response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(client_fd, not_found_response, strlen(not_found_response), 0);
        logError("File not found for route: %s", route);
    }
}
