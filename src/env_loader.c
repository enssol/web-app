/**
 * \file env_loader.c
 * \brief Implements environment variable loading functions.
 * \author Adrian Gallo
 * \license AGPL-3.0-or-later
 */

#include "../include/env_loader.h"
#include "../include/constants.h"
#include "../include/utils.h"
#include "../include/error_codes.h"
#include "../include/logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define SERVER_IP_SIZE 16
#define SSL_CERT_FILE_SIZE 256
#define SSL_KEY_FILE_SIZE 256

char server_ip[SERVER_IP_SIZE];
int server_port;
char ssl_cert_file[SSL_CERT_FILE_SIZE];
char ssl_key_file[SSL_KEY_FILE_SIZE];

/**
 * \brief Loads environment configuration from a file.
 *
 * \param filename The name of the file to load.
 * \return SUCCESS on success, otherwise an error code.
 */
int loadEnvConfig(const char *filename)
{
    FILE *file;
    char line[MAX_LINE_LENGTH];
    char key[MAX_LINE_LENGTH];
    char value[MAX_LINE_LENGTH];

    file = fopen(filename, "r");
    if (!file)
    {
        perror("Failed to open environment config file");
        return ERROR_FILE_OPEN;
    }

    while (fgets(line, sizeof(line), file))
    {
        if (sscanf(line, "%255[^=]=%255s", key, value) == 2)
        {
            if (strcmp(key, "SERVER_IP") == 0)
            {
                strncpy(server_ip, value, SERVER_IP_SIZE - 1);
                server_ip[SERVER_IP_SIZE - 1] = '\0';
            }
            else if (strcmp(key, "SERVER_PORT") == 0)
            {
                server_port = atoi(value);
            }
            else if (strcmp(key, "SSL_CERT_FILE") == 0)
            {
                strncpy(ssl_cert_file, value, SSL_CERT_FILE_SIZE - 1);
                ssl_cert_file[SSL_CERT_FILE_SIZE - 1] = '\0';
            }
            else if (strcmp(key, "SSL_KEY_FILE") == 0)
            {
                strncpy(ssl_key_file, value, SSL_KEY_FILE_SIZE - 1);
                ssl_key_file[SSL_KEY_FILE_SIZE - 1] = '\0';
            }
            logInfo("Loaded environment variable: %s=%s", key, value);
        }
    }

    fclose(file);
    return SUCCESS;
}
