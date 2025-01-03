#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <string.h>

/* Function to compare two strings */
int
compare_strings(const char *str1, const char *str2)
{
    return strcmp(str1, str2) == 0;
}

/* Function to check if a string is empty */
int
is_empty_string(const char *str)
{
    return str == NULL || str[0] == '\0';
}

#endif /* TEST_UTILS_H */
