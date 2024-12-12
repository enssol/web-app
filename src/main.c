/* filepath: /devcontainer/web-app/src/main.c */
/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <locale.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>

#include "../include/config.h"
#include "../include/server.h"
#include "../include/logger.h"
#include "../include/database.h"
#include "../include/auth.h"
#include "../include/audit.h"
#include "../include/session.h"
#include "../include/utils.h"
#include "../include/data_structures.h"

#define DEFAULT_CONFIG_PATH "/etc/env/.env"
#define DEFAULT_LOG_PATH "/etc/log/app.log"
#define DEFAULT_DB_PATH "/etc/db/records.rec"
#define DEFAULT_AUTH_PATH "/etc/auth/passwd"
#define DEFAULT_SSL_CERT "/etc/ssl/cert.pem"
#define DEFAULT_SSL_KEY "/etc/ssl/privkey.pem"
#define DEFAULT_AUDIT_PATH "/var/log/webapp/audit.log"

static volatile sig_atomic_t running = 1;
static struct DBHandle db_handle;

/* Forward declarations */
static void cleanupHandler(int signum);
static void signalHandler(int sig);
static int initializeSubsystems(const struct Config *config);
static int createRequiredDirectories(void);

int main(int argc, char *argv[])
{
    struct Config config;
    const char *config_path = DEFAULT_CONFIG_PATH;
    int ret;
    char audit_path[256];
    struct sigaction sa;

    /* Setup signal handlers */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signalHandler;
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);

    /* Set locale */
    if (!setlocale(LC_ALL, "en_AU.UTF-8")) {
        fprintf(stderr, "Failed to set locale\n");
        return EXIT_FAILURE;
    }

    /* Override config path if provided */
    if (argc > 1) {
        config_path = argv[1];
    }

    /* Create required directories */
    if (createRequiredDirectories() != 0) {
        fprintf(stderr, "Failed to create required directories\n");
        return EXIT_FAILURE;
    }

    /* Load configuration */
    if (loadConfig(config_path, &config) != 0) {
        fprintf(stderr, "Failed to load configuration from %s\n", config_path);
        return EXIT_FAILURE;
    }

    /* Initialize logging first */
    if (logInit(config.log_path, LOG_LEVEL_INFO) != 0) {
        fprintf(stderr, "Failed to initialize logging\n");
        return 1;
    }

    /* Initialize audit system */
    snprintf(audit_path, sizeof(audit_path), "%s/audit.log", config.log_path);
    if (auditInit(audit_path) != 0) {
        logError("Failed to initialize audit system");
        return 1;
    }

    logInfo("Audit system initialized successfully");

    /* Initialize subsystems */
    if (initializeSubsystems(&config) != 0) {
        fprintf(stderr, "Failed to initialize subsystems\n");
        return EXIT_FAILURE;
    }

    logInfo("All subsystems initialized successfully");

    /* Initialize and start server */
    struct ServerConfig server_config;
    server_config.port = config.server_port;
    server_config.ssl_cert = config.ssl_cert_file;
    server_config.ssl_key = config.ssl_key_file;
    server_config.db_file = config.db_file;
    server_config.env_file = config_path;

    ret = initServer(&server_config);
    if (ret != 0) {
        logError("Failed to initialize server");
        return EXIT_FAILURE;
    }

    logInfo("Server initialized, starting main loop");

    /* Initialize authentication system */
    if (authInit("/devcontainer/web-app/etc/auth/passwd") != 0) {
        fprintf(stderr, "Failed to initialize auth system\n");
        return 1;
    }

    /* Validate a user */
    if (authValidateUser("admin", "password123") == 0) {
        printf("User authenticated successfully\n");
    } else {
        printf("Authentication failed\n");
    }

    /* Initialize audit logging */
    if (auditInit("/var/log/webapp/audit.log") != 0) {
        logError("Failed to initialize audit logging");
        return 1;
    }

    /* Main server loop */
    while (running) {
        ret = runServer();
        if (ret != 0) {
            logError("Server error occurred");
            break;
        }
    }

    logInfo("Server shutdown complete");
    return EXIT_SUCCESS;
}

static void cleanupHandler(int signum)
{
    logInfo("Cleaning up resources...");

    stopServer();    /* This will call cleanupSslContext() */
    dbCleanup(&db_handle);
    sessionCleanup();
    auditCleanup();
    logInfo("Audit system shutdown complete");
    logCleanup();
}

static void signalHandler(int sig)
{
    running = 0;
    auditCleanup();
    exit(sig);
}

static int initializeSubsystems(const struct Config *config)
{
    /* Initialize logging first */
    if (logInit(config->log_path) != 0) {
        fprintf(stderr, "Failed to initialize logging\n");
        return -1;
    }

    /* Initialize audit system second */
    if (auditInit(config->audit_path) != 0) {
        logError("Failed to initialize audit system");
        return -1;
    }

    /* Initialize database */
    if (dbInit(&db_handle, config->db_path) != 0) {
        logError("Failed to initialize database");
        return -1;
    }

    /* Set locale from config */
    if (!setlocale(LC_ALL, config->locale)) {
        logError("Failed to set locale to %s", config->locale);
        return -1;
    }

    /* Initialize logging with configured level */
    if (logInit(config->log_file,
                strcmp(config->log_level, "DEBUG") == 0 ? LOG_LEVEL_DEBUG :
                strcmp(config->log_level, "INFO") == 0 ? LOG_LEVEL_INFO :
                strcmp(config->log_level, "WARN") == 0 ? LOG_LEVEL_WARN :
                LOG_LEVEL_ERROR) != 0) {
        fprintf(stderr, "Failed to initialize logging\n");
        return -1;
    }

    logInfo("Logging initialized with level %s", config->log_level);

    /* Initialize database with configured file */
    if (dbInit(config->db_file, &db_handle) != 0) {
        logError("Failed to initialize database");
        return -1;
    }

    /* Set up server config */
    struct ServerConfig server_config = {
        .port = config->server_port,
        .ssl_cert = config->ssl_cert_file,
        .ssl_key = config->ssl_key_file,
        .db_file = config->db_file,
        .max_streams = config->quic_max_streams,
        .quic_timeout = config->quic_timeout_ms
    };

    if (initServer(&server_config) != 0) {
        logError("Failed to initialize server");
        return -1;
    }

    /* Initialize other subsystems */
    if (otherInit() != 0) {
        struct Response *response = dsCreateResponse(HTTP_INTERNAL_ERROR);
        if (response) {
            dsAddResponseHeader(response, "Content-Type", "text/plain");
            response->body = strdup("Initialization failed");
            response->body_length = strlen(response->body);
            logError("Failed to initialize subsystems: %s", response->body);
            dsFreeResponse(response);
        }
        return -1;
    }

    return 0;
}

static int createRequiredDirectories(void)
{
    const char *dirs[] = {
        "/etc/log",
        "/etc/db",
        "/etc/auth",
        "/etc/ssl",
        "/etc/env"
    };
    size_t i;
    size_t num_dirs = sizeof(dirs) / sizeof(dirs[0]);

    for (i = 0; i < num_dirs; i++) {
        if (createDirectoryIfNotExists(dirs[i]) != 0) {
            fprintf(stderr, "Failed to create directory: %s\n", dirs[i]);
            return -1;
        }
    }

    return 0;
}
