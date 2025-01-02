#ifndef STRUCT_H
#define STRUCT_H

/* Data structures */
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

#endif /* STRUCT_H */
