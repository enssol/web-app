/**
 * \file config_loader.c
 * \brief Implements configuration loading functions.
 * \author Adrian Gallo
 * \copyright 2024 Enveng Group
 * \license AGPL-3.0-or-later
 */

#include "../include/config_loader.h"
#include "../include/error_codes.h"
#include "../include/garbage_collector.h"
#include "../include/logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONFIG_ENTRIES 100
#define MAX_LINE_LENGTH 256
#define MAX_KEY_LENGTH 256
#define MAX_VALUE_LENGTH 256

typedef struct
{
    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LENGTH];
} config_entry_t;

static config_entry_t config_entries[MAX_CONFIG_ENTRIES];
static int entry_count = 0;

char document_root[MAX_PATH_LENGTH];

int
initializeConfig (Config *config)
{
    /* Initialize the config structure */
    memset (config, 0, sizeof (Config));
    return SUCCESS;
}

int
loadConfig (const char *filename, Config *config)
{
    FILE *file;
    char line[MAX_LINE_LENGTH];
    char key[MAX_LINE_LENGTH];
    char value[MAX_LINE_LENGTH];

    file = fopen(filename, "r");
    if (file == NULL)
    {
        logError("Failed to open config file: %s", filename);
        return ERROR_FILE_OPEN;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%255[^=]=%255s", key, value) == 2)
        {
            strncpy(config_entries[entry_count].key, key, MAX_KEY_LENGTH - 1);
            config_entries[entry_count].key[MAX_KEY_LENGTH - 1] = '\0';
            strncpy(config_entries[entry_count].value, value, MAX_VALUE_LENGTH - 1);
            config_entries[entry_count].value[MAX_VALUE_LENGTH - 1] = '\0';
            entry_count++;
            logInfo("Loaded config entry: %s=%s", key, value);

            if (strcmp(key, "server_ip") == 0)
            {
                strncpy(config->server_ip, value, sizeof(config->server_ip) - 1);
                config->server_ip[sizeof(config->server_ip) - 1] = '\0';
            }
            else if (strcmp(key, "server_port") == 0)
            {
                config->server_port = atoi(value);
            }
            else if (strcmp(key, "ssl_cert_file") == 0)
            {
                strncpy(config->ssl_cert_file, value, sizeof(config->ssl_cert_file) - 1);
                config->ssl_cert_file[sizeof(config->ssl_cert_file) - 1] = '\0';
            }
            else if (strcmp(key, "ssl_key_file") == 0)
            {
                strncpy(config->ssl_key_file, value, sizeof(config->ssl_key_file) - 1);
                config->ssl_key_file[sizeof(config->ssl_key_file) - 1] = '\0';
            }
            else if (strcmp(key, "document_root") == 0)
            {
                strncpy(config->document_root, value, sizeof(config->document_root) - 1);
                config->document_root[sizeof(config->document_root) - 1] = '\0';
            }
        }
    }

    fclose(file);

    config->entry_count = entry_count;
    return SUCCESS;
}

const char *
getConfigValue (const char *key)
{
    int i;
    for (i = 0; i < entry_count; i++)
        {
            if (strcmp (config_entries[i].key, key) == 0)
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
int
setConfigValue (Config *config, ConfigEntry entry)
{
    if (config->entry_count >= MAX_CONFIG_ENTRIES)
        {
            logError ("Maximum number of config entries exceeded");
            return ERROR_CONFIG_LOAD;
        }

    strncpy (config->entries[config->entry_count].key, entry.key,
             sizeof (config->entries[config->entry_count].key) - 1);
    config->entries[config->entry_count]
        .key[sizeof (config->entries[config->entry_count].key) - 1]
        = '\0';

    strncpy (config->entries[config->entry_count].value, entry.value,
             sizeof (config->entries[config->entry_count].value) - 1);
    config->entries[config->entry_count]
        .value[sizeof (config->entries[config->entry_count].value) - 1]
        = '\0';

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
    logInfo("Configuration loaded:");
    logInfo("Server IP: %s", config->server_ip);
    logInfo("Server Port: %d", config->server_port);
    logInfo("Document Root: %s", config->document_root);
    logInfo("SSL Cert File: %s", config->ssl_cert_file);
    logInfo("SSL Key File: %s", config->ssl_key_file);
}

/**
 * \brief Frees the memory allocated for the configuration.
 *
 * \param config Pointer to the configuration structure to free.
 */
void
freeConfig (Config *config)
{
    if (config == NULL)
        {
            return;
        }

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
    Config config;

    if (filename == NULL)
        {
            logError ("Invalid filename");
            return -1;
        }

    logInfo ("Loading configuration from %s", filename);

    FILE *file = fopen (filename, "r");

    if (file == NULL)
        {
            logError ("Failed to open file: %s", filename);
            return -1;
        }

    char line[MAX_LINE_LENGTH];
    ConfigEntry entry;

    while (fgets (line, sizeof (line), file) != NULL)
        {
            if (parseConfigLine (line, &entry) == 0)
                {
                    if (setConfigValue (&config, entry) != 0)
                        {
                            logError ("Failed to set config value");
                            fclose (file);
                            return -1;
                        }
                }
        }

    fclose (file);

    logFinalConfig (&config);

    return 0;
}
