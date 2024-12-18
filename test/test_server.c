/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* filepath: /devcontainer/web-app/test/test_server.c */

/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

/* Project headers */
#include "../include/server.h"

/* Function prototypes */
static void test_server_init(void);
static int init_suite(void);
static int clean_suite(void);
int register_server_tests(void);

/* Test fixtures */
static const char *TEST_DIR = "test/www";
static int server_fd = -1;

/* Test setup/cleanup */
static int
init_suite(void)
{
    FILE *fp;
    char cwd[1024];
    const char *index_content = "<html><body>Test</body></html>";
    const char *cert_content = "-----BEGIN CERTIFICATE-----\nMIIDJDummy\n-----END CERTIFICATE-----\n";
    const char *key_content = "-----BEGIN PRIVATE KEY-----\nMIIDJDummy\n-----END PRIVATE KEY-----\n";
    mode_t dir_mode = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH; /* 755 */
    mode_t file_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; /* 644 */

    /* Log working directory */
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        fprintf(stderr, "Debug: Current working directory: %s\n", cwd);
    }

    /* Clean any existing test directories first */
    clean_suite();

    /* Create test directory structure with explicit permissions */
    fprintf(stderr, "Creating test directories...\n");

    /* Ensure parent directories exist with proper permissions */
    if (mkdir("test", dir_mode) != 0 && errno != EEXIST) {
        fprintf(stderr, "Failed to create test directory: %s\n", strerror(errno));
        return -1;
    }

    if (mkdir("test/www", dir_mode) != 0 && errno != EEXIST) {
        fprintf(stderr, "Failed to create www directory: %s\n", strerror(errno));
        rmdir("test");
        return -1;
    }

    if (mkdir("test/ssl", dir_mode) != 0 && errno != EEXIST) {
        fprintf(stderr, "Failed to create ssl directory: %s\n", strerror(errno));
        rmdir("test/www");
        rmdir("test");
        return -1;
    }

    /* Create test files with proper permissions */
    if ((fp = fopen(INDEX_FILE, "w")) == NULL) {
        fprintf(stderr, "Failed to create index file: %s\n", strerror(errno));
        goto cleanup;
    }
    fprintf(fp, "%s", index_content);
    fclose(fp);
    chmod(INDEX_FILE, file_mode);

    if ((fp = fopen(SSL_CERT_FILE, "w")) == NULL) {
        fprintf(stderr, "Failed to create cert file: %s\n", strerror(errno));
        goto cleanup;
    }
    fprintf(fp, "%s", cert_content);
    fclose(fp);
    chmod(SSL_CERT_FILE, file_mode);

    if ((fp = fopen(SSL_KEY_FILE, "w")) == NULL) {
        fprintf(stderr, "Failed to create key file: %s\n", strerror(errno));
        goto cleanup;
    }
    fprintf(fp, "%s", key_content);
    fclose(fp);
    chmod(SSL_KEY_FILE, file_mode);

    /* Verify files exist and are accessible */
    if (access(INDEX_FILE, R_OK) != 0) {
        fprintf(stderr, "Index file not accessible: %s\n", strerror(errno));
        goto cleanup;
    }
    if (access(SSL_CERT_FILE, R_OK) != 0) {
        fprintf(stderr, "Certificate file not accessible: %s\n", strerror(errno));
        goto cleanup;
    }
    if (access(SSL_KEY_FILE, R_OK) != 0) {
        fprintf(stderr, "Key file not accessible: %s\n", strerror(errno));
        goto cleanup;
    }

#ifdef DEBUG
    fprintf(stderr, "Debug: Created directories:\n");
    fprintf(stderr, "  WWW_DIR = %s\n", WWW_DIR);
    fprintf(stderr, "  SSL dir = test/ssl\n");
    fprintf(stderr, "  INDEX_FILE = %s\n", INDEX_FILE);
#endif

    fprintf(stderr, "Test suite initialization complete\n");
    return 0;

cleanup:
    unlink(INDEX_FILE);
    unlink(SSL_CERT_FILE);
    unlink(SSL_KEY_FILE);
    rmdir("test/ssl");
    rmdir("test/www");
    rmdir("test");
    return -1;
}

static int
clean_suite(void)
{
    /* Clean up test files */
    unlink(INDEX_FILE);
    unlink(SSL_CERT_FILE);
    unlink(SSL_KEY_FILE);
    rmdir("test/ssl");
    rmdir(TEST_DIR);
    rmdir("test");

    if (server_fd >= 0) {
        server_cleanup(server_fd);
        server_fd = -1;
    }

    return 0;
}

/* Test cases */
static void
test_server_init(void)
{
    int port;

    /* Initialize server */
    server_fd = server_init();

    if (server_fd < 0) {
        fprintf(stderr, "Server init failed with errno: %s\n", strerror(errno));
    }

    CU_ASSERT(server_fd >= 0);

    if (server_fd >= 0) {
        /* Test port binding */
        port = server_get_port(server_fd);
        CU_ASSERT(port > 0);

        /* Cleanup */
        server_cleanup(server_fd);
        server_fd = -1;
    }
}

/* Test suite registration */
int
register_server_tests(void)
{
    CU_pSuite suite;

    suite = CU_add_suite("Server Test Suite", init_suite, clean_suite);
    if (suite == NULL) {
        return -1;
    }

    if (CU_add_test(suite, "Test Server Init", test_server_init) == NULL) {
        return -1;
    }

    return 0;
}

/* Main entry point */
int
main(void)
{
    /* Initialize CUnit test registry */
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    /* Register tests */
    if (register_server_tests() != 0) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run tests */
    CU_basic_run_tests();

    /* Cleanup and return */
    CU_cleanup_registry();
    return CU_get_error();
}
