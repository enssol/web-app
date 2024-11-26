/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

typedef struct {
    char app_name[256];
    // Other configuration fields
} config_t;

int load_ini_config(const char *filename);
int load_conf_config(const char *filename);

#endif // CONFIG_LOADER_H
