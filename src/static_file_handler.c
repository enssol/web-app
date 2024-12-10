/**
 * \file static_file_handler.c
 * \brief Implements functions for handling static file requests.
 * \author Adrian Gallo
 * \license AGPL-3.0-or-later
 */

#include "../include/static_file_handler.h"
#include "../include/config_loader.h"
#include "../include/constants.h"
#include "../include/http_response.h"
#include "../include/logger.h"
#include <fcntl.h> /* Include for open and O_RDONLY */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>    /* Include for strcasecmp */
#include <sys/socket.h> /* Include for send */
#include <sys/stat.h>
#include <unistd.h>

#define RESPONSE_BUFFER_SIZE 16384
#define MAX_PATH_LENGTH 256

extern char document_root[MAX_PATH_LENGTH];

/* External config variable */
extern Config config;

/**
 * \brief Converts a hexadecimal character to its integer value.
 *
 * \param c The hexadecimal character.
 * \return The integer value.
 */
static int
hexValue (char c)
{
    if (c >= '0' && c <= '9')
        {
            return c - '0';
        }
    if (c >= 'a' && c <= 'f')
        {
            return c - 'a' + 10;
        }
    if (c >= 'A' && c <= 'F')
        {
            return c - 'A' + 10;
        }
    return -1;
}

/**
 * \brief Gets the MIME type based on the file extension.
 *
 * \param file_ext The file extension.
 * \return The MIME type.
 */
const char *
getMimeType (const char *file_ext)
{
    if (strcasecmp (file_ext, "html") == 0)
        return "text/html";
    if (strcasecmp (file_ext, "txt") == 0)
        return "text/plain";
    if (strcasecmp (file_ext, "jpg") == 0
        || strcasecmp (file_ext, "jpeg") == 0)
        return "image/jpeg";
    if (strcasecmp (file_ext, "png") == 0)
        return "image/png";
    return "application/octet-stream";
}

void
serveStaticFile (int client_fd, const char *file_path)
{
    char full_path[PATH_MAX];
    int file_fd;
    char *file_extension;
    const char *mime_type;
    char header[256];
    char buffer[RESPONSE_BUFFER_SIZE];
    ssize_t bytes_read;

    snprintf (full_path, sizeof (full_path), "%s/%s", document_root,
              file_path);
    logInfo ("Full file path: %s", full_path);

    file_fd = open (full_path, O_RDONLY);
    if (file_fd == -1)
        {
            const char *not_found_response
                = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
            send (client_fd, not_found_response, strlen (not_found_response),
                  0);
            logError ("Failed to open file: %s", full_path);
            return;
        }

    /* Get the file extension and MIME type */
    file_extension = getFileExtension (file_path);
    mime_type = getMimeType (file_extension);
    free (file_extension);

    if (strcmp (mime_type, "application/octet-stream") == 0)
        {
            const char *unsupported_response
                = "HTTP/1.1 415 Unsupported Media Type\r\nContent-Length: "
                  "0\r\n\r\n";
            send (client_fd, unsupported_response,
                  strlen (unsupported_response), 0);
            close (file_fd);
            logError ("Unsupported media type for file: %s", full_path);
            return;
        }

    snprintf (header, sizeof (header),
              "HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n", mime_type);
    send (client_fd, header, strlen (header), 0);
    logInfo ("Sending response header: %s", header);

    while ((bytes_read = read (file_fd, buffer, sizeof (buffer))) > 0)
        {
            send (client_fd, buffer, bytes_read, 0);
            logInfo ("Sent %zd bytes of file: %s", bytes_read, full_path);
        }

    close (file_fd);
    logInfo ("Finished serving file: %s", full_path);
}
