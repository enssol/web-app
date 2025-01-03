/* filepath: /home/appuser/web-app/include/recutils/rec_file_writer.h */
#ifndef REC_FILE_WRITER_H
#define REC_FILE_WRITER_H

#include "../standards.h"

/* Record management functions */
int create_record_in_file(const char *data);
int update_record_in_file(FILE *fp, const char *data);

#endif /* REC_FILE_WRITER_H */
