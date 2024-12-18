/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* filepath: /devcontainer/web-app/include/server.h */
#ifndef SERVER_H
#define SERVER_H

/* System feature test macros must come first */
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 500
#define __BSD_VISIBLE 1
#define _DEFAULT_SOURCE

#include "bearssl_wrapper.h"

/* Standard C headers */
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>

/* Security Constants */
#define TLS_MIN_VERSION BR_TLS12    /* Minimum TLS 1.2 */
#define MAX_CHAIN_LENGTH 2          /* Root + Server cert */
#define MAX_CERT_SIZE 4096          /* Reasonable cert size limit */
#define BUFFER_SIZE BR_SSL_BUFSIZE_BIDI
#define HOST_BUFFER_SIZE 256
#define CLOUDFLARE_IP_RANGES_COUNT 15

/* Server Configuration */
#ifdef TEST_BUILD
#define SERVER_PORT 8443
#else
#define SERVER_PORT 443
#endif

#define WWW_DIR "/opt/webserver/www"
#define INDEX_FILE "/opt/webserver/www/index.html"

/* SSL Paths */
#define SSL_CERT_FILE "/opt/webserver/ssl/cert.pem"
#define SSL_KEY_FILE "/opt/webserver/ssl/key.pem"
#define SSL_CA_FILE "/opt/webserver/ssl/origin_ca_rsa_root.pem"

/* Valid Hosts */
#define VALID_HOST "enssol.com.au"
#define VALID_WWW_HOST "www.enssol.com.au"

/* Server Context */
struct server_context {
    br_ssl_server_context sc;           /* BearSSL context */
    br_x509_certificate chain[2];       /* Cert chain */
    br_rsa_private_key private_key;     /* Private key structure */
    unsigned char iobuf[BUFFER_SIZE];   /* IO buffer */
    uint8_t *cert_data;                 /* Certificate data */
    uint8_t *key_data;                  /* Key data */
    size_t cert_data_len;               /* Certificate length */
    size_t key_data_len;                /* Key length */
    br_x509_minimal_context xc;         /* X509 context */
};

/* Function Prototypes */

/* Server lifecycle */
int server_init(void);
void server_cleanup(int server_fd);
int server_get_port(int server_fd);
int run_server(int server_fd);

/* Security validation */
int validate_host(const char *host);
int drop_privileges(void);
int is_cloudflare_ip(const char *ip_addr);

/* SSL/TLS functions */
int init_ssl_ctx(struct server_context *ctx);
void cleanup_ssl_ctx(struct server_context *ctx);
int load_certificates(struct server_context *ctx);
int ssl_init(struct server_context *ctx);
int ssl_handshake(br_sslio_context *ioc);

/* External variables */
extern unsigned short g_bound_port;
extern const char *cloudflare_ip_ranges[CLOUDFLARE_IP_RANGES_COUNT];

/* Add forward declaration */
struct server_context;
extern struct server_context ssl_ctx;

#endif /* SERVER_H */
