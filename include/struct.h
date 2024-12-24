#ifndef STRUCT_H
#define STRUCT_H

#include <stddef.h>

/* web_server.h */
struct user_entry {
    char username[64];
    char password[64];
    int uid;
    int gid;
    char fullname[128];
    char homedir[128];
    char shell[64];
    int is_admin;
};

int parse_auth_file(const char *filename, struct user_entry *entries, size_t max_entries);
int validate_integer_fields(char *token);

#endif
