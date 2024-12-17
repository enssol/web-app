/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

/* src/main.c */
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/init.h"
#include "../include/app_error.h"
#include "../include/cache.h"
#include "../include/constants.h"
#include "../include/http_parser.h"
#include "../include/http_response.h"
#include "../include/logging.h"
#include "../include/tcp.h"
#include "../include/fs.h"
#include "../include/env.h"
#include "../include/config.h"
#include "../include/mem.h"
#include "../include/mime.h"
#include "../include/net.h"
#include "../include/process.h"
#include "../include/scheduler.h"
#include "../include/shell.h"
#include "../include/socket.h"

/* Essential directories - relative paths */
static const char *dirs[] = {
    "/var",
    "/var/log",
    "/var/run",
    "/etc",
    "/etc/config",
    NULL
};

/* Required configuration files */
static const char *files[] = {
    "/var/log/app.log",
    "/etc/config.conf",
    "/etc/.env",
    NULL
};

/* Add helper function prototypes */
static int createDirectory(const char *base, const char *path);
static int createFile(const char *base, const char *path);
static int setupFs(const char *root);

#define HTTP_PORT 8080
#define HTTP_MAX_CONNECTIONS 1024
#define HTTP_BACKLOG 128
#define ROOT_PATH "/tmp/enssol-root"
#define MAX_PATH 4096
#define WEB_ROOT "/var/www" /* Add this line */

/* Forward declare the sendFileResponse function */
static int sendFileResponse(struct tcp_connection *conn, const char *path, size_t size);

static volatile sig_atomic_t running = 1;

/* Function prototypes */
static void sigHandler(int signo);
static int initializeSubsystems(const char *log_path);
static void cleanupSubsystems(void);
static int initHttpServer(void);
static int handleHttpConnection(struct socket_info *client_socket);
static void *handleHttpConnectionWrapper(void *arg);
static int setupFs(const char *root);

static void
sigHandler(int signo)
{
    if (signo == SIGINT || signo == SIGTERM)
    {
        running = 0;
    }
}

/* Helper function implementations */
static int
createDirectory(const char *base, const char *path)
{
    char full_path[MAX_PATH];

    if (snprintf(full_path, sizeof(full_path), "%s%s", base, path)
        >= (int)sizeof(full_path)) {
        return -1;
    }

    if (mkdir(full_path, 0755) != 0 && errno != EEXIST) {
        return -1;
    }

    return 0;
}

static int
createFile(const char *base, const char *path)
{
    char full_path[MAX_PATH];
    int fd;

    if (snprintf(full_path, sizeof(full_path), "%s%s", base, path)
        >= (int)sizeof(full_path)) {
        return -1;
    }

    fd = open(full_path, O_WRONLY | O_CREAT, 0644);
    if (fd < 0) {
        return -1;
    }

    close(fd);
    return 0;
}

static int
initializeSubsystems(const char *log_path)
{
    int status;
    struct sigaction sa;

    /* Initialize filesystem */
    status = setupFs(WEB_ROOT);
    if (status != 0) {
        return -1;
    }

    /* Initialize system first */
    if ((status = initSystem(log_path)) != INIT_SUCCESS) {
        fprintf(stderr, "Error: Failed to initialize system\n");
        return -1;
    }

    /* Set up signal handlers */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigHandler;
    if (sigaction(SIGINT, &sa, NULL) != 0 || sigaction(SIGTERM, &sa, NULL) != 0)
    {
        fprintf(stderr, "Error: Failed to set up signal handlers\n");
        return -1;
    }

    /* Initialize logging */
    if ((status = logInit(log_path, LOG_FLAG_TIMESTAMP | LOG_FLAG_LEVEL)) != 0)
    {
        fprintf(stderr, "Error: Failed to initialize logging\n");
        return -1;
    }

    /* Initialize scheduler */
    if ((status = scheduler_init()) != SCHEDULER_SUCCESS)
    {
        fprintf(stderr, "Error: Failed to initialize scheduler\n");
        logCleanup();
        return -1;
    }

    /* Initialize memory management */
    if ((status = memInit(MEM_POOL_SIZE)) != MEM_SUCCESS)
    {
        fprintf(stderr, "Error: Failed to initialize memory management\n");
        scheduler_stop();
        logCleanup();
        return -1;
    }

    /* Initialize network subsystem */
    if ((status = netInit()) != 0)
    {
        fprintf(stderr, "Error: Failed to initialize network subsystem\n");
        memCleanup();
        scheduler_stop();
        logCleanup();
        return -1;
    }

    /* Initialize cache */
    if ((status = cacheInit(CACHE_TYPE_LRU, CACHE_MAX_ENTRIES)) != CACHE_SUCCESS)
    {
        fprintf(stderr, "Error: Failed to initialize cache\n");
        netCleanup();
        memCleanup();
        scheduler_stop();
        logCleanup();
        return -1;
    }

    /* Initialize HTTP server */
    if ((status = initHttpServer()) != 0)
    {
        fprintf(stderr, "Error: Failed to initialize HTTP server\n");
        cacheCleanup();
        netCleanup();
        memCleanup();
        scheduler_stop();
        logCleanup();
        return -1;
    }

    return 0;
}

static void
cleanupSubsystems(void)
{
    scheduler_stop();
    cacheCleanup();
    netCleanup();
    memCleanup();
    logCleanup();
}

static int
initHttpServer(void)
{
    int status;
    struct socket_info server_socket;
    struct socket_info *sock_copy;
    struct process task;
    int opt = 1;

    /* Initialize socket */
    status = netCreateSocket(&server_socket, SOCKET_TCP);
    if (status != 0) {
        logWrite(LOG_ERR, "Failed to create server socket");
        return -1;
    }

    /* Set socket options */
    if (setsockopt(server_socket.fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0) {
        logWrite(LOG_ERR, "Failed to set socket options");
        netClose(&server_socket);
        return -1;
    }

    /* Bind to port */
    status = netBind(&server_socket, "0.0.0.0", HTTP_PORT);
    if (status != 0) {
        logWrite(LOG_ERR, "Failed to bind server socket");
        netClose(&server_socket);
        return -1;
    }

    /* Listen for connections */
    status = netListen(&server_socket);
    if (status != 0) {
        logWrite(LOG_ERR, "Failed to listen on server socket");
        netClose(&server_socket);
        return -1;
    }

    /* Create copy of socket info for task */
    sock_copy = malloc(sizeof(struct socket_info));
    if (sock_copy == NULL) {
        logWrite(LOG_ERR, "Failed to allocate memory for socket");
        netClose(&server_socket);
        return -1;
    }
    memcpy(sock_copy, &server_socket, sizeof(struct socket_info));

    /* Initialize task structure */
    memset(&task, 0, sizeof(struct process));
    strncpy(task.name, "http_server", MAX_PROCESS_NAME - 1);
    task.name[MAX_PROCESS_NAME - 1] = '\0';
    task.state = PROCESS_CREATED;
    task.priority = PRIORITY_NORMAL;
    task.task = handleHttpConnectionWrapper;
    task.arg = sock_copy;

    /* Add task to scheduler */
    status = scheduler_add_task(&task);
    if (status != SCHEDULER_SUCCESS) {
        logWrite(LOG_ERR, "Failed to register HTTP handler");
        free(sock_copy);
        netClose(&server_socket);
        return -1;
    }

    return 0;
}

/* Handle HTTP connection */
static int
handleHttpConnection(struct socket_info *client_socket)
{
    int status;
    char file_path[MAX_PATH_LEN];
    size_t file_size;
    struct http_request request;
    struct http_response response;
    struct tcp_connection tcp_conn;

    /* Initialize TCP connection from socket */
    tcp_conn.sockfd = client_socket->fd;
    tcp_conn.state = TCP_STATE_CONNECTED;
    memcpy(&tcp_conn.addr, &client_socket->addr, sizeof(struct sockaddr_in));

    /* Parse HTTP request */
    status = httpParseRequest(&tcp_conn, &request);
    if (status != 0) {
        return -1;
    }

    /* Build file path from request */
    if (snprintf(file_path, sizeof(file_path), "%s%s",
                WEB_ROOT, request.path) >= (int)sizeof(file_path)) {
        return -1;
    }

    /* Get file size */
    status = fsGetFileSize(file_path, &file_size);
    if (status != 0) {
        /* File not found - return 404 */
        httpResponseCreate(&response, HTTP_STATUS_NOT_FOUND);
        httpResponseSend(&tcp_conn, &response);
        return -1;
    }

    /* Read and send file content */
    return sendFileResponse(&tcp_conn, file_path, file_size);
}

static void *
handleHttpConnectionWrapper(void *arg)
{
    struct socket_info *client_socket = (struct socket_info *)arg;

    if (client_socket != NULL) {
        handleHttpConnection(client_socket);
        netClose(client_socket);
        free(client_socket);
    }

    return NULL;
}

static int
setupFs(const char *root)
{
    if (root == NULL) {
        return -1;
    }
    return fsInit(root);
}

static int
sendFileResponse(struct tcp_connection *conn, const char *path, size_t size)
{
    char buffer[TCP_BUFFER_SIZE];
    struct http_response response;
    const char *mime_type;
    FILE *fp;
    size_t bytes_read;
    int status;

    /* Parameter validation */
    if (conn == NULL || path == NULL) {
        logWrite(LOG_ERR, "Invalid parameters in sendFileResponse");
        return -1;
    }

    /* Create HTTP response */
    status = httpResponseCreate(&response, HTTP_STATUS_OK);
    if (status != 0) {
        logWrite(LOG_ERR, "Failed to create HTTP response");
        return -1;
    }

    /* Set Content-Type based on file extension */
    mime_type = mimeGetType(path);
    status = httpResponseAddHeader(&response, "Content-Type", mime_type);
    if (status != 0) {
        logWrite(LOG_ERR, "Failed to add Content-Type header");
        return -1;
    }

    /* Add Content-Length header */
    if (snprintf(buffer, sizeof(buffer), "%lu", (unsigned long)size) < 0) {
        logWrite(LOG_ERR, "Failed to format Content-Length");
        return -1;
    }
    status = httpResponseAddHeader(&response, "Content-Length", buffer);
    if (status != 0) {
        logWrite(LOG_ERR, "Failed to add Content-Length header");
        return -1;
    }

    /* Send headers */
    status = httpResponseSend(conn, &response);
    if (status != 0) {
        logWrite(LOG_ERR, "Failed to send HTTP headers");
        return -1;
    }

    /* Open and send file */
    fp = fopen(path, "rb");
    if (fp == NULL) {
        logWrite(LOG_ERR, "Failed to open file: %s", path);
        return -1;
    }

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        if (tcpSend(conn, buffer, bytes_read) != (ssize_t)bytes_read) {
            logWrite(LOG_ERR, "Failed to send file data");
            fclose(fp);
            return -1;
        }
    }

    if (ferror(fp)) {
        logWrite(LOG_ERR, "Error reading file: %s", path);
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 0;
}

int
main(int argc, char *argv[])
{
    const char *log_path;
    struct sigaction sa;
    size_t i;
    int status;
    char full_path[MAX_PATH];

    /* Check arguments */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <log_path>\n", argv[0]);
        return 1;
    }

    log_path = argv[1];

    /* Setup signal handlers */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigHandler;
    if (sigaction(SIGINT, &sa, NULL) == -1 ||
        sigaction(SIGTERM, &sa, NULL) == -1) {
        fprintf(stderr, "Failed to setup signal handlers\n");
        return 1;
    }

    /* Create root directory */
    if (mkdir(ROOT_PATH, 0755) != 0 && errno != EEXIST) {
        fprintf(stderr, "Failed to create root directory: %s\n",
                strerror(errno));
        return 1;
    }

    /* Create essential directories */
    for (i = 0; dirs[i] != NULL; i++) {
        if (createDirectory(ROOT_PATH, dirs[i]) != 0) {
            fprintf(stderr, "Failed to create directory %s: %s\n",
                    dirs[i], strerror(errno));
            return 1;
        }
    }

    /* Create essential files */
    for (i = 0; files[i] != NULL; i++) {
        if (createFile(ROOT_PATH, files[i]) != 0) {
            fprintf(stderr, "Failed to create file %s: %s\n",
                    files[i], strerror(errno));
            return 1;
        }
    }

    /* Initialize system */
    if (snprintf(full_path, sizeof(full_path), "%s%s", ROOT_PATH, log_path)
        >= (int)sizeof(full_path)) {
        fprintf(stderr, "Log path too long\n");
        return 1;
    }

    status = initSystem(ROOT_PATH);
    if (status != INIT_SUCCESS) {
        fprintf(stderr, "System initialization failed\n");
        return 1;
    }

    /* Initialize error logging */
    errorInit(log_path);

    /* Initialize signal handlers */
    sa.sa_handler = sigHandler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1 ||
        sigaction(SIGTERM, &sa, NULL) == -1) {
        errorLog(ERROR_CRITICAL, "Failed to set up signal handlers");
        return 1;
    }

    /* Initialize subsystems */
    if (initializeSubsystems(log_path) != 0)
    {
        return EXIT_FAILURE;
    }

    /* Start scheduler */
    if ((status = scheduler_start()) != SCHEDULER_SUCCESS)
    {
        fprintf(stderr, "Error: Failed to start scheduler\n");
        cleanupSubsystems();
        return EXIT_FAILURE;
    }

    /* Main loop */
    while (running)
    {
        shellPrompt();
    }

    /* Cleanup */
    cleanupSubsystems();
    return EXIT_SUCCESS;
}
