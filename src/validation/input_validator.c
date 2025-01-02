#include "../../include/validation/input_validator.h"
#include "../../include/standards.h"

void
parse_query_string(const char *query, char *username, char *password)
{
    char *param;
    char *value;
    char *query_copy;
    char *saveptr1;

    username[0] = '\0';
    password[0] = '\0';

    query_copy = strdup(query);
    if (!query_copy) {
        return;
    }

    param = strtok_r(query_copy, "&", &saveptr1);
    while (param) {
        value = strchr(param, '=');
        if (value) {
            *value = '\0';
            value++;

            if (strcmp(param, "username") == 0) {
                strncpy(username, value, 255);
                username[255] = '\0';
            } else if (strcmp(param, "password") == 0) {
                strncpy(password, value, 255);
                password[255] = '\0';
            }
        }
        param = strtok_r(NULL, "&", &saveptr1);
    }

    free(query_copy);
}
