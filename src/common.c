#include "../include/common.h"

/* Global variables */
volatile sig_atomic_t server_running = 1;

/* Function implementations */
void
signal_handler(int sig)
{
    if (sig == SIGTERM || sig == SIGINT) {
        server_running = 0;
    }
}
