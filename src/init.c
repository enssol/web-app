/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* filepath: src/init.c */
#define _GNU_SOURCE         /* Required for chroot() */
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>         /* For chroot() */
#include <limits.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include "../include/init.h"
#include "../include/app_error.h"
#include "../include/fs.h"
#include "../include/shell.h"

/* Function prototypes */
static int check_root_permissions(void);
static int initialize_subsystems(void);
static int validate_filesystem(void);
static void signal_handler(int signo);

/* System state */
static enum system_state current_state = STATE_STARTUP;
static char root_directory[PATH_MAX];

/* Directory structure */
static const char *system_dirs[] = {
    "bin",              /* Essential binaries */
    "sbin",             /* System binaries */
    "etc",              /* System configuration */
    "etc/config",       /* Application configs */
    "dev",              /* Device files */
    "tmp",              /* Temporary files */
    "var",              /* Variable data */
    "var/log",          /* Log files */
    "var/run",          /* Runtime data */
    "var/spool",        /* Spool directories */
    "usr",              /* User programs */
    "usr/bin",          /* Non-essential binaries */
    "usr/lib",          /* Libraries */
    "usr/include",      /* Header files */
    "home",             /* User home directories */
    "mnt",              /* Mount points */
    NULL                /* Sentinel */
};

/* Required configuration files */
static const char *required_files[] = {
    "etc/system.conf",  /* System configuration */
    "etc/.env",         /* Environment variables */
    "var/log/app.log",  /* Application log */
    "etc/mime.types",   /* MIME type mappings */
    NULL                /* Terminator */
};

/* Check if running as root */
static int
check_root_permissions(void)
{
    if (geteuid() != 0) {
        set_error(ERR_PERMISSION, "Must run as root");
        return -1;
    }
    return 0;
}

/* Initialize core subsystems */
static int
initialize_subsystems(void)
{
    struct sigaction sa;
    int i;
    mode_t dir_mode;

    /* Set up signal handlers */
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1 ||
        sigaction(SIGTERM, &sa, NULL) == -1) {
        set_error(ERR_SYSTEM, "Failed to set up signal handlers");
        return -1;
    }

    /* Create system directories */
    dir_mode = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;

    for (i = 0; system_dirs[i] != NULL; i++) {
        if (mkdir(system_dirs[i], dir_mode) == -1 && errno != EEXIST) {
            set_error(ERR_FILESYSTEM, "Failed to create directory");
            return -1;
        }
    }

    return 0;
}

/* Validate filesystem structure */
static int
validate_filesystem(void)
{
    struct stat st;
    int i;

    for (i = 0; system_dirs[i] != NULL; i++) {
        if (stat(system_dirs[i], &st) == -1) {
            set_error(ERR_FILESYSTEM, "Directory validation failed");
            return -1;
        }
        if (!S_ISDIR(st.st_mode)) {
            set_error(ERR_FILESYSTEM, "Path is not a directory");
            return -1;
        }
    }

    return 0;
}

/* Signal handler */
static void
signal_handler(int signo)
{
    if (signo == SIGTERM || signo == SIGINT) {
        current_state = STATE_SHUTDOWN;
    }
}

/* Initialize the system */
int
init_system(const char *root_dir)
{
    if (root_dir == NULL) {
        set_error(ERR_INVALID_ARG, "Root directory path is NULL");
        return -1;
    }

    if (strlen(root_dir) >= PATH_MAX) {
        set_error(ERR_INVALID_ARG, "Root directory path too long");
        return -1;
    }

    /* Store root directory */
    strncpy(root_directory, root_dir, PATH_MAX - 1);
    root_directory[PATH_MAX - 1] = '\0';

    /* Check permissions */
    if (check_root_permissions() == -1) {
        return -1;
    }

    /* Change to root directory */
    if (chdir(root_directory) == -1) {
        set_error(ERR_FILESYSTEM, "Failed to change directory");
        return -1;
    }

    /* Initialize subsystems */
    if (initialize_subsystems() == -1) {
        return -1;
    }

    /* Validate filesystem */
    if (validate_filesystem() == -1) {
        return -1;
    }

    /* Update system state */
    current_state = STATE_RUNNING;

    return 0;
}

/* Get current system state */
enum system_state
get_system_state(void)
{
    return current_state;
}

/* Shutdown the system */
int
shutdownSystem(void)
{
    if (current_state != STATE_RUNNING) {
        return INIT_ERROR;
    }

    /* Cleanup subsystems */
    fsCleanup();
    current_state = STATE_SHUTDOWN;

    return INIT_SUCCESS;
}
