/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
#ifndef INIT_H
#define INIT_H

/* System states */
enum system_state {
    STATE_STARTUP,
    STATE_RUNNING,
    STATE_SHUTDOWN,
    STATE_ERROR
};

/* Return codes */
#define INIT_SUCCESS 0
#define INIT_ERROR  -1
#define INIT_ERR_ALREADY_RUNNING -2
#define INIT_ERR_DIR_CREATE -3
#define INIT_ERR_FILE_CREATE -4
#define INIT_ERR_CHROOT -5

/* Function prototypes */
int initSystem(const char *root_path);
int shutdownSystem(void);
enum system_state getSystemState(void);

#endif /* INIT_H */
