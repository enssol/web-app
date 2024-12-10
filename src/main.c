/**
 * \file main.c
 * \brief Main entry point for the application.
 * \author Adrian Gallo
 * \license AGPL-3.0-or-later
 */

#include "../include/env_loader.h"
#include "../include/config_loader.h"
#include "../include/logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define ENV_FILE ".env"
#define CONFIG_FILE "etc/config.ini"
#define LOG_FILE "app.log"

/* Global configuration variable */
Config config;

/* Function prototypes */
int initialize(void);
void cleanup(void);
void printLoadedConfig(void);
void startServer(void);

/**
 * \brief Main function.
 *
 * \return Exit status.
 */
int main(void)
{
    /* Initialize the config structure */
    memset(&config, 0, sizeof(Config));

    if (loadConfig(CONFIG_FILE, &config) != SUCCESS)
    {
        fprintf(stderr, "Failed to load configuration\n");
        return EXIT_FAILURE;
    }

    if (initialize() != SUCCESS)
    {
        fprintf(stderr, "Failed to initialize application\n");
        return EXIT_FAILURE;
    }

    printLoadedConfig();

    /* Start the server */
    startServer();

    cleanup();

    /* Free the configuration resources */
    freeConfig(&config);

    return EXIT_SUCCESS;
}

/**
 * \brief Initializes the application.
 *
 * \return SUCCESS on success, otherwise an error code.
 */
int initialize(void)
{
    initLogger(LOG_FILE);
    logInfo("Application starting...");

    if (loadEnvConfig(ENV_FILE) != SUCCESS)
    {
        logError("Failed to load environment configuration");
        return ERROR_FILE_OPEN;
    }

    if (loadConfig(CONFIG_FILE, &config) != SUCCESS)
    {
        logError("Failed to load configuration");
        return ERROR_FILE_OPEN;
    }

    logFinalConfig(&config);
    logInfo("All modules initialized successfully");
    return SUCCESS;
}

/**
 * \brief Cleans up resources before exiting the application.
 */
void cleanup(void)
{
    closeLogger();
}

/**
 * \brief Prints the loaded configuration.
 */
void printLoadedConfig(void)
{
    logInfo("Loaded configuration:");
    logInfo("Server IP: %s", config.server_ip);
    logInfo("Server Port: %d", config.server_port);
    logInfo("Document Root: %s", config.document_root);
    logInfo("SSL Cert File: %s", config.ssl_cert_file);
    logInfo("SSL Key File: %s", config.ssl_key_file);
}

/**
 * \brief Starts the server and handles incoming connections.
 */
void startServer(void)
{
    /* Server start logic */
}
