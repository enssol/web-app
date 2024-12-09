/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/logger.h"
#include "../include/error_codes.h"
#include "../include/auth.h"
#include <unistd.h>
#include <sys/socket.h>

#define RESPONSE_BUFFER_SIZE 1024

/**
 * \brief Handles the login form submission.
 */
void handleLogin(int client_fd)
{
    char *username = getenv("USERNAME");
    char *password = getenv("PASSWORD");

    if (username == NULL || password == NULL)
    {
        const char *invalid_request_response = "HTTP/1.1 400 Bad Request\r\n\r\n<html><body><h1>Invalid request</h1></body></html>";
        send(client_fd, invalid_request_response, strlen(invalid_request_response), 0);
        return;
    }

    int result = authenticateUser(username, password);
    if (result == SUCCESS)
    {
        const char *success_response = "HTTP/1.1 200 OK\r\n\r\n<html><body><h1>Signed in successfully!</h1><a href=\"logout.html\">Log Out</a></body></html>";
        send(client_fd, success_response, strlen(success_response), 0);
    }
    else
    {
        const char *failure_response = "HTTP/1.1 401 Unauthorized\r\n\r\n<html><body><h1>Invalid username or password</h1><a href=\"login.html\">Try Again</a></body></html>";
        send(client_fd, failure_response, strlen(failure_response), 0);
    }
}
