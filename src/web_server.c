#include "../include/web_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include <sys/file.h>
#include <time.h>
#include <string.h>

int
log_audit(const char *username, const char *action)
{
    FILE *fp;
    time_t now;
    char timestamp[32];
    int ret;

    if (!username || !action) {
        return -1;
    }

    fp = fopen("var/log/audit.log", "a");
    if (!fp) {
        return -1;
    }

    /* Get timestamp */
    now = time(NULL);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S",
            localtime(&now));

    /* Lock file for writing */
    ret = flock(fileno(fp), LOCK_EX);
    if (ret == 0) {
        fprintf(fp, "%s|%s|%s\n", timestamp, username, action);
        flock(fileno(fp), LOCK_UN);
    }

    fclose(fp);
    return ret;
}


void
parse_query_string(const char *query, char *username, char *password)
{
    char *param;
    char *value;
    char *query_copy;
    char *saveptr1;

    username[0] = '\0';
    password[0] = '\0';

    query_copy = strdup(query);
    if (!query_copy) {
        return;
    }

    param = strtok_r(query_copy, "&", &saveptr1);
    while (param) {
        value = strchr(param, '=');
        if (value) {
            *value = '\0';
            value++;

            if (strcmp(param, "username") == 0) {
                strncpy(username, value, 255);
                username[255] = '\0';
            } else if (strcmp(param, "password") == 0) {
                strncpy(password, value, 255);
                password[255] = '\0';
            }
        }
        param = strtok_r(NULL, "&", &saveptr1);
    }

    free(query_copy);
}

int
setup_server(int port)
{
    struct sockaddr_in server_addr;
    int server_socket;
    const int enable = 1;

    /* Create socket */
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        return -1;
    }

    /* Set socket options */
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR,
                   &enable, sizeof(int)) < 0) {
        close(server_socket);
        return -1;
    }

    /* Configure server address */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons((unsigned short)port);

    /* Bind socket */
    if (bind(server_socket, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0) {
        close(server_socket);
        return -1;
    }

    /* Listen for connections */
    if (listen(server_socket, 10) < 0) {
        close(server_socket);
        return -1;
    }

    return server_socket;
}