/**
 * Copyright 2024 Enveng Group - Adrian Gallo.
 * SPDX-License-Identifier: 	AGPL-3.0-or-later
 */

#include "../include/server.h"
#include <aio.h>
#include <arpa/inet.h>
#include <assert.h>
#include <complex.h>
#include <cpio.h>
#include <ctype.h>
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <fenv.h>
#include <float.h>
#include <fnmatch.h>
#include <ftw.h>
#include <glob.h>
#include <grp.h>
#include <iconv.h>
#include <inttypes.h>
#include <iso646.h>
#include <langinfo.h>
#include <libgen.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <monetary.h>
#include <mqueue.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <nl_types.h>
#include <poll.h>
#include <pthread.h>
#include <pwd.h>
#include <regex.h>
#include <sched.h>
#include <search.h>
#include <semaphore.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <sys/resource.h>
#include <sys/select.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <syslog.h>
#include <termios.h>
#include <tgmath.h>
#include <time.h>
#include <ulimit.h>
#include <unistd.h>
#include <utime.h>
#include <wchar.h>
#include <wctype.h>
#include <wordexp.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define MAX_CLIENTS 10

void
start_server ()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof (address);
    const char *port = getenv ("PORT");
    const char *ssl_cert_path = getenv ("SSL_CERT_PATH");
    const char *ssl_key_path = getenv ("SSL_KEY_PATH");

    if (port == NULL || ssl_cert_path == NULL || ssl_key_path == NULL)
        {
            fprintf (stderr, "Environment variables PORT, SSL_CERT_PATH, or SSL_KEY_PATH not set\n");
            exit (EXIT_FAILURE);
        }

    SSL_CTX *ctx;
    SSL_library_init ();
    OpenSSL_add_all_algorithms ();
    SSL_load_error_strings ();
    ctx = SSL_CTX_new (SSLv23_server_method ());

    if (!SSL_CTX_use_certificate_file (ctx, ssl_cert_path, SSL_FILETYPE_PEM)
        || !SSL_CTX_use_PrivateKey_file (ctx, ssl_key_path, SSL_FILETYPE_PEM))
        {
            ERR_print_errors_fp (stderr);
            exit (EXIT_FAILURE);
        }

    if ((server_fd = socket (AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror ("socket failed");
            exit (EXIT_FAILURE);
        }

    if (setsockopt (server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                    sizeof (opt)))
        {
            perror ("setsockopt");
            exit (EXIT_FAILURE);
        }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons (atoi (port));

    if (bind (server_fd, (struct sockaddr *)&address, sizeof (address)) < 0)
        {
            perror ("bind failed");
            exit (EXIT_FAILURE);
        }

    if (listen (server_fd, MAX_CLIENTS) < 0)
        {
            perror ("listen");
            exit (EXIT_FAILURE);
        }

    while (1)
        {
            if ((new_socket = accept (server_fd, (struct sockaddr *)&address,
                                      (socklen_t *)&addrlen))
                < 0)
                {
                    perror ("accept");
                    exit (EXIT_FAILURE);
                }

            // Handle client connection
            handle_client (new_socket, ctx);
        }
}

void
handle_client (int client_socket, SSL_CTX *ctx)
{
    SSL *ssl;
    char buffer[1024] = { 0 };
    ssl = SSL_new (ctx);
    SSL_set_fd (ssl, client_socket);

    if (SSL_accept (ssl) <= 0)
        {
            ERR_print_errors_fp (stderr);
        }
    else
        {
            SSL_read (ssl, buffer, sizeof (buffer));
            printf ("Received: %s\n", buffer);
            SSL_write (ssl,
                       "HTTP/1.1 200 OK\nContent-Type: text/html\n\nHello World",
                       strlen ("HTTP/1.1 200 OK\nContent-Type: text/html\n\nHello World"));
        }

    SSL_shutdown (ssl);
    SSL_free (ssl);
    close (client_socket);
}
