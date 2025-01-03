/* filepath: /home/appuser/web-app/src/recutils/rec_obligation_generator.c */
#include "../../include/recutils/rec_obligation_generator.h"
#include "../../include/constant.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>

int
get_next_obligation_number(void)
{
    FILE *fp;
    int number = 1;
    char buffer[32];

    fp = fopen(OBLIGATION_NUMBER_FILE, "r+");
    if (!fp) {
        /* Create file if it doesn't exist */
        fp = fopen(OBLIGATION_NUMBER_FILE, "w+");
        if (!fp) {
            return -1;
        }
        fprintf(fp, "1\n");
        fclose(fp);
        return 1;
    }

    /* Lock file */
    if (flock(fileno(fp), LOCK_EX) != 0) {
        fclose(fp);
        return -1;
    }

    /* Read current number */
    if (fgets(buffer, sizeof(buffer), fp)) {
        number = atoi(buffer);
        if (number < 1) {
            number = 1;
        }
    }

    /* Write next number */
    rewind(fp);
    fprintf(fp, "%d\n", number + 1);

    /* Release lock and close */
    flock(fileno(fp), LOCK_UN);
    fclose(fp);

    return number;
}
