/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* filepath: test/test_server.c */

#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 500
#define __BSD_VISIBLE 1
#define _DEFAULT_SOURCE

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../include/server.h"
#include "../include/bearssl_wrapper.h"

/* Test fixture data */
static const char test_www_dir[] = "test/www";
static const char test_index_file[] = "test/www/index.html";
static const char test_ssl_cert[] = "test/ssl/cert.pem";
static const char test_ssl_key[] = "test/ssl/key.pem";
static const char test_ssl_ca[] __attribute__((unused)) = "test/ssl/origin_ca_rsa_root.pem";
static int server_fd = -1;
static struct server_context test_ctx;

/* Mock SSL context for testing */
static unsigned char test_buffer[BUFFER_SIZE];

/* Test setup and teardown functions */
static int
init_suite(void)
{
    FILE *fp;
    int ret;
    const char *test_html;
    const char *dirs[2];
    size_t i;
    struct stat st;

    /* Initialize variables */
    dirs[0] = test_www_dir;
    dirs[1] = "test/ssl";
    test_html = "<html><body>Test Page</body></html>\n";

    /* Add error logging */
    if (mkdir("test", 0755) != 0 && errno != EEXIST) {
        return -1;
    }

    /* Create test directories */
    for (i = 0; i < sizeof(dirs) / sizeof(dirs[0]); i++) {
        if (stat(dirs[i], &st) != 0) {
            ret = mkdir(dirs[i], 0755);
            if (ret != 0) {
                return -1;
            }
        }
    }

    /* Create test files */
    fp = fopen(test_index_file, "w");
    if (fp == NULL) {
        return -1;
    }
    fprintf(fp, "%s", test_html);
    fclose(fp);

    /* Write test certificates */
    fp = fopen(test_ssl_cert, "w");
    if (fp == NULL) {
        return -1;
    }
    fprintf(fp, "-----BEGIN CERTIFICATE-----\nTEST CERT\n-----END CERTIFICATE-----\n");
    fclose(fp);

    fp = fopen(test_ssl_key, "w");
    if (fp == NULL) {
        return -1;
    }
    fprintf(fp, "-----BEGIN PRIVATE KEY-----\nTEST KEY\n-----END PRIVATE KEY-----\n");
    fclose(fp);

    /* Initialize test SSL context */
    memset(&test_ctx, 0, sizeof(test_ctx));
    memset(test_buffer, 0, sizeof(test_buffer));

    return 0;
}

static int
clean_suite(void)
{
    if (server_fd >= 0) {
        server_cleanup(server_fd);
        server_fd = -1;
    }

    cleanup_ssl_ctx(&test_ctx);

    unlink(test_index_file);
    unlink(test_ssl_cert);
    unlink(test_ssl_key);
    rmdir(test_www_dir);
    rmdir("test/ssl");

    return 0;
}

/* Test functions */
static void
test_server_init(void)
{
    server_fd = server_init();
    CU_ASSERT(server_fd >= 0);

    if (server_fd >= 0) {
        int port = server_get_port(server_fd);
        CU_ASSERT(port > 0);
        server_cleanup(server_fd);
        server_fd = -1;
    }
}

/* Update test_ssl_context_init function */
static void
test_ssl_context_init(void)
{
    uint16_t suites_buf[2];
    uint16_t *suites;
    int result;

    suites = suites_buf;
    suites_buf[0] = BR_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256;
    suites_buf[1] = BR_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384;

    /* Remove unused variables */
    memset(&test_ctx, 0, sizeof(test_ctx));

    /* Test SSL context initialization */
    result = init_ssl_ctx(&test_ctx);
    CU_ASSERT_EQUAL(result, 0);

    if (result == 0) {
        /* Verify SSL context setup */
        uint32_t flags = br_ssl_engine_get_flags_wrapper(&test_ctx.sc.eng);
        CU_ASSERT(flags != 0);

        /* Test buffer setup */
        br_ssl_engine_set_buffer_wrapper(&test_ctx.sc.eng, test_buffer,
                                       sizeof(test_buffer), 1);
        CU_ASSERT_PTR_NOT_NULL(test_ctx.sc.eng.ibuf);

        /* Test version setup */
        br_ssl_engine_set_versions_wrapper(&test_ctx.sc.eng,
                                         TLS_MIN_VERSION, BR_TLS12);
        CU_ASSERT_EQUAL(test_ctx.sc.eng.version_min, TLS_MIN_VERSION);
        CU_ASSERT_EQUAL(test_ctx.sc.eng.version_max, BR_TLS12);

        /* Test cipher suites setup */
        br_ssl_engine_set_suites_wrapper(&test_ctx.sc.eng, suites, 2);
        CU_ASSERT_PTR_NOT_NULL(test_ctx.sc.eng.suites_buf);
    }
}

static void
test_validate_host(void)
{
    const char *valid_requests[] = {
        "GET / HTTP/1.1\r\nHost: enssol.com.au\r\n\r\n",
        "GET / HTTP/1.1\r\nHost: www.enssol.com.au\r\n\r\n",
        "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n",
        "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n"
    };

    const char *invalid_requests[] = {
        "GET / HTTP/1.1\r\nHost: invalid.com\r\n\r\n",
        "GET / HTTP/1.1\r\n\r\n",
        "GET / HTTP/1.1\r\nHost:\r\n\r\n"
    };

    size_t i;

    for (i = 0; i < sizeof(valid_requests) / sizeof(valid_requests[0]); i++) {
        CU_ASSERT_EQUAL(validate_host(valid_requests[i]), 1);
    }

    for (i = 0; i < sizeof(invalid_requests) / sizeof(invalid_requests[0]); i++) {
        CU_ASSERT_EQUAL(validate_host(invalid_requests[i]), 0);
    }

    CU_ASSERT_EQUAL(validate_host(NULL), 0);
}

static void
test_cloudflare_ip_validation(void)
{
    const char *valid_ips[] = {
        "173.245.48.1",
        "103.21.244.1",
        "104.16.0.1",
        "172.64.0.1",
        "131.0.72.1"
    };

    const char *invalid_ips[] = {
        "192.168.1.1",
        "10.0.0.1",
        "172.16.0.1",
        "invalid",
        ""
    };

    size_t i;

    for (i = 0; i < sizeof(valid_ips) / sizeof(valid_ips[0]); i++) {
        CU_ASSERT_EQUAL(is_cloudflare_ip(valid_ips[i]), 1);
    }

    for (i = 0; i < sizeof(invalid_ips) / sizeof(invalid_ips[0]); i++) {
        CU_ASSERT_EQUAL(is_cloudflare_ip(invalid_ips[i]), 0);
    }

    CU_ASSERT_EQUAL(is_cloudflare_ip(NULL), 0);
}

/* Test runner */
int
main(void)
{
    CU_pSuite pSuite;
    int failures;

    pSuite = NULL;
    failures = 0;

    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    pSuite = CU_add_suite("Server Test Suite", init_suite, clean_suite);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((CU_add_test(pSuite, "Test Server Init", test_server_init) == NULL) ||
        (CU_add_test(pSuite, "Test SSL Context Init", test_ssl_context_init) == NULL) ||
        (CU_add_test(pSuite, "Test Host Validation", test_validate_host) == NULL) ||
        (CU_add_test(pSuite, "Test Cloudflare IP Validation",
                     test_cloudflare_ip_validation) == NULL)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    failures = (int)CU_get_number_of_failures();
    CU_cleanup_registry();

    return failures > 0 ? 1 : 0;
}
