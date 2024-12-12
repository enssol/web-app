/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#ifndef QUIC_H
#define QUIC_H

#include <stdio.h>
#include <stdlib.h>

// QUIC context structure
typedef struct {
    int initialized;
} quic_context_t;

// Functions
int initializeQuicContext(quic_context_t *ctx);
void cleanupQuicContext(quic_context_t *ctx);

#endif /* QUIC_H */
