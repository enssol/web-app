/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* filepath: /home/appuser/web-app/test/test_runner.c */
#include <CUnit/Basic.h>
#include "../include/web_server.h"
#include "test_suites.h"

int
main(void)
{
    CU_pSuite web_server_suite;
    CU_pSuite web_server_security_suite;

    /* Initialize CUnit registry */
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    /* Create web server suite */
    web_server_suite = CU_add_suite("Web Server Tests", NULL, NULL);
    if (web_server_suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Create security suite */
    web_server_security_suite = CU_add_suite("Web Server Security Tests",
                                           web_server_suite_init,
                                           web_server_suite_cleanup);
    if (web_server_security_suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Initialize test suites */
    if (init_web_server_suite(web_server_suite) != 0) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (init_web_server_security_suite(web_server_security_suite) != 0) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run tests */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
