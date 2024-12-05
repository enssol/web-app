/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include <stdio.h>
#include "../include/config_loader.h"
#include "../include/logger.h"
#include "../include/garbage_collector.h"
#include "../include/error_handler.h"
#include "../include/validator.h"

// Define constants and global variables
#define APP_NAME "App"
#define CURRENT_CONFIG_VERSION 2.0
#define CONFIG_VERSION 2.0
const int global_var = 0;

const char *app_mode;
double version;

// Initialize configuration with default values
typedef struct {
    const char *app_name;
    const char *log_level;
    const char *app_mode;
    double version;
} Config;

const Config config = {
    .app_name = "DefaultApp",
    .log_level = "INFO",
    .app_mode = "production",
    .version = 1.0
};

// Function prototypes
void initialize(void);
void hello(void);
void cleanup(void);
void upgrade_config(double old_version);

int main(void)
{
    // Initialize the application
    initialize();

    // Main loop
    hello();

    // Cleanup
    cleanup();
    return 0;
}

void hello(void)
{
    printf("Hello, world!\n");
}

/**
 * Initialize the application
 */
void initialize(void)
{
    // Initialize logger
    init_logger();
    // Load configuration
    if (load_ini_config("../etc/config.ini") != 0)
    {
        handleError("Failed to load INI configuration");
    }

    // Log application start
    log_info("Application started: %s v%.1f", config.app_name, config.version);
}

/**
 * Cleanup the application
 */
void cleanup(void)
{
    // Perform cleanup tasks
}

/**
 * Upgrade configuration to the current version.
 * This function applies necessary transformations to upgrade the configuration to the current version.
 * @param old_version The old version of the configuration.
 */
void upgrade_config(double old_version)
{
    if (old_version < 2.0)
    {
        // Add upgrade logic here
    }
    // Add more transformations for future versions as needed
}
