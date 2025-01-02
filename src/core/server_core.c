#include "../../include/core/server_core.h"
#include "../../include/constant.h"
#include "../../include/common.h"

static int server_socket = -1;

int
server_core_init(void)
{
    struct sigaction sa;

    /* Setup signal handler */
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGTERM, &sa, NULL) < 0 ||
        sigaction(SIGINT, &sa, NULL) < 0) {
        return -1;
    }

    /* Initialize server socket */
    server_socket = setup_server(DEFAULT_PORT);
    if (server_socket < 0) {
        return -1;
    }

    return 0;
}

int
setup_server(int port)
{
    int sock;
    const int enable = 1;
    union {
        struct sockaddr sa;
        struct sockaddr_in sin;
    } addr;

    /* Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return -1;
    }

    /* Set socket options */
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
                   &enable, sizeof(int)) < 0) {
        close(sock);
        return -1;
    }

    /* Initialize server address structure */
    memset(&addr, 0, sizeof(addr));
    addr.sin.sin_family = AF_INET;
    addr.sin.sin_addr.s_addr = INADDR_ANY;
    addr.sin.sin_port = htons((unsigned short)port);

    /* Bind socket */
    if (bind(sock, &addr.sa, sizeof(addr.sin)) < 0) {
        close(sock);
        return -1;
    }

    /* Listen for connections */
    if (listen(sock, SOMAXCONN) < 0) {
        close(sock);
        return -1;
    }

    return sock;
}

void
server_core_stop(void)
{
    server_running = 0;
}

int
server_core_cleanup(void)
{
    if (server_socket >= 0) {
        close(server_socket);
        server_socket = -1;
    }
    return 0;
}

/* Serve static files from correct directory */
if (strncmp(path, "/pages/", 7) == 0) {
    return serve_static_file(client_socket, PAGES_DIR, path + 7);
}
