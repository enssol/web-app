/* filepath: /home/appuser/web-app/include/logging/log_writer.h */
#ifndef LOG_WRITER_H
#define LOG_WRITER_H

#include "../standards.h"

/* Function prototypes */
int log_message(int severity, const char *username, const char *action, const char *message);
int log_audit(const char *username, const char *action);

#endif /* LOG_WRITER_H */
