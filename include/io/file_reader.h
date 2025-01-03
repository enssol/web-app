#ifndef FILE_READER_H
#define FILE_READER_H

#include "../standards.h"

/* Function prototypes */
int read_file_with_lock(const char *filepath, char *buffer, size_t size);
long get_file_size(const char *filepath);

#endif /* FILE_READER_H */
