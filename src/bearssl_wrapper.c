/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

/* System feature test macros must come first */
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 500
#define __BSD_VISIBLE 1
#define _DEFAULT_SOURCE

#include "../include/bearssl_wrapper.h"
#include <bearssl_ssl.h>

/* Remove br_ssl_init_full function as it's incomplete */

int
br_sslio_init_wrapper(br_sslio_context *ctx,
                     br_ssl_engine_context *engine,
                     br_ssl_io_read_t read_function,
                     br_ssl_io_write_t write_function,
                     void *read_context,
                     void *write_context)
{
    /* Define function pointer types explicitly to match BearSSL's expectations */
    br_io_read_fn read_fn;
    br_io_write_fn write_fn;

    /* Cast the function pointers using the helper types */
    read_fn = (br_io_read_fn)read_function;
    write_fn = (br_io_write_fn)write_function;

    /* Initialize BearSSL I/O context with proper argument order */
    br_sslio_init(ctx, engine, read_fn, write_fn, read_context, write_context);
    return 0;
}

int
br_ssl_do_handshake_wrapper(br_sslio_context *ioc)
{
    return br_sslio_flush(ioc);
}

int
br_ssl_write_all_wrapper(br_sslio_context *ioc, const void *data, size_t len)
{
    return br_sslio_write_all(ioc, data, len);
}

int
br_ssl_read_all_wrapper(br_sslio_context *ioc, void *data, size_t len)
{
    return br_sslio_read(ioc, data, len);
}

void
br_ssl_flush_wrapper(br_sslio_context *ioc)
{
    br_sslio_flush(ioc);
}

uint32_t
br_ssl_engine_get_flags_wrapper(const br_ssl_engine_context *cc)
{
    return br_ssl_engine_get_flags((br_ssl_engine_context *)cc);
}

void
br_ssl_engine_set_buffer_wrapper(br_ssl_engine_context *cc,
                                unsigned char *buf, size_t len, int bidi)
{
    br_ssl_engine_set_buffer(cc, buf, len, bidi);
}

void
br_ssl_engine_set_versions_wrapper(br_ssl_engine_context *cc,
                                 unsigned version_min, unsigned version_max)
{
    br_ssl_engine_set_versions(cc, version_min, version_max);
}

void
br_ssl_engine_set_suites_wrapper(br_ssl_engine_context *cc,
                                const uint16_t *suites, size_t len)
{
    br_ssl_engine_set_suites(cc, suites, len);
}

void
br_ssl_server_init_full_rsa_wrapper(br_ssl_server_context *cc,
                                   const br_x509_certificate *chain,
                                   size_t chain_len,
                                   const br_rsa_private_key *sk)
{
    br_ssl_server_init_full_rsa(cc, chain, chain_len, sk);
}

/* Replace the function implementation */
void
br_ssl_init_full_rsa_wrapper(br_ssl_server_context *cc,
                            const br_x509_certificate *chain,
                            size_t chain_len,
                            const br_rsa_private_key *sk)
{
    br_ssl_server_init_full_rsa(cc, chain, chain_len, sk);
}
