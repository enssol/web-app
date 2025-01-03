#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <stddef.h>

// Helper function to initialize test environment
void initialize_test_environment(void);

// Helper function to cleanup test environment
void cleanup_test_environment(void);

// Helper function to compare strings
int compare_strings(const char *str1, const char *str2);

#endif /* TEST_HELPERS_H */
