/**
 * \file server.c
 * \brief Implements server-specific functions.
 * \author Adrian Gallo
 * \copyright 2024 Enveng Group
 * \license AGPL-3.0-or-later
 */

#include "../include/server.h"
#include "../include/config_loader.h"
#include "../include/http_parser.h"
#include "../include/http_response.h"
#include "../include/logger.h"
#include "../include/socket_module.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BACKLOG 10
#define PATH_SIZE 256
#define MAX_PATH_LENGTH 256

/* Function prototypes */
const char *getServerIp (void);
int getServerPort (void);
void parseHttpRequest (const char *request, char *file_path);
void routeRequest (int client_fd, const char *file_path);

/**
 * \brief Creates and binds the server socket.
 *
 * \return Server socket file descriptor on success, otherwise -1.
 */
int
createAndBindSocket (void)

    /**
     * \brief Accepts an incoming connection.
     *
     * \param server_fd Server socket file descriptor.
     * \return Client socket file descriptor on success, otherwise -1.
     */
    int acceptConnection (int server_fd)

    /**
     * \brief Handles client requests.
     *
     * \param arg Pointer to the client socket file descriptor.
     * \return NULL.
     */
    void *handleClient (void *arg)
{
    int client_fd = *(int *)arg;
    free (arg);

    char buffer[RESPONSE_BUFFER_SIZE];
    ssize_t bytes_read;
    char file_path[256];

    while ((bytes_read = read (client_fd, buffer, sizeof (buffer))) > 0)
        {
            /* Handle client request */
            logInfo ("Received data from client: %s", buffer);
            parseHttpRequest (buffer, file_path);
            routeRequest (client_fd, file_path);
        }

    close (client_fd);
    return NULL;
}
