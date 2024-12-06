/**
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "env_loader.h"
#include "logger.h"
#include "error_handler.h"
#include "garbage_collector.h"
#include "constants.h"

int loadEnvConfig(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        logError("Failed to open .env file: %s", filename);
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        // Trim leading and trailing whitespace
        char *trimmed_line = line;
        while (isspace((unsigned char)*trimmed_line)) trimmed_line++;
        size_t len = strlen(trimmed_line);
        if (len == 0)
        {
            continue;
        }
        char *end = trimmed_line + len - 1;
        while (end > trimmed_line && isspace((unsigned char)*end)) end--;
        end[1] = '\0';

        // Skip empty lines and comments
        if (trimmed_line[0] == '\0' || trimmed_line[0] == '#' || trimmed_line[0] == ';')
        {
            continue;
        }

        logInfo("Read line: %s", trimmed_line);

        char *key = strtok(trimmed_line, "=");
        char *value = strtok(NULL, "\n");

        if (key && value)
        {
            // Trim leading and trailing whitespace from key and value
            while (isspace((unsigned char)*key)) key++;
            end = key + strlen(key) - 1;
            while (end > key && isspace((unsigned char)*end)) end--;
            end[1] = '\0';

            while (isspace((unsigned char)*value)) value++;
            end = value + strlen(value) - 1;
            while (end > value && isspace((unsigned char)*end)) end--;
            end[1] = '\0';

            logInfo("Parsed key: %s, value: %s", key, value);

            if (strcmp(key, "SERVER_IP") == 0)
            {
                strncpy(SERVER_IP, value, sizeof(SERVER_IP) - 1);
                SERVER_IP[sizeof(SERVER_IP) - 1] = '\0'; // Ensure null-termination
                logInfo("Loaded SERVER_IP: %s", SERVER_IP);
            }
            else if (strcmp(key, "SERVER_PORT") == 0)
            {
                SERVER_PORT = atoi(value);
                logInfo("Loaded SERVER_PORT: %d", SERVER_PORT);
            }
            else if (strcmp(key, "SSL_CERT_FILE") == 0)
            {
                strncpy(SSL_CERT_FILE, value, sizeof(SSL_CERT_FILE) - 1);
                SSL_CERT_FILE[sizeof(SSL_CERT_FILE) - 1] = '\0'; // Ensure null-termination
                logInfo("Loaded SSL_CERT_FILE: %s", SSL_CERT_FILE);
            }
            else if (strcmp(key, "SSL_KEY_FILE") == 0)
            {
                strncpy(SSL_KEY_FILE, value, sizeof(SSL_KEY_FILE) - 1);
                SSL_KEY_FILE[sizeof(SSL_KEY_FILE) - 1] = '\0'; // Ensure null-termination
                logInfo("Loaded SSL_KEY_FILE: %s", SSL_KEY_FILE);
            }
        }
        else
        {
            logError("Failed to parse line: %s", trimmed_line);
        }
    }

    fclose(file);
    return 0;
}
