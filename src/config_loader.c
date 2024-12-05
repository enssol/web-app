/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/config_loader.h"
#include "../include/validator.h"
#include "../include/logger.h"
#include "../include/error_handler.h"

#define MAX_LINE_LENGTH           1024
#define MAX_ERROR_MESSAGE_LENGTH  512

/**
 * @brief Helper function to process configuration file.
 *
 * @param file Pointer to the opened file.
 * @param config_type Type of configuration (INI or CONF).
 * @return int 0 on success, -1 on error.
 */
static int processConfigFile(FILE *file, const char *config_type)
{
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file))
    {
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "\n");
        if (key && value)
        {
            if (validate_string(value) != 0)
            {
                char error_message[MAX_ERROR_MESSAGE_LENGTH];
                snprintf(error_message, sizeof(error_message), "Invalid %s config value", config_type);
                logError(error_message);
                return -1;
            }
        }
    }

    return 0;
}

/**
 * @brief Load INI configuration file.
 *
 * @param filename Path to the configuration file.
 * @return int 0 on success, -1 on error.
 */
int loadIniConfig(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        logError("Failed to open INI config file: %s", filename);
        return -1;
    }

    int result = processConfigFile(file, "INI");

    if (fclose(file) != 0)
    {
        handleError("Error closing INI config file");
        return -1;
    }

    return result;
}

void someFunction() {
    // Example usage of log_error
    logError("An error occurred");
}
