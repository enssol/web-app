/* filepath: test/test_init.c */
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../include/init.h"
#include "../include/app_error.h"
#include "../include/env.h"        /* Add this for env functions */
#include "../include/constants.h"
#include "../include/config.h"
#include "../include/fs.h"
#include "test_suite.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

/* Test directory definitions */
static const char *test_dirs[] = {
    "test_tmp",
    "test_tmp/etc",
    "test_tmp/etc/config",
    "test_tmp/var",
    "test_tmp/var/log",
    "test_tmp/var/run",
    NULL
};

/* Forward declarations of helper functions */
static int createTestDirectories(void) __attribute__((unused));
static int createEnvFile(void) __attribute__((unused));
static int createTestConfig(void) __attribute__((unused));
static int createTestEnv(void) __attribute__((unused));

static int createEnvFile(void)
{
    FILE *fp;
    const char *env_path = "test_tmp/etc/config/env";
    mode_t old_umask;

    /* Set restrictive permissions during file creation */
    old_umask = umask(077);

    fp = fopen(env_path, "w");
    if (fp == NULL) {
        umask(old_umask);
        return -1;
    }

    /* Write required environment variables */
    fprintf(fp, "APP_NAME=TestApp\n");
    fprintf(fp, "APP_VERSION=1.0.0\n");
    fprintf(fp, "APP_ENV=test\n");
    fprintf(fp, "APP_DEBUG=1\n");
    fprintf(fp, "APP_PORT=8080\n");
    fprintf(fp, "APP_HOST=localhost\n");
    fprintf(fp, "LOG_LEVEL=debug\n");
    fprintf(fp, "LOG_PATH=test_tmp/var/log/app.log\n");
    fprintf(fp, "LOG_FORMAT=text\n");
    fprintf(fp, "LOG_MAX_SIZE=1048576\n");
    fprintf(fp, "DB_HOST=localhost\n");
    fprintf(fp, "DB_PORT=5432\n");
    fprintf(fp, "DB_NAME=testdb\n");
    fprintf(fp, "DB_USER=testuser\n");
    fprintf(fp, "DB_PASSWORD=testpass\n");
    fprintf(fp, "CACHE_DRIVER=memory\n");
    fprintf(fp, "CACHE_PREFIX=test\n");
    fprintf(fp, "CACHE_TTL=3600\n");

    fclose(fp);
    umask(old_umask);

    /* Set file permissions */
    if (chmod(env_path, 0600) != 0) {
        return -1;
    }

    return 0;
}

static int createTestDirectories(void)
{
    const char **dir;
    mode_t old_umask;

    old_umask = umask(022);

    /* Create test directories */
    for (dir = test_dirs; *dir != NULL; dir++) {
        if (mkdir(*dir, 0755) != 0 && errno != EEXIST) {
            umask(old_umask);
            return -1;
        }
    }

    umask(old_umask);
    return 0;
}

static int createTestConfig(void)
{
    FILE *fp;
    const char *config_path = "test_tmp/etc/config/system.conf";
    mode_t old_umask;

    old_umask = umask(077);

    fp = fopen(config_path, "w");
    if (fp == NULL) {
        umask(old_umask);
        return -1;
    }

    /* Write standard test configuration */
    fprintf(fp, "APP_NAME=TestApp\n");
    fprintf(fp, "APP_VERSION=1.0.0\n");
    fprintf(fp, "APP_ENV=test\n");
    fprintf(fp, "LOG_PATH=test_tmp/var/log/app.log\n");

    fclose(fp);
    umask(old_umask);

    return 0;
}

static int createTestEnv(void)
{
    FILE *fp;
    mode_t old_umask;

    /* Create test environment file */
    old_umask = umask(022);

    fp = fopen("test_tmp/etc/config/env", "w");
    if (fp == NULL) {
        umask(old_umask);
        return -1;
    }

    /* Write test environment variables */
    fprintf(fp, "APP_NAME=TestApp\n");
    fprintf(fp, "APP_VERSION=1.0.0\n");
    fprintf(fp, "APP_ENV=test\n");
    fprintf(fp, "APP_DEBUG=1\n");
    fprintf(fp, "APP_PORT=8080\n");
    fprintf(fp, "APP_HOST=localhost\n");
    fprintf(fp, "LOG_LEVEL=debug\n");
    fprintf(fp, "LOG_PATH=/var/log\n");
    fprintf(fp, "LOG_FORMAT=text\n");
    fprintf(fp, "LOG_MAX_SIZE=1048576\n");

    fclose(fp);
    umask(old_umask);
    return 0;
}

static int cleanup_test_files(void)
{
    const char *files[] = {
        "test_tmp/etc/config/system.conf",
        "test_tmp/etc/config/env",
        "test_tmp/var/log/app.log",
        NULL
    };
    const char **file;

    for (file = files; *file != NULL; file++) {
        if (unlink(*file) != 0 && errno != ENOENT) {
            fprintf(stderr, "Failed to remove file %s: %s\n",
                    *file, strerror(errno));
        }
    }
    return 0;
}

static int teardown(void)
{
    char **dirs_reverse;
    size_t count;
    size_t i;
    int result;

    /* First remove files */
    cleanup_test_files();

    /* Count directories */
    for (count = 0; test_dirs[count] != NULL; count++)
        ;

    /* Allocate space for reverse array */
    dirs_reverse = malloc(count * sizeof(char *));
    if (dirs_reverse == NULL) {
        fprintf(stderr, "Memory allocation failed in teardown\n");
        return -1;
    }

    /* Copy in reverse order */
    for (i = 0; i < count; i++) {
        dirs_reverse[i] = (char *)test_dirs[count - 1 - i];
    }

    result = 0;
    /* Remove directories in reverse order */
    for (i = 0; i < count; i++) {
        if (rmdir(dirs_reverse[i]) != 0 && errno != ENOENT) {
            fprintf(stderr, "Failed to remove directory %s: %s\n",
                    dirs_reverse[i], strerror(errno));
            result = -1;
        }
    }

    free(dirs_reverse);
    return result;
}

/* Test fixture setup/teardown functions */
static int setup(void)
{
    int status;

    /* Create test directories */
    status = createTestDirectories();
    if (status != 0) {
        return -1;
    }

    /* Create test environment */
    status = createEnvFile();
    if (status != 0) {
        return -1;
    }

    /* Create test configuration */
    status = createTestConfig();
    if (status != 0) {
        return -1;
    }

    return 0;
}

/* Test cases */
void test_system_initialization(void)
{
    int status;

    /* Set environment variables for test */
    setenv("APP_BASE_PATH", "test_tmp", 1);
    setenv("APP_ENV", "test", 1);
    setenv("CONFIG_PATH", "test_tmp/etc/config/system.conf", 1);
    setenv("LOG_PATH", "test_tmp/var/log/app.log", 1);

    status = initSystem();
    CU_ASSERT_EQUAL_FATAL(status, INIT_SUCCESS);

    if (status == INIT_SUCCESS) {
        CU_ASSERT_EQUAL(getSystemState(), STATE_RUNNING);
        shutdownSystem();
    }
}

void
test_config_loading(void)
{
    int status;
    char *config_path = "test_tmp/etc/config/system.conf";

    /* Clean environment first */
    unsetenv("APP_BASE_PATH");
    envCleanup();

    /* Set up test environment */
    status = setenv("APP_BASE_PATH", "test_tmp", 1);
    CU_ASSERT_EQUAL(status, 0);

    /* Initialize environment explicitly */
    status = envInit("test_tmp/etc/config/env");
    CU_ASSERT_EQUAL(status, 0);
    CU_ASSERT_EQUAL(envGetStatus(), ENV_SUCCESS);

    /* Test configuration loading */
    status = loadConfiguration(config_path);
    CU_ASSERT_EQUAL(status, INIT_SUCCESS);

    /* Test invalid paths */
    status = loadConfiguration("/nonexistent/path");
    CU_ASSERT_EQUAL(status, INIT_ERROR);

    status = loadConfiguration(NULL);
    CU_ASSERT_EQUAL(status, INIT_ERROR);

    /* Cleanup */
    envCleanup();
    unsetenv("APP_BASE_PATH");
}

void test_system_shutdown(void)
{
    int status;

    setenv("APP_BASE_PATH", "test_tmp", 1);
    status = initSystem();
    CU_ASSERT_EQUAL_FATAL(status, INIT_SUCCESS);

    status = shutdownSystem();
    CU_ASSERT_EQUAL(status, INIT_SUCCESS);
    CU_ASSERT_EQUAL(getSystemState(), STATE_SHUTDOWN);
}

void test_init_system(void)
{
    int status;

    setenv("APP_BASE_PATH", "test_tmp", 1);
    status = initSystem();

    CU_ASSERT_EQUAL_FATAL(status, INIT_SUCCESS);
    CU_ASSERT_EQUAL(getSystemState(), STATE_RUNNING);

    shutdownSystem();
}

/* Test suite initialization */
int test_init(void)
{
    CU_pSuite suite = NULL;

    /* Create suite with setup/teardown */
    suite = CU_add_suite("Init Module Tests", setup, teardown);
    if (suite == NULL) {
        fprintf(stderr, "Failed to create init test suite\n");
        return -1;
    }

    /* Add test cases */
    if ((CU_add_test(suite, "System Initialization", test_system_initialization) == NULL) ||
        (CU_add_test(suite, "Configuration Loading", test_config_loading) == NULL) ||
        (CU_add_test(suite, "System Shutdown", test_system_shutdown) == NULL) ||
        (CU_add_test(suite, "Init System", test_init_system) == NULL)) {
        fprintf(stderr, "Failed to add init test cases\n");
        return -1;
    }

    return 0;
}
