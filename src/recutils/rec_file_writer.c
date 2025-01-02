/* filepath: /home/appuser/web-app/src/recutils/rec_file_writer.c */
#include "../../include/recutils/rec_file_writer.h"
#include "../../include/constant.h"
#include "../../include/logging/log_writer.h"
#include <sys/stat.h>

int
create_record_in_file(const char *data)
{
    FILE *fp;
    int ret = ERR_NONE;
    char filepath[PATH_MAX];

    /* Input validation */
    if (!data) {
        return ERR_PARAM;
    }

    /* Use RECORDS_DIR constant */
    if (snprintf(filepath, sizeof(filepath), "%s/%s", RECORDS_DIR, ENDPOINT_READ) < 0) {
        return ERR_INTERNAL;
    }

    /* Create records directory if needed */
    if (mkdir(RECORDS_DIR, 0755) < 0 && errno != EEXIST) {
        return ERR_IO;
    }

    /* Open file with exclusive lock */
    fp = fopen(filepath, "a");
    if (!fp) {
        return ERR_IO;
    }

    /* Get exclusive lock */
    if (flock(fileno(fp), LOCK_EX) == 0) {
        if (fprintf(fp, "%s\n", data) < 0) {
            ret = ERR_IO;
        }
        flock(fileno(fp), LOCK_UN);
    } else {
        ret = ERR_IO;
    }

    fclose(fp);
    return ret;
}

int
update_record_in_file(FILE *fp, const char *data)
{
    /* Input validation */
    if (!fp || !data) {
        return ERR_PARAM;
    }

    /* Get exclusive lock */
    if (flock(fileno(fp), LOCK_EX) != 0) {
        return ERR_IO;
    }

    /* Write data */
    if (fprintf(fp, "%s\n", data) < 0) {
        flock(fileno(fp), LOCK_UN);
        return ERR_IO;
    }

    /* Release lock */
    flock(fileno(fp), LOCK_UN);
    return ERR_NONE;
}
