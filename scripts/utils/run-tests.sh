#!/bin/sh

#######################################
# Orchestrate CUnit test runner
#
# Usage: ./run-tests.sh
#######################################

# Strict mode
set -e
set -u

# Load common configuration
. ../config/common-config.sh
. ../utils/compile-flags.sh

# Configure test flags
CFLAGS="${CFLAGS} -O0 -ggdb3 -fprofile-arcs -ftest-coverage"
LDFLAGS="${LDFLAGS} -lgcov -lcunit -lcrypt -lpthread -lrt"

# Build and run tests
make clean
make test CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}"

# Run the test binary
if ! ./bin/test_web_server; then
    printf "Tests failed\n" >&2
    exit 1
fi

# Generate coverage report
gcovr -r . --html --html-details -o coverage.html

# Verify test artifacts
if [ ! -f coverage.html ]; then
    printf "Coverage report not generated\n" >&2
    exit 1
fi

printf "Tests and coverage report completed successfully\n"
