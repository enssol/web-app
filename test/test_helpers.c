#include "test_helpers.h"
#include <stdio.h>
#include <string.h>

int
load_test_data(struct test_data *data)
{
    FILE *file;
    char buffer[TEST_MAX_BUF];

    if (data == NULL) {
        return -1;
    }

    file = fopen("test/fixtures/test_data.conf", "r");
    if (file == NULL) {
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (sscanf(buffer, "id = %d", &data->id) == 1) {
            continue;
        }
        if (sscanf(buffer, "name = \"%[^\"]\"", data->name) == 1) {
            continue;
        }
    }

    fclose(file);
    return 0;
}
