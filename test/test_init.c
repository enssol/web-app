/* filepath: test/test_init.c */
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../include/init.h"
#include "../include/app_error.h"
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
    "test_tmp/etc/config",
    "test_tmp/var/log",
    "test_tmp/var/run",
    NULL
};

/* Forward declarations of helper functions */
static int createTestDirectories(void) __attribute__((unused));
static int createEnvFile(void) __attribute__((unused));
static int createTestConfig(void) __attribute__((unused));

static int createEnvFile(void)
{
    FILE *fp;
    const char *env_path = "test_tmp/etc/config/env";

    fp = fopen(env_path, "w");
    if (fp == NULL) {
        return -1;
    }

    /* Write test environment variables */
    fprintf(fp, "APP_NAME=TestApp\n");
    fprintf(fp, "APP_VERSION=1.0.0\n");
    fprintf(fp, "APP_ENV=test\n");
    fprintf(fp, "LOG_PATH=test_tmp/var/log/app.log\n");
    fprintf(fp, "LOG_LEVEL=debug\n");
    fclose(fp);

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

    /* Set restrictive file permissions */
    old_umask = umask(077);

    /* Create and open config file */
    fp = fopen(config_path, "w");
    if (fp == NULL) {
        umask(old_umask);
        return -1;
    }

    /* Write test configuration */
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
    fprintf(fp, "DB_USER=test\n");
    fprintf(fp, "DB_PASSWORD=test\n");
    fprintf(fp, "CACHE_DRIVER=memory\n");
    fprintf(fp, "CACHE_PREFIX=test_\n");
    fprintf(fp, "CACHE_TTL=300\n");

    fclose(fp);

    /* Restore original umask */
    umask(old_umask);

    /* Create environment file with same settings */
    return createEnvFile();
}

static int teardown(void)
{
    const char *paths[] = {
        "test_tmp/etc/config/system.conf",
        "test_tmp/etc/config/env",
        "test_tmp/etc/config",
        "test_tmp/etc",
        "test_tmp/var/log/app.log",
        "test_tmp/var/log",
        "test_tmp/var/run",
        "test_tmp/var",
        "test_tmp",
        NULL
    };
    const char **path;

    for (path = paths; *path != NULL; path++) {
        remove(*path);
        rmdir(*path);
    }
    return 0;
}

/* Test fixture setup/teardown functions */
static int setup(void)
{
    const char **dir;
    mode_t old_umask = umask(022);

    /* Create test root */
    mkdir("test_tmp", 0755);

    /* Create test directories */
    for (dir = test_dirs; *dir != NULL; dir++) {
        if (mkdir(*dir, 0755) != 0 && errno != EEXIST) {
            return -1;
        }
    }

    umask(old_umask);
    return 0;
}

/* Test cases */
void test_system_initialization(void)
{
    setenv("CONFIG_PATH", "test_tmp/etc/config/system.conf", 1);
    setenv("LOG_PATH", "test_tmp/var/log/app.log", 1);

    CU_ASSERT_EQUAL(initSystem(), INIT_SUCCESS);
    CU_ASSERT_EQUAL(getSystemState(), STATE_RUNNING);
    shutdownSystem();
}

void test_config_loading(void)
{
    CU_ASSERT_EQUAL(loadConfiguration("test_tmp/etc/config/system.conf"), INIT_SUCCESS);
    CU_ASSERT_EQUAL(loadConfiguration("/nonexistent/path"), INIT_ERROR);
    CU_ASSERT_EQUAL(loadConfiguration(NULL), INIT_ERROR);
}

void test_system_shutdown(void)
{
    initSystem();
    CU_ASSERT_EQUAL(shutdownSystem(), INIT_SUCCESS);
    CU_ASSERT_EQUAL(getSystemState(), STATE_SHUTDOWN);
}

void test_init_system(void)
{
    int status = initSystem();
    CU_ASSERT_EQUAL(status, INIT_SUCCESS);
    CU_ASSERT_EQUAL(getSystemState(), STATE_RUNNING);
}

/* Test suite initialization */
int test_init(void)
{
    CU_pSuite suite;

    suite = CU_add_suite("Init Module Tests", setup, teardown);
    if (suite == NULL) {
        return -1;
    }

    if ((CU_add_test(suite, "System Initialization", test_system_initialization) == NULL) ||
        (CU_add_test(suite, "Configuration Loading", test_config_loading) == NULL) ||
        (CU_add_test(suite, "System Shutdown", test_system_shutdown) == NULL) ||
        (CU_add_test(suite, "Init System", test_init_system) == NULL)) {
        return -1;
    }

    return 0;
}
