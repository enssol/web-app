/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include "quic.h"

int initializeQuicContext(quic_context_t *ctx)
{
    if (!ctx) return -1;
    ctx->initialized = 1;
    printf("QUIC context initialized.\n");
    return 0;
}

void cleanupQuicContext(quic_context_t *ctx)
{
    if (ctx && ctx->initialized) {
        ctx->initialized = 0;
        printf("QUIC context cleaned up.\n");
    }
}
