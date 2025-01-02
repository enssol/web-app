#ifndef CONSTANT_H
#define CONSTANT_H

/* System constants */
#define MAX_BUFFER_SIZE 4096
#define DEFAULT_PORT 8080

/* Path constants */
#define AUTH_FILE "./config/auth/passwd"
#define RECORDS_DIR "./var/records"
#define SCJV_RECORD "./var/records/trans-scjv.rec"
#define MS1180_RECORD "./var/records/trans-ms1180.rec"
#define W6946_RECORD "./var/records/trans-w6946.rec"
#define SCHEMA_DESC "./var/records/schema-v1.desc"
#define OBLIGATION_NUMBER_FILE "./var/records/obligation_number.txt"

/* Web path constants */
#define WWW_ROOT "./public"
#define VIEWS_DIR "./public/views"
#define PAGES_DIR "./public/views/pages"

/* Log paths */
#define LOG_DIR "./var/log"
#define WEB_LOG_DIR "./var/log/web"
#define APP_LOG_DIR "./var/log/app"
#define SECURITY_LOG_DIR "./var/log/security"
#define SYSTEM_LOG_DIR "./var/log/system"

#define LOG_SUFFIX "-YYYY-MM-DD.log"
#define LOG_ARCHIVE_SUFFIX ".archive"

/* Logging constants */
#define LOG_MAX_SIZE (10 * 1024 * 1024) /* 10MB */
#define LOG_MAX_FILES 5

/* Action types */
#define ACTION_LOGIN "User login"
#define ACTION_LOGOUT "User logout"
#define ACTION_VIEW_PROFILE "Viewed profile"
#define ACTION_UPDATE_PROFILE "Updated profile"
#define ACTION_VIEW_PROJECT "Viewed project"
#define ACTION_MANAGE_USERS "Managed users"

/* API endpoints */
#define ENDPOINT_CREATE "/create_record"
#define ENDPOINT_UPDATE "/update_record"
#define ENDPOINT_NEXT_NUMBER "/get_next_number"

/* Error codes */
#define ERR_NONE 0      /* No error */
#define ERR_AUTH -1     /* Authentication error */
#define ERR_IO -2       /* I/O error */
#define ERR_PARAM -3    /* Invalid parameter */
#define ERR_PERM -4     /* Permission denied */
#define ERR_NOTFOUND -5 /* Not found */
#define ERR_INTERNAL -6 /* Internal error */

/* Log levels */
#define LOG_NONE    0   /* No logging */
#define LOG_ERROR  -1   /* Error conditions */
#define LOG_INFO    1   /* Informational messages */
#define LOG_WARN    2   /* Warning conditions */
#define LOG_DEBUG   3   /* Debug messages */
#define LOG_FATAL  -2   /* Fatal conditions */

/* Log types */
#define LOG_AUTH    10  /* Authentication related */
#define LOG_ACCESS  11  /* Access related */
#define LOG_SYSTEM  12  /* System related */
#define LOG_AUDIT   13  /* Audit trail */
#define LOG_CONFIG  14  /* Configuration related */
#define LOG_SECURE  15  /* Security related */

#endif /* CONSTANT_H */
