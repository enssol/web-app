/* filepath: /home/appuser/web-app/src/http/http_record_handler.c */
#include "../../include/http/http_record_handler.h"
#include "../../include/recutils/rec_file_writer.h"
#include "../../include/recutils/rec_record_parser.h"
#include "../../include/logging/log_writer.h"
#include "../../include/constant.h"

int
handle_create_record(int client_socket, const char *data)
{
    char filepath[PATH_MAX];
    const char *project;

    /* Extract project from data */
    if (strstr(data, "SCJV")) {
        project = SCJV_RECORD;
    } else if (strstr(data, "MS1180")) {
        project = MS1180_RECORD;
    } else if (strstr(data, "W6946")) {
        project = W6946_RECORD;
    } else {
        return ERR_PARAM;
    }

    if (snprintf(filepath, sizeof(filepath), "%s", project) < 0) {
        return ERR_INTERNAL;
    }

    int result;
    const char success_response[] =
        "HTTP/1.0 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Access-Control-Allow-Origin: *\r\n\r\n"
        "{\"status\":\"success\",\"message\":\"Record created\"}\r\n";
    const char error_response[] =
        "HTTP/1.0 400 Bad Request\r\n"
        "Content-Type: application/json\r\n"
        "Access-Control-Allow-Origin: *\r\n\r\n"
        "{\"status\":\"error\",\"message\":\"Invalid request\"}\r\n";

    if (!data) {
        write(client_socket, error_response, strlen(error_response));
        return ERR_PARAM;
    }

    result = create_record_in_file(data);
if (result != ERR_NONE) {
    log_record_operation("create_failed", data);
    return result;
}

    write(client_socket, success_response, strlen(success_response));
    return ERR_NONE;
}

int
handle_update_record(int client_socket, const char *data)
{
    FILE *fp;
    int result;
    const char *success_response =
        "HTTP/1.0 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Access-Control-Allow-Origin: *\r\n\r\n"
        "{\"status\":\"success\",\"message\":\"Record updated\"}\r\n";
    const char *error_response =
        "HTTP/1.0 400 Bad Request\r\n"
        "Content-Type: application/json\r\n"
        "Access-Control-Allow-Origin: *\r\n\r\n"
        "{\"status\":\"error\",\"message\":\"Update failed\"}\r\n";

    if (!data) {
        write(client_socket, error_response, strlen(error_response));
        return ERR_PARAM;
    }

    fp = fopen(ENDPOINT_READ, "a");
    if (!fp) {
        write(client_socket, error_response, strlen(error_response));
        return ERR_IO;
    }

    result = update_record_in_file(fp, data);
    fclose(fp);

    if (result != ERR_NONE) {
        write(client_socket, error_response, strlen(error_response));
        return result;
    }

    write(client_socket, success_response, strlen(success_response));
    return ERR_NONE;
}

int
handle_next_number(int client_socket)
{
    char response[256];
    int number;

    number = get_next_obligation_number();
    if (number < 0) {
        snprintf(response, sizeof(response),
            "HTTP/1.0 500 Internal Server Error\r\n"
            "Content-Type: application/json\r\n"
            "Access-Control-Allow-Origin: *\r\n\r\n"
            "{\"status\":\"error\",\"message\":\"Failed to get number\"}\r\n");
        write(client_socket, response, strlen(response));
        return ERR_INTERNAL;
    }

    snprintf(response, sizeof(response),
        "HTTP/1.0 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Access-Control-Allow-Origin: *\r\n\r\n"
        "{\"status\":\"success\",\"number\":%d}\r\n", number);

    write(client_socket, response, strlen(response));
    return ERR_NONE;
}
