/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#include "file_io.h"

int readFile(const char *path, char *buffer, size_t size)
{
    FILE *file = fopen(path, "r");
    if (!file) return -1;
    fread(buffer, 1, size, file);
    fclose(file);
    return 0;
}

int writeFile(const char *path, const char *data)
{
    FILE *file = fopen(path, "w");
    if (!file) return -1;
    fprintf(file, "%s", data);
    fclose(file);
    return 0;
}
