#include "../../include/http/http_request_handler.h"
#include "../../include/http/http_user_handler.h"
#include "../../include/http/http_record_handler.h"
#include "../../include/constant.h"
#include "../../include/common.h"
#include "../../include/authentication/user_authenticator.h"
#include "../../include/logging/log_writer.h"
#include "../../include/validation/input_validator.h"
#include "../../include/io/socket_handler.h"

int
handle_client(int client_socket, const char *www_root)
{
    char buffer[MAX_BUFFER_SIZE];
    char method[16];
    char path[MAX_PATH_SIZE];
    char protocol[16];
    char username[256];
    char password[256];
    const char *unauthorized = "HTTP/1.0 401 Unauthorized\r\n"
                             "Content-Type: text/html\r\n"
                             "WWW-Authenticate: Basic realm=\"User Login\"\r\n\r\n";
    const char *not_found = "HTTP/1.0 404 Not Found\r\n"
                           "Content-Type: text/html\r\n\r\n"
                           "<h1>404 Not Found</h1>";

    if (client_socket < 0 || !www_root) {
        return ERR_PARAM;
    }

    /* Read the HTTP request */
    if (read_socket(client_socket, buffer, sizeof(buffer)) <= 0) {
        return ERR_IO;
    }

    /* Parse the request line */
    if (sscanf(buffer, "%15s %1023s %15s", method, path, protocol) != 3) {
        write(client_socket, not_found, strlen(not_found));
        return ERR_PARSE;
    }

    /* Handle authentication for protected paths */
    if (strncmp(path, "/api/", 5) == 0) {
        /* Extract credentials */
        parse_query_string(buffer, username, password);

        if (!check_auth(username, password)) {
            write(client_socket, unauthorized, strlen(unauthorized));
            return ERR_AUTH;
        }
    }

    /* Update static file paths to use /public prefix */
    if (strncmp(path, "/public/", 8) == 0) {
        /* Serve from WWW_ROOT */
        return serve_static_file(client_socket, www_root, path + 7);
    }

    /* Page routes */
    if (strcmp(path, "/pages/dashboard.html") == 0) {
        /* Route to dashboard */
    } else if (strcmp(path, "/pages/audit.html") == 0) {
        /* Route to audit */
    }
    /* etc for other pages */

    /* Route the request */
    if (strcmp(path, "/api/users") == 0) {
        if (strcmp(method, "GET") == 0) {
            return handle_users_request(client_socket);
        }
    } else if (strcmp(path, "/api/record") == 0) {
        if (strcmp(method, "POST") == 0) {
            return handle_create_record(client_socket, buffer);
        } else if (strcmp(method, "PUT") == 0) {
            return handle_update_record(client_socket, buffer);
        }
    } else if (strcmp(path, "/api/next") == 0) {
        if (strcmp(method, "GET") == 0) {
            return handle_next_number(client_socket);
        }
    }

    /* Path not found */
    write(client_socket, not_found, strlen(not_found));
    return ERR_NONE;
}
