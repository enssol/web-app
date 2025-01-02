/* filepath: /home/appuser/web-app/include/util/data_utils.h */
#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#include "../standards.h"
#include "../struct.h"

/* Function prototypes */
int parse_auth_file(const char *filename, struct user_entry *entries, size_t max_entries);

#endif /* DATA_UTILS_H */
