#include <stdio.h>
#include <string.h>
#include <assert.h>

// Function prototypes
void test_sql_injection(const char *input);

int
main(void)
{
    // Test cases for SQL Injection vulnerabilities
    test_sql_injection("'; DROP TABLE users; --");
    test_sql_injection("' OR '1'='1");
    test_sql_injection("' UNION SELECT * FROM users --");

    printf("All SQL Injection tests passed.\n");
    return 0;
}

void
test_sql_injection(const char *input)
{
    // Simulate SQL Injection vulnerability check
    assert(strstr(input, "DROP TABLE") == NULL);
    assert(strstr(input, "OR '1'='1") == NULL);
    assert(strstr(input, "UNION SELECT") == NULL);
}
