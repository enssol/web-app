#include "../../include/logging/log_rotator.h"
#include "../../include/constant.h"
#include <stdio.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

int
check_log_size(const char *filename)
{
    struct stat st;

    if (stat(filename, &st) < 0) {
        return -1;
    }

    return st.st_size >= LOG_MAX_SIZE;
}

int
rotate_log(const char *logname)
{
    char oldpath[PATH_MAX];
    char newpath[PATH_MAX];
    char logpath[PATH_MAX];
    int i;

    /* Parameter validation */
    if (!logname) {
        return ERR_PARAM;
    }

    /* Construct log directory path based on log type */
    const char *log_dir = LOG_DIR;
    if (strncmp(logname, "web", 3) == 0) {
        log_dir = WEB_LOG_DIR;
    } else if (strncmp(logname, "app", 3) == 0) {
        log_dir = APP_LOG_DIR;
    } else if (strncmp(logname, "security", 8) == 0) {
        log_dir = SECURITY_LOG_DIR;
    } else if (strncmp(logname, "system", 6) == 0) {
        log_dir = SYSTEM_LOG_DIR;
    }

    /* Remove oldest log if exists */
    snprintf(oldpath, sizeof(oldpath), "%s/%s.%d%s",
             log_dir, logname, LOG_MAX_FILES, LOG_ARCHIVE_SUFFIX);
    remove(oldpath);

    /* Rotate existing logs */
    for (i = LOG_MAX_FILES - 1; i > 0; i--) {
        snprintf(oldpath, sizeof(oldpath), "%s/%s.%d%s",
                log_dir, logname, i, LOG_ARCHIVE_SUFFIX);
        snprintf(newpath, sizeof(newpath), "%s/%s.%d%s",
                log_dir, logname, i + 1, LOG_ARCHIVE_SUFFIX);
        rename(oldpath, newpath);
    }

    /* Update path construction */
    snprintf(oldpath, sizeof(oldpath), "%s/%s%s",
             log_dir, logname, LOG_SUFFIX);
    snprintf(newpath, sizeof(newpath), "%s/%s.1%s",
             log_dir, logname, LOG_ARCHIVE_SUFFIX);

    if (rename(oldpath, newpath) != 0) {
        return ERR_IO;
    }

    /* New log paths */
    snprintf(logpath, sizeof(logpath), "%s/web%s", log_dir, LOG_SUFFIX);
    snprintf(logpath, sizeof(logpath), "%s/audit%s", log_dir, LOG_SUFFIX);

    return ERR_NONE;
}
