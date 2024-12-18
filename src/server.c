/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* filepath: src/server.c */

#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 500
#define __BSD_VISIBLE 1
#define _DEFAULT_SOURCE

#include "../include/server.h"
#include "../include/bearssl_wrapper.h"

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

/* Move static functions to top */
static int sock_read(void *ctx, unsigned char *buf, size_t len);
static int sock_write(void *ctx, const unsigned char *buf, size_t len);
static int load_key_data(struct server_context *ctx);
static int load_cert_data(struct server_context *ctx);
static int handle_client_connection(br_sslio_context *ioc, int client_fd);

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

/* Define static response strings */
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
struct server_context g_ssl_ctx;  /* Global SSL context */
struct server_context ssl_ctx;  /* Global SSL context */
unsigned short g_bound_port = 0;

int
server_init(void)
{
    int server_fd;
    int opt;
    struct sockaddr_in address;
    socklen_t addrlen;
    struct stat st;

    /* Initialize with error value */
    server_fd = -1;
    opt = 1;

    /* Initialize syslog */
    openlog("webserver", LOG_PID|LOG_CONS, LOG_DAEMON);
    syslog(LOG_INFO, "Server starting initialization");

    /* Check directories */
    if (stat(WWW_DIR, &st) != 0 || !S_ISDIR(st.st_mode)) {
        syslog(LOG_ERR, "WWW directory does not exist");
        return -1;
    }

    if (stat(INDEX_FILE, &st) != 0 || !S_ISREG(st.st_mode)) {
        syslog(LOG_ERR, "Index file does not exist");
        return -1;
    }

    /* Create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        syslog(LOG_ERR, "socket failed");
        return -1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)) < 0) {
        syslog(LOG_ERR, "setsockopt failed: %m");
        close(server_fd);
        return -1;
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);  /* Explicit conversion */
    address.sin_port = htons(SERVER_PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        syslog(LOG_ERR, "bind failed: %m");
        close(server_fd);
        return -1;
    }

    /* Get actual bound address */
    addrlen = sizeof(address);
    if (getsockname(server_fd, (struct sockaddr *)&address, &addrlen) == 0) {
        g_bound_port = ntohs(address.sin_port);
        syslog(LOG_INFO, "Bound to port %d", g_bound_port);
    }

    if (init_ssl_ctx(&ssl_ctx) != 0) {
        syslog(LOG_ERR, "SSL context initialization failed");
        close(server_fd);
        return -1;
    }

    if (load_certificates(&ssl_ctx) != 0) {
        syslog(LOG_ERR, "Certificate loading failed");
        cleanup_ssl_ctx(&ssl_ctx);
        close(server_fd);
        return -1;
    }

#if !SKIP_PRIVILEGE_DROP
    if (drop_privileges() != 0) {
        syslog(LOG_ERR, "Failed to drop privileges");
        close(server_fd);
        return -1;
    }
#endif

    if (listen(server_fd, SOMAXCONN) < 0) {
        syslog(LOG_ERR, "listen failed: %m");
        close(server_fd);
        return -1;
    }

    syslog(LOG_INFO, "Server initialized successfully");
    return server_fd;
}

/* Update server_run function */
static int
server_run(int server_fd)
{
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len;
    char client_ip[INET_ADDRSTRLEN];
    br_sslio_context ioc;
    br_ssl_io_read_t read_func = (br_ssl_io_read_t)sock_read;
    br_ssl_io_write_t write_func = (br_ssl_io_write_t)sock_write;

    client_len = sizeof(client_addr);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            syslog(LOG_ERR, "Accept failed: %m");
            continue;
        }

        /* Get client IP */
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));

        /* Validate Cloudflare IP */
        if (!is_cloudflare_ip(client_ip)) {
            syslog(LOG_WARNING, "Rejected non-Cloudflare IP: %s", client_ip);
            close(client_fd);
            continue;
        }

        /* Reset SSL context */
        br_ssl_server_reset(&ssl_ctx.sc);

        /* Initialize BearSSL I/O with wrapper */
        if (br_sslio_init_wrapper(&ioc, &ssl_ctx.sc.eng,
                                read_func, write_func,
                                &client_fd, &client_fd) != 0) {
            close(client_fd);
            continue;
        }

        /* Handle connection */
        if (handle_client_connection(&ioc, client_fd) != 0) {
            close(client_fd);
            continue;
        }

        close(client_fd);
    }
    return 0;
}

/* Add a public wrapper function */
int
run_server(int server_fd)
{
    return server_run(server_fd);
}

int
validate_host(const char *request)
{
    const char *host_start;
    const char *host_end;
    size_t host_len;
    char host[HOST_BUFFER_SIZE];

    if (request == NULL) {
        return 0;
    }

    /* Find Host header */
    host_start = strstr(request, "Host:");
    if (host_start == NULL) {
        return 0;
    }

    /* Skip "Host:" prefix and any whitespace */
    host_start += 5;
    while (*host_start == ' ') {
        host_start++;
    }

    /* Find end of line */
    host_end = strstr(host_start, "\r\n");
    if (host_end == NULL) {
        return 0;
    }

    /* Extract host value */
    host_len = (size_t)(host_end - host_start);
    if (host_len >= HOST_BUFFER_SIZE || host_len == 0) {
        return 0;
    }

    memcpy(host, host_start, host_len);
    host[host_len] = '\0';

    /* Validate against allowed hosts */
    return (strcmp(host, VALID_HOST) == 0 ||
            strcmp(host, VALID_WWW_HOST) == 0 ||
            strcmp(host, "localhost") == 0 ||
            strcmp(host, "127.0.0.1") == 0);
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

    if (inet_pton(AF_INET, ip_addr, &addr) != 1) {
        return 0;
    }

    ip = ntohl(addr.s_addr);

    for (i = 0; i < CLOUDFLARE_IP_RANGES_COUNT; i++) {
        strncpy(network, cloudflare_ip_ranges[i], sizeof(network) - 1);
        network[sizeof(network) - 1] = '\0';

        slash = strchr(network, '/');
        if (slash == NULL) {
            continue;
        }

        *slash = '\0';
        prefix = atoi(slash + 1);

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
    uint16_t suites_buf[2];
    const uint16_t *suites = suites_buf;

    if (ctx == NULL) {
        return -1;
    }

    /* Clear context */
    memset(ctx, 0, sizeof(*ctx));

    /* Set cipher suites */
    suites_buf[0] = BR_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256;
    suites_buf[1] = BR_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384;

    /* Initialize server core using wrapper */
    br_ssl_server_init_full_rsa_wrapper(&ctx->sc, ctx->chain, MAX_CHAIN_LENGTH,
                                       &ctx->private_key);

    /* Set minimum TLS version using wrapper */
    br_ssl_engine_set_versions_wrapper(&ctx->sc.eng, TLS_MIN_VERSION, BR_TLS12);

    /* Set buffer using wrapper */
    br_ssl_engine_set_buffer_wrapper(&ctx->sc.eng, ctx->iobuf, BUFFER_SIZE, 1);

    /* Set cipher suites using wrapper */
    br_ssl_engine_set_suites_wrapper(&ctx->sc.eng, suites, 2);

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

    if (!ctx || !buf || len == 0) {
        return -1;
    }

    fd = *(int *)ctx;
    rlen = read(fd, buf, len);

    if (rlen <= 0) {
        if (rlen == 0) return -1;  /* EOF */
        if (errno == EINTR) return 0;  /* Retry */
        return -1;
    }

    return (int)rlen;
}

static int
sock_write(void *ctx, const unsigned char *buf, size_t len)
{
    int fd;
    ssize_t wlen;

    if (!ctx || !buf || len == 0) {
        return -1;
    }

    fd = *(int *)ctx;
    wlen = write(fd, buf, len);

    if (wlen <= 0) {
        if (errno == EINTR) return 0;  /* Retry */
        return -1;
    }

    return (int)wlen;
}

static int
handle_client_connection(br_sslio_context *ioc, int client_fd)
{
    char buffer[BUFFER_SIZE];
    size_t len;
    int html_fd;
    int result;
    ssize_t read_len;

    (void)client_fd;

    /* Use wrapper for handshake */
    if (br_ssl_do_handshake_wrapper(ioc) != 0) {
        return -1;
    }

    /* Use wrapper for read */
    len = (size_t)br_ssl_read_all_wrapper(ioc, buffer, sizeof(buffer) - 1);
    if (len <= 0) {
        return -1;
    }
    buffer[len] = '\0';

    /* Validate host */
    if (!validate_host(buffer)) {
        br_ssl_write_all_wrapper(ioc, response_400, strlen(response_400));
        br_ssl_flush_wrapper(ioc);
        return -1;
    }

    /* Use wrapper for write */
    result = br_ssl_write_all_wrapper(ioc, response_header, strlen(response_header));
    if (result < 0) {
        return -1;
    }

    /* Send file content */
    html_fd = open(INDEX_FILE, O_RDONLY);
    if (html_fd < 0) {
        return -1;
    }

    while ((read_len = read(html_fd, buffer, sizeof(buffer))) > 0) {
        result = br_ssl_write_all_wrapper(ioc, buffer, (size_t)read_len);
        if (result < 0) {
            close(html_fd);
            return -1;
        }
    }

    close(html_fd);
    br_ssl_flush_wrapper(ioc);

    return 0;
}
