/* filepath: /home/appuser/web-app/include/logging/log_rotator.h */
#ifndef LOG_ROTATOR_H
#define LOG_ROTATOR_H

#include "../standards.h"

/* Function prototypes */
int rotate_log(const char *logname);
int check_log_size(const char *filename);

#endif /* LOG_ROTATOR_H */
