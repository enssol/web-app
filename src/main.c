/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "config_loader.h"
#include "env_loader.h"
#include "logger.h"
#include "garbage_collector.h"
#include "error_handler.h"
#include "validator.h"
#include "hello.h"

// Define constants and global variables
#define APP_NAME "MyApp"
#define CURRENT_CONFIG_VERSION 2.0
int global_var = 0;

/**
 * Configuration structure
 * This structure holds all the configuration options for the application.
 */
typedef struct {
    char app_name[50];        // Application name
    double version;           // Configuration version
    char log_level[10];       // Logging level (INFO, DEBUG, ERROR)
    int enable_feature_x;     // Enable feature X
    int enable_feature_y;     // Enable feature Y
    int max_retries;          // Maximum number of retries
    char app_mode[20];        // Application mode (development, production)
    int enable_feature_z;     // Enable feature Z
} Config;

/**
 * Initialize configuration with default values
 * These values will be used if the corresponding configuration options are not provided.
 */
Config config = {
    .app_name = "DefaultApp",
    .version = CURRENT_CONFIG_VERSION,
    .log_level = "INFO",
    .enable_feature_x = 0,
    .enable_feature_y = 0,
    .max_retries = 3,
    .app_mode = "production",
    .enable_feature_z = 0
};

// Function prototypes
void initialize();
void cleanup();
void handle_error(const char *error_message);
int load_ini_config(const char *filename);
int load_conf_config(const char *filename);
int load_env_config(const char *filename);
int validate_config(const char *key, const char *value);
int validate_integer(const char *value);
int validate_double(const char *value);
int validate_string(const char *value);
int validate_boolean(const char *value);
void upgrade_config(double old_version);

int main() {
    // Initialization
    initialize();

    // Main loop
    print_hello();

    // Cleanup
    cleanup();

    return 0;
}

/**
 * Initialize the application
 * This function sets up the initial state of the application, including configuration, logging, and database connections.
 */
void initialize() {
    // Load environment variables
    if (load_env_config("config/.env") != 0) {
        handle_error("Failed to load environment variables");
    }

    // Load configuration files
    if (load_ini_config("config/config.ini") != 0) {
        handle_error("Failed to load config.ini");
    }
    if (load_conf_config("config/config.conf") != 0) {
        handle_error("Failed to load config.conf");
    }

    // Initialize logging
    init_logger();

    // Initialize garbage collector
    init_garbage_collector();

    log_info("Initialization complete");
}

/**
 * Cleanup the application
 * This function releases any resources that were allocated during the application's execution.
 */
void cleanup() {
    log_info("Cleaning up...");
    cleanup_garbage_collector();
    log_info("Cleanup complete");
}

/**
 * Handle errors
 * This function prints an error message to stderr.
 * @param error_message The error message to print.
 */
void handle_error(const char *error_message) {
    fprintf(stderr, "Error: %s\n", error_message);
}

/**
 * Validate and assign configuration values based on the key.
 * @param key The configuration key.
 * @param value The configuration value.
 * @return 0 if the value is valid, -1 otherwise.
 */
int validate_config(const char *key, const char *value) {
    if (strcmp(key, "app_name") == 0) {
        if (validate_string(value) != 0) {
            return -1; // Invalid value
        }
        strcpy(config.app_name, value);
    } else if (strcmp(key, "version") == 0) {
        if (validate_double(value) != 0) {
            return -1; // Invalid value
        }
        double version = atof(value);
        if (version != CURRENT_CONFIG_VERSION) {
            upgrade_config(version);
        }
        config.version = version;
    } else if (strcmp(key, "log_level") == 0) {
        if (strcmp(value, "INFO") != 0 && strcmp(value, "DEBUG") != 0 && strcmp(value, "ERROR") != 0) {
            return -1; // Invalid value
        }
        strcpy(config.log_level, value);
    } else if (strcmp(key, "enable_feature_x") == 0) {
        if (validate_boolean(value) != 0) {
            return -1; // Invalid value
        }
        config.enable_feature_x = atoi(value);
    } else if (strcmp(key, "enable_feature_y") == 0) {
        if (validate_boolean(value) != 0) {
            return -1; // Invalid value
        }
        config.enable_feature_y = atoi(value);
    } else if (strcmp(key, "max_retries") == 0) {
        if (validate_integer(value) != 0) {
            return -1; // Invalid value
        }
        config.max_retries = atoi(value);
    } else if (strcmp(key, "APP_MODE") == 0) {
        if (validate_string(value) != 0) {
            return -1; // Invalid value
        }
        strcpy(config.app_mode, value);
    } else if (strcmp(key, "ENABLE_FEATURE_Z") == 0) {
        if (validate_boolean(value) != 0) {
            return -1; // Invalid value
        }
        config.enable_feature_z = atoi(value);
    }

    return 0; // Valid value
}

/**
 * Validate if the value is a valid integer.
 * @param value The value to validate.
 * @return 0 if the value is a valid integer, -1 otherwise.
 */
int validate_integer(const char *value) {
    for (int i = 0; value[i] != '\0'; i++) {
        if (!isdigit(value[i])) {
            return -1; // Invalid integer
        }
    }
    return 0; // Valid integer
}

/**
 * Validate if the value is a valid double.
 * @param value The value to validate.
 * @return 0 if the value is a valid double, -1 otherwise.
 */
int validate_double(const char *value) {
    int dot_count = 0;
    for (int i = 0; value[i] != '\0'; i++) {
        if (!isdigit(value[i])) {
            if (value[i] == '.' && dot_count == 0) {
                dot_count++;
            } else {
                return -1; // Invalid double
            }
        }
    }
    return 0; // Valid double
}

/**
 * Validate if the value is a valid string.
 * @param value The value to validate.
 * @return 0 if the value is a valid string, -1 otherwise.
 */
int validate_string(const char *value) {
    if (strlen(value) == 0) {
        return -1; // Invalid string
    }
    return 0; // Valid string
}

/**
 * Validate if the value is a valid boolean.
 * @param value The value to validate.
 * @return 0 if the value is a valid boolean, -1 otherwise.
 */
int validate_boolean(const char *value) {
    if (strcmp(value, "true") != 0 && strcmp(value, "false") != 0) {
        return -1; // Invalid boolean
    }
    return 0; // Valid boolean
}

/**
 * Load configuration from an INI file.
 * Use case: Simple text files with key-value pairs, often used for basic configuration settings.
 * @param filename The path to the INI file.
 * @return 0 if the configuration was loaded successfully, -1 otherwise.
 */
int load_ini_config(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return -1; // Error opening file
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "\n");
        if (key && value) {
            if (validate_config(key, value) != 0) {
                handle_error("Invalid INI config value");
                fclose(file);
                return -1;
            }
            printf("INI Config: %s = %s\n", key, value);
        }
    }

    fclose(file);
    return 0; // Success
}

/**
 * Load configuration from a CONF file.
 * Use case: Commonly used for system and application configuration files, especially in Unix-like systems.
 * @param filename The path to the CONF file.
 * @return 0 if the configuration was loaded successfully, -1 otherwise.
 */
int load_conf_config(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return -1; // Error opening file
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "\n");
        if (key && value) {
            if (validate_config(key, value) != 0) {
                handle_error("Invalid CONF config value");
                fclose(file);
                return -1;
            }
            printf("CONF Config: %s = %s\n", key, value);
        }
    }

    fclose(file);
    return 0; // Success
}

/**
 * Load configuration from an ENV file.
 * Use case: Environment variables.
 * @param filename The path to the ENV file.
 * @return 0 if the configuration was loaded successfully, -1 otherwise.
 */
int load_env_config(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return -1; // Error opening file
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "\n");
        if (key && value) {
            if (validate_config(key, value) != 0) {
                handle_error("Invalid ENV config value");
                fclose(file);
                return -1;
            }
            printf("ENV Config: %s = %s\n", key, value);
        }
    }

    fclose(file);
    return 0; // Success
}

/**
 * Upgrade configuration to the current version.
 * This function applies necessary transformations to upgrade the configuration to the current version.
 * @param old_version The old version of the configuration.
 */
void upgrade_config(double old_version) {
    if (old_version < 2.0) {
        // Example transformation for version 2.0
        // Add new configuration options or modify existing ones
        strcpy(config.log_level, "INFO"); // Set default log level if not present
        config.version = 2.0;
        printf("Upgraded configuration to version 2.0\n");
    }
    // Add more transformations for future versions as needed
}