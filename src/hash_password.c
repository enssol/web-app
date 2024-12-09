/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#define HASH_LENGTH 64

void hashPassword(const char *password, char *hash)
{
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, password, strlen(password));
    SHA256_Final(digest, &ctx);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        sprintf(&hash[i * 2], "%02x", digest[i]);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <username> <password>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *username = argv[1];
    const char *password = argv[2];
    char hash[HASH_LENGTH + 1];

    hashPassword(password, hash);

    FILE *file = fopen("etc/auth.passwd", "a");
    if (!file)
    {
        perror("Failed to open auth.passwd file");
        return EXIT_FAILURE;
    }

    fprintf(file, "%s:%s\n", username, hash);
    fclose(file);

    printf("User %s added successfully.\n", username);
    return EXIT_SUCCESS;
}
