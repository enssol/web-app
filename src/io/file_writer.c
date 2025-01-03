#include "../../include/io/file_writer.h"
#include "../../include/constant.h"
#include <stdio.h>
#include <sys/file.h>
#include <errno.h>

int
write_file_with_lock(const char *filepath, const char *data)
{
    FILE *fp;
    int ret = ERR_NONE;

    if (!filepath || !data) {
        return ERR_PARAM;
    }

    fp = fopen(filepath, "w");
    if (!fp) {
        return ERR_IO;
    }

    if (flock(fileno(fp), LOCK_EX) != 0) {
        fclose(fp);
        return ERR_IO;
    }

    if (fputs(data, fp) == EOF) {
        ret = ERR_IO;
    }

    flock(fileno(fp), LOCK_UN);
    fclose(fp);
    return ret;
}

int
append_file_with_lock(const char *filepath, const char *data)
{
    FILE *fp;
    int ret = ERR_NONE;

    if (!filepath || !data) {
        return ERR_PARAM;
    }

    fp = fopen(filepath, "a");
    if (!fp) {
        return ERR_IO;
    }

    if (flock(fileno(fp), LOCK_EX) != 0) {
        fclose(fp);
        return ERR_IO;
    }

    if (fputs(data, fp) == EOF || fputc('\n', fp) == EOF) {
        ret = ERR_IO;
    }

    flock(fileno(fp), LOCK_UN);
    fclose(fp);
    return ret;
}
