/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

/* filepath: include/web_server.h */
#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <stddef.h>
#include <stdio.h>

/* Constants */
#define MAX_BUFFER_SIZE 4096
#define AUTH_FILE "./etc/auth.passwd"
#define DEFAULT_PORT 8080
#define WWW_ROOT "./www"
#define ACTION_LOGIN "User login"
#define ACTION_LOGOUT "User logout"
#define ACTION_VIEW_PROFILE "Viewed profile"
#define ACTION_UPDATE_PROFILE "Updated profile"
#define ACTION_VIEW_PROJECT "Viewed project"
#define ACTION_MANAGE_USERS "Managed users"
#define UNUSED(x) ((void)(x))
#define LOG_MAX_SIZE (10 * 1024 * 1024) /* 10MB */
#define LOG_MAX_FILES 5
#define LOG_DIR "var/log"
#define LOG_SUFFIX ".log"
#define LOG_ARCHIVE_SUFFIX ".old"

/* Error codes */
#define ERR_NONE 0      /* No error */
#define ERR_AUTH -1     /* Authentication error */
#define ERR_IO -2       /* I/O error */
#define ERR_PARAM -3    /* Invalid parameter */
#define ERR_PERM -4     /* Permission denied */
#define ERR_NOTFOUND -5 /* Not found */
#define ERR_INTERNAL -6 /* Internal error */

/* Standard severity levels (aligned with syslog) */
#define LOG_NONE    0   /* No logging */
#define LOG_ERROR  -1   /* Error conditions */
#define LOG_INFO    1   /* Informational messages */
#define LOG_WARN    2   /* Warning conditions */
#define LOG_DEBUG   3   /* Debug messages */
#define LOG_FATAL  -2   /* Fatal conditions */

/* Web-specific log types */
#define LOG_AUTH    10  /* Authentication related */
#define LOG_ACCESS  11  /* Access related */
#define LOG_SYSTEM  12  /* System related */
#define LOG_AUDIT   13  /* Audit trail */
#define LOG_CONFIG  14  /* Configuration related */
#define LOG_SECURE  15  /* Security related */

/* CRUD endpoints */
#define ENDPOINT_CREATE "/create_record"
#define ENDPOINT_UPDATE "/update_record"
#define ENDPOINT_DELETE "/delete_record"
#define ENDPOINT_READ "/var/records/scjv.rec"
#define ENDPOINT_NEXT_NUMBER "/get_next_number"
#define OBLIGATION_NUMBER_FILE "/var/records/next_number.txt"
#define RECORDS_DIR "/home/appuser/fork-web-app/var/records"


struct user_entry {
    char username[64];
    char password[64];
    int uid;
    int gid;
    char fullname[128];
    char homedir[128];
    char shell[64];
    int is_admin;
};

/* Function prototypes */
int check_auth(const char *username, const char *password);
void parse_query_string(const char *query, char *username, char *password);
int handle_client(int client_socket, const char *www_root);
int setup_server(int port);
int handle_update_user(int client_socket, const char *username, const char *fullname, const char *email, const char *project);
int handle_users_request(int client_socket);
int parse_auth_file(const char *filename, struct user_entry *entries, size_t max_entries);
int log_message(int level, const char *username, const char *action, const char *msg);
int handle_create_record(int client_socket, const char *data);
int handle_update_record(int client_socket, const char *data);
int handle_delete_record(int client_socket, const char *data);
int update_record_in_file(FILE *fp, const char *data);
int delete_record_from_file(FILE *fp, const char *obligation_number);
int rotate_log(const char *logname);
int check_log_size(const char *filename);
int create_record_in_file(const char *data);
int handle_next_number(int client_socket);
int get_next_obligation_number(void);

#endif /* WEB_SERVER_H */
