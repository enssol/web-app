/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */
/* filepath: /devcontainer/web-app/src/hosts.c */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "../include/hosts.h"
#include "../include/server.h"

static struct known_hosts hosts;

/* Add mutex for thread safety */
static pthread_mutex_t hosts_mutex = PTHREAD_MUTEX_INITIALIZER;

static uint32_t
cidr_to_mask(int bits)
{
    return bits ? ~((1u << (32 - bits)) - 1) : 0;
}

int
hosts_init(void)
{
    const char *default_hosts[] = {
        "127.0.0.1",
        "localhost",
        "0.0.0.0",
        "www.enssol.com.au",
        "enssol.com.au",
        "203.51.2.82"
    };

    const char *cloudflare_cidrs[] = {
        "173.245.48.0/20",
        "103.21.244.0/22",
        "103.22.200.0/22",
        "103.31.4.0/22",
        "141.101.64.0/18",
        "108.162.192.0/18",
        "190.93.240.0/20",
        "188.114.96.0/20",
        "197.234.240.0/22",
        "198.41.128.0/17",
        "162.158.0.0/15",
        "104.16.0.0/13",
        "104.24.0.0/14",
        "172.64.0.0/13",
        "131.0.72.0/22"
    };

    size_t i;

    memset(&hosts, 0, sizeof(hosts));

    /* Add default hosts */
    for (i = 0; i < sizeof(default_hosts) / sizeof(default_hosts[0]); i++) {
        if (hosts_add(default_hosts[i]) != 0) {
            return -1;
        }
    }

    /* Add Cloudflare CIDR blocks */
    for (i = 0; i < sizeof(cloudflare_cidrs) / sizeof(cloudflare_cidrs[0]); i++) {
        if (hosts_add_cidr(cloudflare_cidrs[i]) != 0) {
            return -1;
        }
    }

    return 0;
}

int
hosts_add(const char *host)
{
    int ret = -1;

    if (pthread_mutex_lock(&hosts_mutex) != 0) {
        return -1;
    }

    if (host != NULL && hosts.host_count < MAX_HOSTS) {
        strncpy(hosts.hosts[hosts.host_count], host, MAX_HOST_LENGTH - 1);
        hosts.hosts[hosts.host_count][MAX_HOST_LENGTH - 1] = '\0';
        hosts.host_count++;
        ret = 0;
    }

    pthread_mutex_unlock(&hosts_mutex);
    return ret;
}

int
hosts_add_cidr(const char *cidr)
{
    char ip[16];
    int bits;
    char *slash;
    struct cidr_block *block;

    if (cidr == NULL || hosts.cidr_count >= MAX_CIDR_BLOCKS) {
        return -1;
    }

    strncpy(ip, cidr, sizeof(ip) - 1);
    ip[sizeof(ip) - 1] = '\0';

    slash = strchr(ip, '/');
    if (slash == NULL) {
        return -1;
    }

    *slash = '\0';
    bits = atoi(slash + 1);
    if (bits < 0 || bits > 32) {
        return -1;
    }

    block = &hosts.cidrs[hosts.cidr_count];
    if (inet_pton(AF_INET, ip, &block->network) != 1) {
        return -1;
    }

    block->mask = cidr_to_mask(bits);
    block->network.s_addr &= block->mask;
    hosts.cidr_count++;

    return 0;
}

int
hosts_check(const char *host)
{
    size_t i;
    struct in_addr addr;

    if (host == NULL) {
        return 0;
    }

    /* Check if host is in known hosts list */
    for (i = 0; i < hosts.host_count; i++) {
        if (strcmp(hosts.hosts[i], host) == 0) {
            return 1;
        }
    }

    /* Check if host is an IP address */
    if (inet_pton(AF_INET, host, &addr) == 1) {
        return hosts_check_ip(&addr);
    }

    return 0;
}

int
hosts_check_ip(const struct in_addr *addr)
{
    size_t i;
    uint32_t ip;

    if (addr == NULL) {
        return 0;
    }

    ip = ntohl(addr->s_addr);

    /* Check if IP is in any CIDR block */
    for (i = 0; i < hosts.cidr_count; i++) {
        uint32_t net = ntohl(hosts.cidrs[i].network.s_addr);
        uint32_t mask = hosts.cidrs[i].mask;

        if ((ip & mask) == net) {
            return 1;
        }
    }

    return 0;
}

void
hosts_cleanup(void)
{
    memset(&hosts, 0, sizeof(hosts));
}
