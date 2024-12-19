/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* filepath: web_server.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define INDEX_FILE "index.html"
#define BACKLOG 10
#define SERVER_STRING "Server: EnvEngWebServer/1.0\r\n"

/* Global variables for cleanup */
static volatile sig_atomic_t server_running = 1;
static int server_socket = -1;

/* Function prototypes */
static void signal_handler(int sig);
static int setup_signals(void);
static int setup_socket(void);
static void handle_client(int client_socket);
static void cleanup(void);

static void
signal_handler(int sig)
{
    switch (sig) {
        case SIGTERM:
        case SIGINT:
            server_running = 0;
            break;
        case SIGCHLD:
            while (waitpid(-1, NULL, WNOHANG) > 0);
            break;
        default:
            break;
    }
}

static int
setup_signals(void)
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        return -1;
    }
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        return -1;
    }
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        return -1;
    }

    return 0;
}

static int
setup_socket(void)
{
    struct sockaddr_in server_addr;
    int optval;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        return -1;
    }

    optval = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR,
                   &optval, sizeof(optval)) < 0) {
        close(server_socket);
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0) {
        close(server_socket);
        return -1;
    }

    if (listen(server_socket, BACKLOG) < 0) {
        close(server_socket);
        return -1;
    }

    return 0;
}

static void
handle_client(int client_socket)
{
    char buffer[BUFFER_SIZE];
    char response_header[BUFFER_SIZE];
    int file_fd;
    struct stat file_stat;
    ssize_t bytes_read;
    ssize_t bytes_written;
    const char *filename = INDEX_FILE;

    /* Clear buffer before use */
    memset(buffer, 0, sizeof(buffer));

    /* Read client request */
    bytes_read = read(client_socket, buffer, BUFFER_SIZE - 1);
    if (bytes_read < 0) {
        return;
    }
    buffer[bytes_read] = '\0';

    /* Check if valid GET request */
    if (strncmp(buffer, "GET / ", 6) != 0) {
        /* Send 404 response */
        snprintf(response_header, sizeof(response_header),
                "HTTP/1.0 404 Not Found\r\n"
                "%s"
                "Content-Type: text/plain\r\n"
                "\r\n"
                "404 Not Found\r\n",
                SERVER_STRING);
        write(client_socket, response_header, strlen(response_header));
        return;
    }

    /* Open and check index file */
    file_fd = open(filename, O_RDONLY);
    if (file_fd < 0 || fstat(file_fd, &file_stat) < 0) {
        /* Send 500 response */
        snprintf(response_header, sizeof(response_header),
                "HTTP/1.0 500 Internal Server Error\r\n"
                "%s"
                "Content-Type: text/plain\r\n"
                "\r\n"
                "500 Internal Server Error\r\n",
                SERVER_STRING);
        write(client_socket, response_header, strlen(response_header));
        if (file_fd >= 0) {
            close(file_fd);
        }
        return;
    }

    /* Send success response header */
    snprintf(response_header, sizeof(response_header),
            "HTTP/1.0 200 OK\r\n"
            "%s"
            "Content-Type: text/html\r\n"
            "Content-Length: %ld\r\n"
            "\r\n",
            SERVER_STRING,
            (long)file_stat.st_size);

    bytes_written = write(client_socket, response_header, strlen(response_header));
    if (bytes_written < 0) {
        close(file_fd);
        return;
    }

    /* Send file content */
    while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(client_socket, buffer, (size_t)bytes_read);
        if (bytes_written < 0) {
            break;
        }
    }

    close(file_fd);
}

static void
cleanup(void)
{
    if (server_socket != -1) {
        close(server_socket);
        server_socket = -1;
    }
}

int
main(void)
{
    struct sockaddr_in client_addr;
    socklen_t client_len;
    int client_socket;

    if (setup_signals() < 0) {
        perror("Failed to setup signals");
        return EXIT_FAILURE;
    }

    if (setup_socket() < 0) {
        perror("Failed to setup socket");
        return EXIT_FAILURE;
    }

    printf("Server running on port %d...\n", PORT);

    while (server_running) {
        client_len = sizeof(client_addr);
        client_socket = accept(server_socket,
                             (struct sockaddr *)&client_addr,
                             &client_len);

        if (client_socket < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("Failed to accept connection");
            continue;
        }

        handle_client(client_socket);
    }

    cleanup();
    return EXIT_SUCCESS;
}
