#include "../../include/io/socket_handler.h"
#include "../../include/constant.h"
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

int
read_socket(int sockfd, char *buffer, size_t size)
{
    ssize_t total_read;
    ssize_t bytes_read;
    size_t remaining;

    if (sockfd < 0 || !buffer || size == 0) {
        return ERR_PARAM;
    }

    total_read = 0;
    remaining = size - 1;

    while (remaining > 0) {
        bytes_read = read(sockfd, buffer + total_read, remaining);
        if (bytes_read < 0) {
            if (errno == EINTR) {
                continue;
            }
            return ERR_IO;
        }
        if (bytes_read == 0) {
            break;
        }
        total_read += bytes_read;
        remaining -= bytes_read;
    }

    buffer[total_read] = '\0';
    return total_read;
}

int
write_socket(int sockfd, const char *data, size_t size)
{
    ssize_t total_written;
    ssize_t bytes_written;
    size_t remaining;

    if (sockfd < 0 || !data) {
        return ERR_PARAM;
    }

    total_written = 0;
    remaining = size;

    while (remaining > 0) {
        bytes_written = write(sockfd, data + total_written, remaining);
        if (bytes_written < 0) {
            if (errno == EINTR) {
                continue;
            }
            return ERR_IO;
        }
        total_written += bytes_written;
        remaining -= bytes_written;
    }

    return total_written;
}
