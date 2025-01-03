#include "../../include/logging/log_writer.h"
#include "../../include/logging/log_rotator.h"
#include "../../include/constant.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/file.h>
#include <limits.h>
#include <errno.h>

int
log_record_operation(const char *operation, const char *data)
{
    return log_message(LOG_INFO, "system", operation, data);
}

int
log_message(int severity, const char *username, const char *action, const char *message)
{
    FILE *fp;
    time_t now;
    char timestamp[32];
    char logpath[PATH_MAX];
    int ret;

    /* Validate parameters */
    if (!username || !action || !message) {
        return ERR_PARAM;
    }

    /* Format timestamp */
    now = time(NULL);
    if (strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S",
                localtime(&now)) == 0) {
        return ERR_INTERNAL;
    }

    /* Check log size and rotate if needed */
    snprintf(logpath, sizeof(logpath), "%s/web%s", LOG_DIR, LOG_SUFFIX);
    if (check_log_size(logpath)) {
        ret = rotate_log("web");
        if (ret != ERR_NONE) {
            return ret;
        }
    }

    /* Open log file in append mode */
    fp = fopen(logpath, "a");
    if (!fp) {
        return ERR_IO;
    }

    /* Write log entry */
    fprintf(fp, "%s|%d|%s|%s|%s\n",
            timestamp, severity, username, action, message);

    fclose(fp);
    return ERR_NONE;
}

int
log_audit(const char *username, const char *action)
{
    FILE *fp;
    time_t now;
    char timestamp[32];
    char logpath[PATH_MAX];
    int ret;

    if (!username || !action) {
        return -1;
    }

    /* Construct log path */
    snprintf(logpath, sizeof(logpath), "%s/audit%s", LOG_DIR, LOG_SUFFIX);

    /* Check log size and rotate if needed */
    if (check_log_size(logpath) == 1) {
        ret = rotate_log("audit");
        if (ret != ERR_NONE) {
            return ret;
        }
    }

    /* Open log file with exclusive lock */
    fp = fopen(logpath, "a");
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
