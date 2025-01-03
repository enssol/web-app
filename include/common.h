#ifndef COMMON_H
#define COMMON_H

#include "standards.h"

/* Macros */
#define UNUSED(x) ((void)(x))

/* Global variables */
extern volatile sig_atomic_t server_running;

/* Function prototypes */
void signal_handler(int sig);

#endif /* COMMON_H */
