/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* filepath: test/test_server.c */
#include <CUnit/Basic.h>
#include "../include/server.h"
#include <errno.h>

static void
test_server_init(void)
{
    int server_fd;

    server_fd = server_init();
    if (server_fd < 0) {
        fprintf(stderr, "server_init failed with errno=%d\n", errno);
    }
    CU_ASSERT(server_fd >= 0);

    if (server_fd >= 0) {
        server_cleanup(server_fd);
    }
}

int
main(void)
{
    CU_pSuite suite = NULL;

    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    suite = CU_add_suite("Server", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(suite, "test_server_init", test_server_init) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
