#include "../../include/io/file_reader.h"
#include "../../include/constant.h"
#include <stdio.h>
#include <errno.h>
#include <sys/file.h>

int
read_file_with_lock(const char *filepath, char *buffer, size_t size)
{
    FILE *fp;
    size_t bytes_read;
    int ret = ERR_NONE;

    if (!filepath || !buffer || size == 0) {
        return ERR_PARAM;
    }

    fp = fopen(filepath, "r");
    if (!fp) {
        return ERR_IO;
    }

    if (flock(fileno(fp), LOCK_SH) != 0) {
        fclose(fp);
        return ERR_IO;
    }

    bytes_read = fread(buffer, 1, size - 1, fp);
    if (ferror(fp)) {
        ret = ERR_IO;
    } else {
        buffer[bytes_read] = '\0';
    }

    flock(fileno(fp), LOCK_UN);
    fclose(fp);
    return ret;
}

long
get_file_size(const char *filepath)
{
    FILE *fp;
    long size;

    if (!filepath) {
        return -1;
    }

    fp = fopen(filepath, "r");
    if (!fp) {
        return -1;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return -1;
    }

    size = ftell(fp);
    fclose(fp);
    return size;
}
