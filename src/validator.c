/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "validator.h"
#include "logger.h"

int validate_config(const char *key, const char *value) {
    if (strcmp(key, "app_name") == 0) {
        if (validate_string(value) != 0) {
            return -1; // Invalid value
        }
        strcpy(config.app_name, value);
    } else if (strcmp(key, "version") == 0) {
        if (validate_double(value) != 0) {
            return -1; // Invalid value
        }
        config.version = atof(value);
    } else if (strcmp(key, "log_level") == 0) {
        if (strcmp(value, "INFO") != 0 && strcmp(value, "DEBUG") != 0 && strcmp(value, "ERROR") != 0) {
            return -1; // Invalid value
        }
        strcpy(config.log_level, value);
    } else if (strcmp(key, "enable_feature_x") == 0) {
        if (validate_boolean(value) != 0) {
            return -1; // Invalid value
        }
        config.enable_feature_x = strcmp(value, "true") == 0;
    } else if (strcmp(key, "enable_feature_y") == 0) {
        if (validate_boolean(value) != 0) {
            return -1; // Invalid value
        }
        config.enable_feature_y = strcmp(value, "true") == 0;
    } else if (strcmp(key, "max_retries") == 0) {
        if (validate_integer(value) != 0) {
            return -1; // Invalid value
        }
        config.max_retries = atoi(value);
    } else if (strcmp(key, "app_mode") == 0) {
        if (validate_string(value) != 0) {
            return -1; // Invalid value
        }
        strcpy(config.app_mode, value);
    } else if (strcmp(key, "enable_feature_z") == 0) {
        if (validate_boolean(value) != 0) {
            return -1; // Invalid value
        }
        config.enable_feature_z = strcmp(value, "true") == 0;
    } else {
        return -1; // Unknown key
    }

    return 0; // Valid value
}

int validate_integer(const char *value) {
    for (int i = 0; value[i] != '\0'; i++) {
        if (!isdigit(value[i])) {
            return -1; // Invalid integer
        }
    }
    return 0; // Valid integer
}

int validate_double(const char *value) {
    int dot_count = 0;
    for (int i = 0; value[i] != '\0'; i++) {
        if (!isdigit(value[i])) {
            if (value[i] == '.' && dot_count == 0) {
                dot_count++;
            } else {
                return -1; // Invalid double
            }
        }
    }
    return 0; // Valid double
}

int validate_string(const char *value) {
    if (strlen(value) == 0) {
        return -1; // Invalid string
    }
    return 0; // Valid string
}

int validate_boolean(const char *value) {
    if (strcmp(value, "true") != 0 && strcmp(value, "false") != 0) {
        return -1; // Invalid boolean
    }
    return 0; // Valid boolean
}
