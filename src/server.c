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
#include <pthread.h>
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
 * \brief Handles client requests.
 *
 * \param arg Pointer to the client socket file descriptor.
 * \return NULL.
 */
void *
handleClient (void *arg)
{
    int client_fd = *(int *)arg;
    free (arg);

    char buffer[1024];
    ssize_t bytes_read;

    bytes_read = read (client_fd, buffer, sizeof (buffer) - 1);
    if (bytes_read > 0)
    {
        buffer[bytes_read] = '\0';
        logInfo ("Received request: %s", buffer);

        char file_path[PATH_SIZE];
        parseHttpRequest (buffer, file_path);
        routeRequest (client_fd, file_path);
    }

    close (client_fd);
    return NULL;
}

/**
 * \brief Initializes the server configuration.
 *
 * \param config Pointer to the server configuration structure.
 */
void initializeServer(server_config_t *config)
{
    int opt = 1;

    config->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (config->server_fd == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(config->server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("Error setting socket options");
        close(config->server_fd);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(getServerIp());
    server_addr.sin_port = htons(getServerPort());

    if (bind(config->server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error binding socket");
        close(config->server_fd);
        exit(EXIT_FAILURE);
    }
}

/**
 * \brief Runs the server.
 *
 * \param config Pointer to the server configuration structure.
 * \return 0 on success, -1 on failure.
 */
int
runServer (server_config_t *config)
{
    pthread_t thread_id;
    int       client_fd;
    int *     client_fd_ptr;

    if (listen (config->server_fd, BACKLOG) < 0)
    {
        perror ("Error in listen");
        return -1;
    }

    while ((client_fd = accept (config->server_fd, NULL, NULL)) >= 0)
    {
        client_fd_ptr = (int *)malloc (sizeof (int));
        if (client_fd_ptr == NULL)
        {
            perror ("Error allocating memory for client_fd_ptr");
            continue;
        }
        *client_fd_ptr = client_fd;

        pthread_create (&thread_id, NULL, handleClient, client_fd_ptr);
        pthread_detach (thread_id);  // Detach the thread to avoid memory leaks
    }

    return 0;
}

/**
 * \brief Cleans up the server configuration.
 *
 * \param config Pointer to the server configuration structure.
 */
void
cleanupServer (server_config_t *config)
{
    close(config->server_fd);
}
