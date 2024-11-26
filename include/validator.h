/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "config_loader.h" // Include the header where config_t is defined

int validate_config(config_t *config, const char *value);
int validate_integer(const char *value);
int validate_double(const char *value);
int validate_string(const char *value);
int validate_boolean(const char *value);

#endif // VALIDATOR_H
