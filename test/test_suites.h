/* filepath: /home/appuser/web-app/test/test_suites.h */
/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#ifndef TEST_SUITES_H
#define TEST_SUITES_H

#include <CUnit/Basic.h>

/* Constants */
#define BUFFER_SIZE 1024
#define TEST_PORT 8081

/* Test suite initialization and cleanup functions */
int web_server_suite_init(void);
int web_server_suite_cleanup(void);

/* Test suite setup functions */
int init_web_server_suite(CU_pSuite suite);
int init_web_server_security_suite(CU_pSuite suite);

/* Test case functions */
void test_server_setup(void);
void test_server_accept(void);
void test_server_response(void);

/* Suite getter function */
CU_pSuite get_web_server_suite(void);

#endif /* TEST_SUITES_H */
