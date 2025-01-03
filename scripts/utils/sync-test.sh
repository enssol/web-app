#!/bin/sh

# Define directories
SRC_DIR="src"
INCLUDE_DIR="include"
TEST_DIR="test/unit"

# Function to create test file if it doesn't exist
create_test_file() {
    src_file="$1"
    test_file="$2"

    if [ ! -f "${test_file}" ]; then
        echo "Creating test file: ${test_file}"
        mkdir -p "$(dirname "${test_file}")"
        cat <<EOF > "${test_file}"
/* Test file for $(basename "${src_file}") */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "$(basename "${src_file}" .c).h"

int main(void) {
    printf("Running tests for $(basename "${src_file}")\\n");
    // Add your test cases here
    return 0;
}
EOF
    fi
}

# Sync test files for source files
find "${SRC_DIR}" -type f -name "*.c" | while read -r src_file; do
    test_file="${TEST_DIR}/$(dirname "${src_file#"${SRC_DIR}"/}")/$(basename "${src_file}" .c)_test.c"
    create_test_file "${src_file}" "${test_file}"
done

# Sync test files for header files
find "${INCLUDE_DIR}" -type f -name "*.h" | while read -r include_file; do
    test_file="${TEST_DIR}/$(dirname "${include_file#"${INCLUDE_DIR}"/}")/$(basename "${include_file}" .h)_test.c"
    create_test_file "${include_file}" "${test_file}"
done

echo "Test files sync complete."
