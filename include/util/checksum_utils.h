#ifndef CHECKSUM_UTILS_H
#define CHECKSUM_UTILS_H

#include <stddef.h>

/* Constants */
#define SHA256_DIGEST_LENGTH 32
#define MD5_DIGEST_LENGTH 16

/* Function prototypes */
int calculate_sha256(const char *filename, unsigned char *digest);
int calculate_md5(const char *filename, unsigned char *digest);
int compare_checksums(const unsigned char *digest1, const unsigned char *digest2, size_t length);

#endif /* CHECKSUM_UTILS_H */
