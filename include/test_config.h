#ifndef TEST_CONFIG_H
#define TEST_CONFIG_H

/* Constants */
#define TEST_MAX_BUF 1024
#define TEST_TIMEOUT 30

/* Test data structure */
struct test_data {
    int id;
    char name[TEST_MAX_BUF];
};

/* Function prototypes */
int load_test_data(const char *filename, struct test_data *data, size_t size);

#endif /* TEST_CONFIG_H */
