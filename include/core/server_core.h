#ifndef SERVER_CORE_H
#define SERVER_CORE_H

#include "../standards.h"

/* Core server function declarations */
int server_core_init(void);
int server_core_cleanup(void);
int setup_server(int port);
void server_core_stop(void);

#endif /* SERVER_CORE_H */
