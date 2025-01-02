#include "../../include/http/http_user_handler.h"
#include "../../include/constant.h"
#include "../../include/standards.h"
#include "../../include/common.h"
#include "../../include/logging/log_writer.h"
#include "../../include/validation/input_validator.h"
#include "../../include/authentication/user_authenticator.h"

int
handle_users_request(int client_socket)
{
    char buffer[MAX_BUFFER_SIZE];
    const char *response;
    int result;

    if (client_socket < 0) {
        return ERR_PARAM;
    }

    /* Read user data file */
    result = read_file_with_lock(USERS_FILE, buffer, sizeof(buffer));
    if (result != ERR_NONE) {
        response = "HTTP/1.0 500 Internal Server Error\r\n"
                  "Content-Type: application/json\r\n"
                  "Access-Control-Allow-Origin: *\r\n\r\n"
                  "{\"status\":\"error\",\"message\":\"Failed to read users\"}\r\n";
        write(client_socket, response, strlen(response));
        return result;
    }

    /* Send successful response with user data */
    response = "HTTP/1.0 200 OK\r\n"
              "Content-Type: application/json\r\n"
              "Access-Control-Allow-Origin: *\r\n\r\n";
    write(client_socket, response, strlen(response));
    write(client_socket, buffer, strlen(buffer));

    return ERR_NONE;
}

int
handle_update_user(int client_socket, const char *username,
                  const char *fullname, const char *email,
                  const char *project)
{
    FILE *fp;
    char record[MAX_BUFFER_SIZE];
    const char *response;

    if (client_socket < 0 || !username || !fullname || !email || !project) {
        return ERR_PARAM;
    }

    /* Format user record */
    if (snprintf(record, sizeof(record),
                "{\"username\":\"%s\",\"fullname\":\"%s\","
                "\"email\":\"%s\",\"project\":\"%s\"}",
                username, fullname, email, project) >= sizeof(record)) {
        return ERR_OVERFLOW;
    }

    /* Update user record */
    fp = fopen(USERS_FILE, "a");
    if (!fp) {
        response = "HTTP/1.0 500 Internal Server Error\r\n"
                  "Content-Type: application/json\r\n"
                  "Access-Control-Allow-Origin: *\r\n\r\n"
                  "{\"status\":\"error\",\"message\":\"Failed to update user\"}\r\n";
        write(client_socket, response, strlen(response));
        return ERR_IO;
    }

    /* Write record with locking */
    if (flock(fileno(fp), LOCK_EX) == 0) {
        fprintf(fp, "%s\n", record);
        flock(fileno(fp), LOCK_UN);
    }

    fclose(fp);

    /* Log the update */
    log_message(LOG_INFO, username, "user_update", record);

    /* Send success response */
    response = "HTTP/1.0 200 OK\r\n"
              "Content-Type: application/json\r\n"
              "Access-Control-Allow-Origin: *\r\n\r\n"
              "{\"status\":\"success\",\"message\":\"User updated\"}\r\n";
    write(client_socket, response, strlen(response));

    return ERR_NONE;
}
