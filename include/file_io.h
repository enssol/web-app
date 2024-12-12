/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>
#include <stdlib.h>

// File I/O functions
int readFile(const char *path, char *buffer, size_t size);
int writeFile(const char *path, const char *data);

#endif /* FILE_IO_H */
