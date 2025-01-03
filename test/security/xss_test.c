#include <stdio.h>
#include <string.h>
#include <assert.h>

// Function prototypes
void test_xss_vulnerability(const char *input);

int
main(void)
{
    // Test cases for XSS vulnerabilities
    test_xss_vulnerability("<script>alert('XSS');</script>");
    test_xss_vulnerability("<img src='x' onerror='alert(1)'>");
    test_xss_vulnerability("<body onload=alert('XSS')>");

    printf("All XSS tests passed.\n");
    return 0;
}

void
test_xss_vulnerability(const char *input)
{
    // Simulate XSS vulnerability check
    assert(strstr(input, "<script>") == NULL);
    assert(strstr(input, "onerror") == NULL);
    assert(strstr(input, "onload") == NULL);
}
