#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include "../standards.h"

/* Function prototypes */
int write_file_with_lock(const char *filepath, const char *data);
int append_file_with_lock(const char *filepath, const char *data);

#endif /* FILE_WRITER_H */
