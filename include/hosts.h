/* Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */
/* filepath: /devcontainer/web-app/include/hosts.h */
#ifndef HOSTS_H
#define HOSTS_H

#include <sys/types.h>
#include <netinet/in.h>

#define MAX_HOSTS 64
#define MAX_HOST_LENGTH 256
#define MAX_CIDR_BLOCKS 32

struct cidr_block {
    struct in_addr network;
    uint32_t mask;
};

struct known_hosts {
    char hosts[MAX_HOSTS][MAX_HOST_LENGTH];
    size_t host_count;
    struct cidr_block cidrs[MAX_CIDR_BLOCKS];
    size_t cidr_count;
};

/* Function prototypes */
int hosts_init(void);
int hosts_add(const char *host);
int hosts_add_cidr(const char *cidr);
int hosts_check(const char *host);
int hosts_check_ip(const struct in_addr *addr);
void hosts_cleanup(void);

#endif /* HOSTS_H */
