/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#ifndef BEARSSL_WRAPPER_H
#define BEARSSL_WRAPPER_H

/* Allow non-C90 features in this file only */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeclaration-after-statement"
#pragma GCC diagnostic ignored "-Winline"
#pragma GCC diagnostic ignored "-Wc90-c99-compat"
#pragma GCC diagnostic ignored "-Wpedantic"

/* Feature test macros must come first */
#if !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 500
#define __BSD_VISIBLE 1
#define _DEFAULT_SOURCE

#include <sys/types.h>

/* BearSSL headers */
#include <bearssl.h>
#include <bearssl_ssl.h>   /* This includes the I/O functions */
#include <bearssl_x509.h>
#include <bearssl_aead.h>
#include <bearssl_rand.h>
#include <bearssl_hmac.h>
#include <bearssl_hash.h>
#include <bearssl_pem.h>

#pragma GCC diagnostic pop

/* Update the function pointer type definitions */
typedef int (*br_ssl_io_read_t)(void *ctx, unsigned char *buf, size_t len);
typedef int (*br_ssl_io_write_t)(void *ctx, const unsigned char *buf, size_t len);

/* Helper types that match BearSSL's internal function pointer types */
typedef int (*br_io_read_fn)(void *ctx, unsigned char *buf, size_t len);
typedef int (*br_io_write_fn)(void *ctx, const unsigned char *buf, size_t len);

/* Function prototypes for wrapped BearSSL functions */
uint32_t br_ssl_engine_get_flags_wrapper(const br_ssl_engine_context *cc);
void br_ssl_engine_set_buffer_wrapper(br_ssl_engine_context *cc,
                                    unsigned char *buf, size_t len, int bidi);
void br_ssl_engine_set_versions_wrapper(br_ssl_engine_context *cc,
                                      unsigned version_min, unsigned version_max);
void br_ssl_engine_set_suites_wrapper(br_ssl_engine_context *cc,
                                     const uint16_t *suites, size_t len);

/* SSL Server functions */
void br_ssl_server_init_full_rsa_wrapper(br_ssl_server_context *cc,
                                        const br_x509_certificate *chain,
                                        size_t chain_len,
                                        const br_rsa_private_key *sk);

/* Add missing prototype */
void br_ssl_init_full_rsa_wrapper(br_ssl_server_context *cc,
                                 const br_x509_certificate *chain,
                                 size_t chain_len,
                                 const br_rsa_private_key *sk);

/* SSL I/O functions */
int br_sslio_init_wrapper(br_sslio_context *ctx,
                         br_ssl_engine_context *engine,
                         br_ssl_io_read_t read_function,
                         br_ssl_io_write_t write_function,
                         void *read_context,
                         void *write_context);

int br_ssl_do_handshake_wrapper(br_sslio_context *ioc);
int br_ssl_write_all_wrapper(br_sslio_context *ioc, const void *data, size_t len);
int br_ssl_read_all_wrapper(br_sslio_context *ioc, void *data, size_t len);
void br_ssl_flush_wrapper(br_sslio_context *ioc);

#endif /* BEARSSL_WRAPPER_H */
