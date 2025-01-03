/* filepath: /home/appuser/web-app/src/recutils/rec_record_parser.c */
#include "../../include/recutils/rec_record_parser.h"
#include "../../include/constant.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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
            perror("Error opening obligation number file");
            return -1;
        }
        fprintf(fp, "1\n");
        fclose(fp);
        return 1;
    }

    /* Lock file */
    if (flock(fileno(fp), LOCK_EX) != 0) {
        perror("Error locking obligation number file");
        fclose(fp);
        return -1;
    }

    /* Read current number */
    if (fgets(buffer, sizeof(buffer), fp)) {
        number = atoi(buffer);
        if (number < 1) {
            number = 1;
        }
    } else {
        perror("Error reading obligation number file");
    }

    /* Validate number format */
    if (number < 1) {
        fprintf(stderr, "Invalid obligation number format\n");
        number = 1;
    }

    /* Handle sequence gaps/resets */
    if (number > 1000) {
        fprintf(stderr, "Obligation number sequence reset\n");
        number = 1;
    }

    /* Write next number */
    rewind(fp);
    fprintf(fp, "%d\n", number + 1);

    /* Release lock and close */
    flock(fileno(fp), LOCK_UN);
    fclose(fp);

    /* Log obligation number generation */
    fprintf(stdout, "Generated obligation number: %d\n", number);

    return number;
}
