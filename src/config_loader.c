/**
 * \file config_loader.c
 * \brief Implements configuration loading functions.
 * \author Adrian Gallo
 * \copyright 2024 Enveng Group
 * \license AGPL-3.0-or-later
 */

#include "../include/config_loader.h"
#include "../include/error_codes.h"
#include "../include/logger.h"
#include "../include/server.h" /* Assuming server_config_t and MAX_CLIENTS are defined here */
#include "../include/garbage_collector.h"
#include <arpa/inet.h>
#include <limits.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONFIG_ENTRIES 100
#define MAX_LINE_LENGTH    256
#define MAX_KEY_LENGTH     256
#define MAX_VALUE_LENGTH   256

typedef struct
{
    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LENGTH];
} config_entry_t;

static config_entry_t config_entries[MAX_CONFIG_ENTRIES];
static int entry_count = 0;

int loadConfig(const char *filename, Config *config)
{
    FILE *file;
    char line[MAX_LINE_LENGTH];
    char *key;
    char *value;
    int i;

    file = fopen(filename, "r");
    if (file == NULL)
    {
        logError("Failed to open config file: %s", filename);
        return -1;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        key = strtok(line, "=");
        value = strtok(NULL, "\n");
        if (key != NULL && value != NULL)
        {
            strncpy(config_entries[entry_count].key, key, MAX_KEY_LENGTH);
            strncpy(config_entries[entry_count].value, value, MAX_VALUE_LENGTH);
            entry_count++;
        }
    }

    fclose(file);

    config->entry_count = entry_count;

    return 0;
}

const char *getConfigValue(const char *key)
{
    int i;
    for (i = 0; i < entry_count; i++)
    {
        if (strcmp(config_entries[i].key, key) == 0)
        {
            return config_entries[i].value;
        }
    }
    return NULL;
}

char *configPath = NULL;
int logLevel = 0;
int maxConnections = 0;
int timeout = 0;


int setConfigValue (Config *config, ConfigEntry entry);
void logFinalConfig (const Config *config);
void freeConfig (Config *config);

/**
 * \brief Sets a configuration value.
 *
 * \param config Configuration structure.
 * \param entry Configuration entry.
 * \return SUCCESS on success, otherwise an error code.
 */
int setConfigValue(Config *config, ConfigEntry entry)
{
    if (config->entry_count >= MAX_CONFIG_ENTRIES)
    {
        logError("Maximum number of config entries exceeded");
        return ERROR_CONFIG_LOAD;
    }

    strncpy(config->entries[config->entry_count].key, entry.key, sizeof(config->entries[config->entry_count].key) - 1);
    config->entries[config->entry_count].key[sizeof(config->entries[config->entry_count].key) - 1] = '\0';

    strncpy(config->entries[config->entry_count].value, entry.value, sizeof(config->entries[config->entry_count].value) - 1);
    config->entries[config->entry_count].value[sizeof(config->entries[config->entry_count].value) - 1] = '\0';

    config->entry_count++;
    return SUCCESS;
}

/**
 * \brief Logs the final configuration.
 *
 * \param config The configuration structure to log.
 */
void
logFinalConfig (const Config *config)
{
    int i;

    logInfo("Configuration loaded:");
    for (i = 0; i < config->entry_count; i++)
    {
        logInfo("  %s = %s", config->entries[i].key, config->entries[i].value);
    }
}

/**
 * \brief Frees the memory allocated for the configuration.
 *
 * \param config Pointer to the configuration structure to free.
 */
void freeConfig(Config *config)
{
    if (config == NULL)
    {
        return;
    }

    GarbageCollector gc;
    initializeGarbageCollector(&gc);
    cleanupGarbageCollector(&gc);
    config->entry_count = 0;
}

/**
 * \brief Parses a configuration line.
 *
 * \param line The line to parse.
 * \param entry The configuration entry to populate.
 * \return 0 on success, -1 on failure.
 */
static int
parseConfigLine (char *line, ConfigEntry *entry)
{
    char key[MAX_LINE_LENGTH];
    char value[MAX_LINE_LENGTH];

    if (sscanf (line, "%255[^=]=%255s", key, value) != 2)
        {
            return -1;
        }

    strncpy (entry->key, key, sizeof (entry->key) - 1);
    entry->key[sizeof (entry->key) - 1] = '\0';
    strncpy (entry->value, value, sizeof (entry->value) - 1);
    entry->value[sizeof (entry->value) - 1] = '\0';

    return 0;
}

int
checkAdditionOverflow (int a, int b)
{
    if ((b > 0) && (a > (INT_MAX - b)))
        {
            return 1; /* Overflow */
        }
    if ((b < 0) && (a < (INT_MIN - b)))
        {
            return 1; /* Underflow */
        }
    return 0; /* No overflow */
}

int
loadIniConfig (const char *filename)
{
    if (filename == NULL)
        {
            logError ("Invalid filename");
            return -1;
        }

    logInfo ("Loading configuration from %s", filename);

    /* Load configuration logic here */

    return 0;
}

void
initialize_server (server_config_t *config)
{
    (void)config;
}

int
run_server (server_config_t *config)
{
    pthread_t thread_id;
    int client_fd;
    int *client_fd_ptr;

    if (listen (config->server_fd, MAX_CLIENTS) < 0)
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

            pthread_create (&thread_id, NULL, handle_client, client_fd_ptr);
        }

    return 0;
}

void
cleanup_server (server_config_t *config)
{
    (void)config;
}
