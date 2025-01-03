/* filepath: /home/appuser/web-app/include/logging/log_reader.h */
#ifndef LOG_READER_H
#define LOG_READER_H

#include "../standards.h"

/* Function prototypes */
int read_log_entries(const char *logname);
int parse_log_line(char *line, size_t len);

#endif /* LOG_READER_H */
