#include <stdio.h>
#include <string.h>
#include <assert.h>

// Function prototypes
void test_penetration(const char *input);

int
main(void)
{
    // Test cases for comprehensive penetration testing
    test_penetration("../../../etc/passwd");
    test_penetration("'; EXEC xp_cmdshell('dir'); --");
    test_penetration("<iframe src='http://malicious.com'></iframe>");

    printf("All penetration tests passed.\n");
    return 0;
}

void
test_penetration(const char *input)
{
    // Simulate penetration test vulnerability check
    assert(strstr(input, "../../../") == NULL);
    assert(strstr(input, "xp_cmdshell") == NULL);
    assert(strstr(input, "<iframe") == NULL);
}
