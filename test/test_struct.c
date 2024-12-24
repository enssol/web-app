#include <CUnit/Basic.h>
#include "../include/struct.h"
#include "test_suites.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

#define NUM_ENTRIES 1

static int test_client[2];


/* Setup and teardown */
static int suite_setup(void) {
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, test_client) != 0) {
        return -1;
    }
    return 0;
}

static int suite_teardown(void) {
    close(test_client[0]);
    close(test_client[1]);
    return 0;
}

static void
test_parse_auth_file_valid(void)
{
    static const char *test_filename;
    FILE *fp;
    int result;
    struct user_entry entries[NUM_ENTRIES];
    size_t max_entries = NUM_ENTRIES;
    
    test_filename = "test/test_auth.passwd";
    fp = fopen(test_filename, "r");
    CU_ASSERT_PTR_NOT_NULL(fp);

    result = parse_auth_file(test_filename, entries, max_entries);

    /* 
        Username:Password:UID:GID:FullName:HomeDir:Shell:IsAdmin
        john:smith:1000:1000:John Smith:/home/john:/bin/sh:0
    
    */

    CU_ASSERT_EQUAL(strcmp(entries[0].username, "john"), 0);
    CU_ASSERT_EQUAL(strcmp(entries[0].password, "smith"), 0);
    CU_ASSERT_EQUAL(entries[0].uid, 1000);
    CU_ASSERT_EQUAL(entries[0].gid, 1000);
    CU_ASSERT_EQUAL(strcmp(entries[0].fullname, "John Smith"), 0);
    CU_ASSERT_EQUAL(strcmp(entries[0].homedir, "/home/john"), 0);
    CU_ASSERT_EQUAL(strcmp(entries[0].shell, "/bin/sh"), 0);
    CU_ASSERT_EQUAL(entries[0].is_admin, 0);
        
    CU_ASSERT_EQUAL(result, NUM_ENTRIES);

    fclose(fp);
}

static void
test_parse_auth_file_invalid(void)
{
    static const char *test_filename;
    FILE *fp;
    int result;
    struct user_entry entries[NUM_ENTRIES];
    size_t max_entries = NUM_ENTRIES;
    /* int i; */

    test_filename = "test/invalid_test_auth.passwd";
    fp = fopen(test_filename, "r");
    CU_ASSERT_PTR_NOT_NULL(fp);

    result = parse_auth_file(test_filename, entries, max_entries);
    CU_ASSERT_EQUAL(result, 0);

    fclose(fp);
}

/* Test suite initialization */
int init_struct_suite(CU_pSuite suite) {
    suite->pInitializeFunc = suite_setup;
    suite->pCleanupFunc = suite_teardown;
    
    CU_add_test(suite, "Test Valid File", test_parse_auth_file_valid);
    CU_add_test(suite, "Test Invalid File", test_parse_auth_file_invalid);
    
    return 0;
}
