/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#include "app.h"
#include "server.h"

int main(int argc, char *argv[]) {
    loadEnvironmentVariables(".env");
    initializeServer();
    return 0;
}
