#include "../include/struct.h"
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h> 

int
parse_auth_file(const char *filename, struct user_entry *entries, size_t max_entries)
{
    FILE *fp;
    char line[512];
    size_t count = 0;
    char *token;
    char *saveptr;

    fp = fopen(filename, "r");
    if (!fp) {
        return -1;
    }

    while (fgets(line, sizeof(line), fp) && count < max_entries) {
        /* Skip comments and empty lines */
        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }

        /* Parse line fields */
        token = strtok_r(line, ":", &saveptr);
        if (!token) continue;
        strncpy(entries[count].username, token, sizeof(entries[count].username) - 1);

        token = strtok_r(NULL, ":", &saveptr);
        if (!token) continue;
        strncpy(entries[count].password, token, sizeof(entries[count].password) - 1);

        /* Parse remaining fields */
        token = strtok_r(NULL, ":", &saveptr);
        if (!token || validate_integer_fields(token) != 0) continue;
        entries[count].uid = atoi(token);

        token = strtok_r(NULL, ":", &saveptr);
        if (!token || validate_integer_fields(token) != 0) continue;
        entries[count].gid = atoi(token);

        token = strtok_r(NULL, ":", &saveptr);
        if (!token) continue;
        strncpy(entries[count].fullname, token, sizeof(entries[count].fullname) - 1);

        token = strtok_r(NULL, ":", &saveptr);
        if (!token) continue;
        strncpy(entries[count].homedir, token, sizeof(entries[count].homedir) - 1);

        token = strtok_r(NULL, ":", &saveptr);
        if (!token) continue;
        strncpy(entries[count].shell, token, sizeof(entries[count].shell) - 1);

        token = strtok_r(NULL, ":", &saveptr);
        entries[count].is_admin = token ? atoi(token) : 0;
        count++;
    }

    fclose(fp);
    return (int)count;
}

int validate_integer_fields(char *token) 
{
    int i;
    for(i = 0; token[i] != '\0'; i++) {
        if (!isdigit(token[i])) {
            return -1;
        }
    }
    return 0;
}