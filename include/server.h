/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>

/* System paths */
#define INSTALL_DIR "/opt/webserver"
#define WWW_DIR INSTALL_DIR "/www"
#define SSL_DIR INSTALL_DIR "/ssl"
#define LOG_DIR "/var/log/webserver"

/* File paths */
#define SSL_CERT_FILE SSL_DIR "/cert.pem"
#define SSL_KEY_FILE SSL_DIR "/key.pem"
#define ACCESS_LOG LOG_DIR "/access.log"
#define ERROR_LOG LOG_DIR "/error.log"

/* Server constants */
#define MAX_PATH 1024
#define MAX_RESPONSE 4096
#define MAX_BUFFER_SIZE 8192
#define MAX_PENDING 10
#define DEFAULT_PORT 8080
#define DEFAULT_SSL_PORT 443
#define MAX_REQUESTS_PER_MINUTE 60

/* Domain configuration */
#define DEFAULT_DOMAIN "enssol.com.au"
#define DEFAULT_WWW_DOMAIN "www.enssol.com.au"

/* HTTP Status codes */
#define HTTP_OK 200
#define HTTP_NOT_FOUND 404
#define HTTP_INTERNAL_ERROR 500

/* Configuration structure */
struct server_config {
    int port;                    /* Server port */
    int ssl_port;               /* SSL port */
    const char *doc_root;        /* Document root directory */
    const char *cert_file;       /* SSL certificate file */
    const char *key_file;        /* SSL private key file */
    const char *access_log;      /* Access log path */
    const char *error_log;       /* Error log path */
    const char *domain;          /* Primary domain */
    const char *www_domain;      /* WWW domain */
    int use_ssl;                /* Enable SSL */
};

/* HTTP response structure */
struct http_response {
    int status_code;
    char content[MAX_RESPONSE];
    size_t content_length;
    const char *content_type;    /* MIME type */
    char headers[MAX_RESPONSE];
};

/* Function prototypes */
void log_error(const char *message);
int server_init(const struct server_config *config);
int server_process(void);
void server_cleanup(void);
int handle_static_file(const char *path, struct http_response *resp);

#endif /* SERVER_H */
