/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#ifndef SSL_H
#define SSL_H

#include <openssl/ssl.h>

// SSL context management
SSL_CTX *createSslContext(const char *cert_path, const char *key_path);

#endif /* SSL_H */
