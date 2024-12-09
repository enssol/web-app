/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include "../include/logger.h"
#include "../include/error_codes.h"

#define AUTH_FILE "etc/auth.passwd"
#define MAX_LINE_LENGTH 256
#define HASH_LENGTH 64

/**
 * \brief Hashes a password using SHA-256.
 *
 * \param password The password to hash.
 * \param hash The resulting hash.
 */
void hashPassword(const char *password, char *hash)
{
    EVP_MD_CTX    *mdctx;
    unsigned char  md_value[EVP_MAX_MD_SIZE];
    unsigned int   md_len;
    unsigned int   i;

    mdctx = EVP_MD_CTX_new();
    if (mdctx == NULL)
    {
        logError("Failed to create EVP_MD_CTX");
        return;
    }

    if (EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL) != 1)
    {
        logError("Failed to initialize digest context");
        EVP_MD_CTX_free(mdctx);
        return;
    }

    if (EVP_DigestUpdate(mdctx, password, strlen(password)) != 1)
    {
        logError("Failed to update digest");
        EVP_MD_CTX_free(mdctx);
        return;
    }

    if (EVP_DigestFinal_ex(mdctx, md_value, &md_len) != 1)
    {
        logError("Failed to finalize digest");
        EVP_MD_CTX_free(mdctx);
        return;
    }

    EVP_MD_CTX_free(mdctx);

    for (i = 0; i < md_len; i++)
    {
        sprintf(&hash[i * 2], "%02x", md_value[i]);
    }
}

/**
 * \brief Authenticates a user by comparing the provided password with the stored hash.
 *
 * \param username The username.
 * \param password The password.
 * \return SUCCESS if authentication is successful, otherwise an error code.
 */
int authenticateUser(const char *username, const char *password)
{
    FILE *file;
    char  line[MAX_LINE_LENGTH];
    char  computed_hash[HASH_LENGTH + 1];
    char *stored_username;
    char *stored_password_hash;

    file = fopen(AUTH_FILE, "r");
    if (file == NULL)
    {
        logError("Failed to open auth file: %s", AUTH_FILE);
        return ERROR_FILE_OPEN;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        stored_username      = strtok(line, ":");
        stored_password_hash = strtok(NULL, "\n");

        if (stored_username != NULL && stored_password_hash != NULL && strcmp(username, stored_username) == 0)
        {
            hashPassword(password, computed_hash);
            if (strcmp(stored_password_hash, computed_hash) == 0)
            {
                fclose(file);
                return SUCCESS;
            }
        }
    }

    fclose(file);
    return ERROR_AUTHENTICATION_FAILED;
}
