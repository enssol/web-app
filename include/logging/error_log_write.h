/* filepath: /home/appuser/web-app/include/logging/error_log_writer.h */
#ifndef ERROR_LOG_WRITER_H
#define ERROR_LOG_WRITER_H

#include "../standards.h"

/* Function prototypes */
int log_error(const char *username, const char *error_message);
int log_system_error(const char *error_message);

#endif /* ERROR_LOG_WRITER_H */
