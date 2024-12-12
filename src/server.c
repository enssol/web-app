/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include "server.h"

void initializeServer(void) {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Socket bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_socket, 10) < 0) {
        perror("Socket listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server running on port %d\n", SERVER_PORT);

    // Handle clients
    while (1) {
        addr_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (client_socket < 0) {
            perror("Client connection failed");
            continue;
        }

        handleClient(client_socket);
    }

    close(server_socket);
}

void handleClient(int client_socket) {
    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytes_read;

    bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) {
        perror("Failed to receive data");
        close(client_socket);
        return;
    }

    buffer[bytes_read] = '\0';

    if (strncmp(buffer, "GET /login", 10) == 0) {
        sendStaticPage(client_socket, "static/login.html");
    } else if (strncmp(buffer, "GET /dashboard", 14) == 0) {
        sendStaticPage(client_socket, "static/dashboard.html");
    } else if (strncmp(buffer, "GET /update_profile", 19) == 0) {
        sendStaticPage(client_socket, "static/update_profile.html");
    } else {
        const char *error_page = "HTTP/1.1 404 Not Found\r\n\r\n";
        send(client_socket, error_page, strlen(error_page), 0);
    }

    close(client_socket);
}

void sendStaticPage(int client_socket, const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        perror("Failed to open file");
        const char *error_page = "HTTP/1.1 500 Internal Server Error\r\n\r\n";
        send(client_socket, error_page, strlen(error_page), 0);
        return;
    }

    char response[MAX_BUFFER_SIZE];
    snprintf(response, sizeof(response), "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    send(client_socket, response, strlen(response), 0);

    while (fgets(response, sizeof(response), file)) {
        send(client_socket, response, strlen(response), 0);
    }

    fclose(file);
}
