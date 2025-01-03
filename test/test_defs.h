#ifndef TEST_DEFS_H
#define TEST_DEFS_H

#include <stdio.h>
#include <stdlib.h>

/* Macro to check test results */
#define TEST_ASSERT(cond) \
    do { \
        if (!(cond)) { \
            fprintf(stderr, "Test failed: %s, file %s, line %d\n", #cond, __FILE__, __LINE__); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

/* Macro to print test results */
#define TEST_PRINT(msg) \
    do { \
        printf("Test: %s\n", msg); \
    } while (0)

#endif /* TEST_DEFS_H */
