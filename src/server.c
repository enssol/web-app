/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* filepath: src/server.c */

#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 500

#include "../include/server.h"

/* System headers */
#include <arpa/inet.h>    /* for inet_ntop */
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <arpa/inet.h>
#include <stdlib.h>  /* for atoi() */
#include <sys/stat.h>  /* For stat() */

/* Keep only static function prototypes */
static int sock_read(void *ctx, unsigned char *buf, size_t len);
static int sock_write(void *ctx, const unsigned char *buf, size_t len);
static int load_key_data(struct server_context *ctx);
static int load_cert_data(struct server_context *ctx);
static int handle_client_connection(int client_fd, struct server_context *ctx);

/* Add Cloudflare IP ranges */
const char *cloudflare_ip_ranges[CLOUDFLARE_IP_RANGES_COUNT] = {
    "173.245.48.0/20",
    "103.21.244.0/22",
    "103.22.200.0/22",
    "103.31.4.0/22",
    "141.101.64.0/18",
    "108.162.192.0/18",
    "190.93.240.0/20",
    "188.114.96.0/20",
    "197.234.240.0/22",
    "198.41.128.0/17",
    "162.158.0.0/15",
    "104.16.0.0/13",
    "104.24.0.0/14",
    "172.64.0.0/13",
    "131.0.72.0/22"
};

/* Define the const strings declared in header */
const char *response_header =
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "\r\n";

const char *response_400 =
    "HTTP/1.0 400 Bad Request\r\n"
    "Content-Type: text/plain\r\n"
    "\r\n"
    "Invalid Host\r\n";

const char *response_404 =
    "HTTP/1.0 404 Not Found\r\n"
    "Content-Type: text/plain\r\n"
    "\r\n"
    "File Not Found\r\n";

/* Global variables */
struct server_context ssl_ctx;  /* Single SSL context */
unsigned short g_bound_port = 0;

int
server_init(void)
{
    int sock_fd;
    struct sockaddr_in address;
    socklen_t addrlen;
    struct stat st;
    int opt = 1;
    char cwd[1024];

#ifdef DEBUG
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        fprintf(stderr, "Debug: Server init - CWD: %s\n", cwd);
    }
    fprintf(stderr, "Debug: Server init - WWW_DIR: %s\n", WWW_DIR);
    fprintf(stderr, "Debug: Server init - INDEX_FILE: %s\n", INDEX_FILE);
#ifdef TEST_BUILD
    fprintf(stderr, "Debug: Running in TEST_BUILD mode\n");
#else
    fprintf(stderr, "Debug: Running in PRODUCTION mode\n");
#endif
#endif

    /* Check www directory with detailed error reporting */
    if (stat(WWW_DIR, &st) != 0) {
        fprintf(stderr, "WWW directory check failed (%s): %s\n",
                WWW_DIR, strerror(errno));
        return -1;
    }

    if (!S_ISDIR(st.st_mode)) {
        fprintf(stderr, "WWW path is not a directory: %s\n", WWW_DIR);
        return -1;
    }

    /* Check index file with detailed error reporting */
    if (stat(INDEX_FILE, &st) != 0) {
        fprintf(stderr, "Index file check failed (%s): %s\n",
                INDEX_FILE, strerror(errno));
        return -1;
    }

    if (!S_ISREG(st.st_mode)) {
        fprintf(stderr, "Index path is not a regular file: %s\n", INDEX_FILE);
        return -1;
    }

    /* Create socket */
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        fprintf(stderr, "Socket creation failed: %s\n", strerror(errno));
        return -1;
    }

    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)) < 0) {
        syslog(LOG_ERR, "setsockopt failed: %s", strerror(errno));
        close(sock_fd);
        return -1;
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SERVER_PORT);

    if (bind(sock_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        syslog(LOG_ERR, "bind failed: %s", strerror(errno));
        close(sock_fd);
        return -1;
    }

    /* Get bound port */
    addrlen = sizeof(address);
    if (getsockname(sock_fd, (struct sockaddr *)&address, &addrlen) == 0) {
        g_bound_port = ntohs(address.sin_port);
    }

#ifndef TEST_BUILD
    /* Skip SSL initialization in test mode */
    if (init_ssl_ctx(&ssl_ctx) != 0 || load_certificates(&ssl_ctx) != 0) {
        syslog(LOG_ERR, "SSL initialization failed");
        close(sock_fd);
        return -1;
    }
#endif

    if (listen(sock_fd, SOMAXCONN) < 0) {
        syslog(LOG_ERR, "listen failed: %s", strerror(errno));
        close(sock_fd);
        return -1;
    }

    return sock_fd;
}

/* Internal server run implementation */
int
server_run(int server_fd)
{
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len;
    char client_ip[INET_ADDRSTRLEN];

    client_len = sizeof(client_addr);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            syslog(LOG_ERR, "Accept failed: %m");
            continue;
        }

        /* Get client IP */
        if (inet_ntop(AF_INET, &client_addr.sin_addr, client_ip,
                      sizeof(client_ip)) == NULL) {
            close(client_fd);
            continue;
        }

        /* Validate Cloudflare IP */
        if (!is_cloudflare_ip(client_ip)) {
            syslog(LOG_WARNING, "Rejected non-Cloudflare IP: %s", client_ip);
            close(client_fd);
            continue;
        }

        /* Handle connection */
        if (handle_client_connection(client_fd, &ssl_ctx) != 0) {
            syslog(LOG_ERR, "Failed to handle client connection");
        }

        close(client_fd);
    }
    return 0;
}

/* Public wrapper function */
int
run_server(int server_fd)
{
    return server_run(server_fd);
}

int
validate_host(const char *host)
{
    /* Basic input validation */
    if (host == NULL) {
        return 0;
    }

    /* Check against allowed hostnames */
    if (strcmp(host, VALID_HOST) == 0 ||
        strcmp(host, VALID_WWW_HOST) == 0) {
        return 1;
    }

    return 0;
}

int
server_get_port(int server_fd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    if (getsockname(server_fd, (struct sockaddr *)&addr, &len) < 0) {
        return -1;
    }
    return ntohs(addr.sin_port);
}

void
server_cleanup(int server_fd)
{
    if (server_fd >= 0) {
        close(server_fd);
    }
}

int
drop_privileges(void)
{
    struct passwd *pw;

    pw = getpwnam("webserver");
    if (pw == NULL) {
        syslog(LOG_ERR, "getpwnam failed: %m");
        return -1;
    }

    if (setgid(pw->pw_gid) != 0) {
        syslog(LOG_ERR, "setgid failed: %m");
        return -1;
    }

    if (setuid(pw->pw_uid) != 0) {
        syslog(LOG_ERR, "setuid failed: %m");
        return -1;
    }

    return 0;
}

/* Add function to check if IP is from Cloudflare */
int
is_cloudflare_ip(const char *ip_addr)
{
    struct in_addr addr;
    struct in_addr net;
    struct in_addr mask;
    unsigned long ip;
    char network[20];
    char *slash;
    int i;
    int prefix;

    /* Check for NULL input */
    if (ip_addr == NULL) {
        return 0;
    }

    /* Convert IP address */
    if (inet_pton(AF_INET, ip_addr, &addr) != 1) {
        return 0;
    }

    ip = ntohl(addr.s_addr);

    for (i = 0; i < CLOUDFLARE_IP_RANGES_COUNT; i++) {
        /* Bounds check */
        if (cloudflare_ip_ranges[i] == NULL) {
            continue;
        }

        /* Copy network safely */
        strncpy(network, cloudflare_ip_ranges[i], sizeof(network) - 1);
        network[sizeof(network) - 1] = '\0';

        slash = strchr(network, '/');
        if (slash == NULL) {
            continue;
        }

        *slash = '\0';
        prefix = atoi(slash + 1);

        /* Validate prefix range */
        if (prefix < 0 || prefix > 32) {
            continue;
        }

        if (inet_pton(AF_INET, network, &net) != 1) {
            continue;
        }

        mask.s_addr = htonl(0xffffffff << (32 - prefix));

        if ((ip & ntohl(mask.s_addr)) == (ntohl(net.s_addr) & ntohl(mask.s_addr))) {
            return 1;
        }
    }

    return 0;
}

/* Fix init_ssl_ctx function */
int
init_ssl_ctx(struct server_context *ctx)
{
    /* Basic TLS 1.2 setup */
    br_ssl_server_init_full_rsa(&ctx->sc, ctx->chain, 1, &ctx->private_key);
    br_ssl_engine_set_versions(&ctx->sc.eng, BR_TLS12, BR_TLS12);
    br_ssl_engine_set_buffer(&ctx->sc.eng, ctx->iobuf, sizeof(ctx->iobuf), 1);

    return 0;
}

/* Fix load_certificates function */
int
load_certificates(struct server_context *ctx)
{
    FILE *f = NULL;
    long file_size;

    /* Load certificate chain */
    if (!(f = fopen(SSL_CERT_FILE, "rb"))) {
        goto error;
    }

    if (fseek(f, 0, SEEK_END) != 0 || (file_size = ftell(f)) < 0) {
        goto error;
    }

    rewind(f);

    if (!(ctx->cert_data = malloc((size_t)file_size))) {
        goto error;
    }

    if (fread(ctx->cert_data, 1, (size_t)file_size, f) != (size_t)file_size) {
        goto error;
    }

    ctx->cert_data_len = (size_t)file_size;
    fclose(f);
    f = NULL;

    /* Load private key */
    if (!(f = fopen(SSL_KEY_FILE, "rb"))) {
        goto error;
    }

    if (fseek(f, 0, SEEK_END) != 0 || (file_size = ftell(f)) < 0) {
        goto error;
    }

    rewind(f);

    if (!(ctx->key_data = malloc((size_t)file_size))) {
        goto error;
    }

    if (fread(ctx->key_data, 1, (size_t)file_size, f) != (size_t)file_size) {
        goto error;
    }

    ctx->key_data_len = (size_t)file_size;
    fclose(f);

    /* Parse certificate and key */
    if (load_cert_data(ctx) != 0 || load_key_data(ctx) != 0) {
        goto error;
    }

    return 0;

error:
    if (f) fclose(f);
    cleanup_ssl_ctx(ctx);
    return -1;
}

void
cleanup_ssl_ctx(struct server_context *ctx)
{
    if (ctx != NULL) {
        if (ctx->cert_data != NULL) {
            memset(ctx->cert_data, 0, ctx->cert_data_len);
            free(ctx->cert_data);
            ctx->cert_data = NULL;
        }
        if (ctx->key_data != NULL) {
            memset(ctx->key_data, 0, ctx->key_data_len);
            free(ctx->key_data);
            ctx->key_data = NULL;
        }
        memset(ctx->iobuf, 0, sizeof(ctx->iobuf));
        ctx->cert_data_len = 0;
        ctx->key_data_len = 0;
    }
}

static int
load_key_data(struct server_context *ctx)
{
    br_skey_decoder_context dc;
    const br_rsa_private_key *rsa;

    /* Initialize key decoder */
    br_skey_decoder_init(&dc);
    br_skey_decoder_push(&dc, ctx->key_data, ctx->key_data_len);

    /* Get RSA key */
    rsa = br_skey_decoder_get_rsa(&dc);
    if (rsa == NULL) {
        return -1;
    }

    /* Copy key data */
    ctx->private_key = *rsa;

    return 0;
}

static int
load_cert_data(struct server_context *ctx)
{
    br_x509_decoder_context dc;

    /* Initialize certificate decoder */
    br_x509_decoder_init(&dc, 0, 0);
    br_x509_decoder_push(&dc, ctx->cert_data, ctx->cert_data_len);

    /* Check if decoding was successful */
    if (!br_x509_decoder_last_error(&dc)) {
        /* Set up certificate */
        ctx->chain[0].data = ctx->cert_data;
        ctx->chain[0].data_len = ctx->cert_data_len;
        return 0;
    }

    return -1;
}

static int
sock_read(void *ctx, unsigned char *buf, size_t len)
{
    int fd;
    ssize_t rlen;

    fd = *(int *)ctx;
    rlen = read(fd, buf, len);

    if (rlen <= 0) {
        return rlen == 0 ? -1 : 0;
    }
    return (int)rlen;
}

static int
sock_write(void *ctx, const unsigned char *buf, size_t len)
{
    int fd;
    ssize_t wlen;

    fd = *(int *)ctx;
    wlen = write(fd, buf, len);

    if (wlen <= 0) {
        return 0;
    }
    return (int)wlen;
}

static int
handle_client_connection(int client_fd, struct server_context *ctx)
{
    unsigned char *buf;
    ssize_t read_len;
    br_sslio_context ioc;
    int ret;

    /* Reset server context */
    ret = br_ssl_server_reset(&ctx->sc);
    if (ret != 1) {
        return -1;
    }

    /* Initialize I/O wrapper */
    br_sslio_init(&ioc, &ctx->sc.eng, sock_read, &client_fd,
                  sock_write, &client_fd);

    /* Perform handshake */
    if (br_sslio_flush(&ioc) < 0) {
        return -1;
    }

    /* Read request */
    buf = ctx->iobuf;
    read_len = br_sslio_read(&ioc, buf, sizeof(ctx->iobuf) - 1);
    if (read_len < 0) {
        br_sslio_close(&ioc);
        return -1;
    }

    /* Null terminate received data */
    if (read_len > 0) {
        buf[read_len] = '\0';
    } else {
        buf[0] = '\0';
    }

    /* Send response */
    if (br_sslio_write_all(&ioc, response_header, strlen(response_header)) < 0) {
        br_sslio_close(&ioc);
        return -1;
    }

    /* Flush and close properly */
    if (br_sslio_flush(&ioc) < 0) {
        br_sslio_close(&ioc);
        return -1;
    }

    br_sslio_close(&ioc);
    return 0;
}
