#include "checksum_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/md5.h>

/* Function to read file in binary mode */
static int read_file(const char *filename, unsigned char **buffer, size_t *size)
{
    FILE *file;
    long file_size;
    size_t read_size;

    file = fopen(filename, "rb");
    if (file == NULL) {
        return -1;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return -1;
    }

    file_size = ftell(file);
    if (file_size < 0) {
        fclose(file);
        return -1;
    }

    if (fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        return -1;
    }

    *buffer = (unsigned char *)malloc(file_size);
    if (*buffer == NULL) {
        fclose(file);
        return -1;
    }

    read_size = fread(*buffer, 1, file_size, file);
    if (read_size != file_size) {
        free(*buffer);
        fclose(file);
        return -1;
    }

    *size = file_size;
    fclose(file);
    return 0;
}

/* Function to calculate SHA256 checksum */
int calculate_sha256(const char *filename, unsigned char *digest)
{
    unsigned char *buffer;
    size_t size;
    SHA256_CTX sha256;

    if (read_file(filename, &buffer, &size) != 0) {
        return -1;
    }

    SHA256_Init(&sha256);
    SHA256_Update(&sha256, buffer, size);
    SHA256_Final(digest, &sha256);

    free(buffer);
    return 0;
}

/* Function to calculate MD5 checksum */
int calculate_md5(const char *filename, unsigned char *digest)
{
    unsigned char *buffer;
    size_t size;
    MD5_CTX md5;

    if (read_file(filename, &buffer, &size) != 0) {
        return -1;
    }

    MD5_Init(&md5);
    MD5_Update(&md5, buffer, size);
    MD5_Final(digest, &md5);

    free(buffer);
    return 0;
}

/* Function to compare checksums */
int compare_checksums(const unsigned char *digest1, const unsigned char *digest2, size_t length)
{
    return memcmp(digest1, digest2, length) == 0 ? 0 : -1;
}
