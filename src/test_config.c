#include "test_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
load_test_data(const char *filename, struct test_data *data, size_t size)
{
    FILE *file;
    char buffer[TEST_MAX_BUF];
    size_t i = 0;

    file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL && i < size) {
        char *token = strtok(buffer, ",");
        if (token != NULL) {
            data[i].id = atoi(token);
            token = strtok(NULL, ",");
            if (token != NULL) {
                strncpy(data[i].name, token, TEST_MAX_BUF - 1);
                data[i].name[TEST_MAX_BUF - 1] = '\0';
            }
        }
        i++;
    }

    fclose(file);
    return 0;
}
