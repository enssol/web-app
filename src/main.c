/* filepath: /home/appuser/fork-web-app/src/main.c */
/* Local headers */
#include "../include/standards.h"
#include "../include/constant.h"
#include "../include/common.h"
#include "../include/web_server.h"
#include "../include/core/server_core.h"

int
main(void)
{
    int client_fd;

    /* Initialize server core */
    if (server_core_init() < 0) {
        perror("Failed to initialize server");
        return EXIT_FAILURE;
    }

    printf("Server running on port %d...\n", DEFAULT_PORT);

    /* Main server loop */
    while (server_running) {
        client_fd = accept(server_socket, NULL, NULL);
        if (client_fd < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("Failed to accept connection");
            continue;
        }

        handle_client(client_fd, WWW_ROOT);
        close(client_fd);
    }

    /* Cleanup */
    server_core_cleanup();
    return EXIT_SUCCESS;
}
