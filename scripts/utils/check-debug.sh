#!/bin/sh

#######################################
# Check debug symbols and verify debug information
#
# Usage: ./check-debug.sh
#######################################

# Strict mode
set -e
set -u

# Load common configuration
. ../config/common-config.sh

# Check if objdump is installed
if ! command -v objdump >/dev/null 2>&1; then
    printf "ERROR: objdump is not installed\n" >&2
    exit 1
fi

# Check if gdb is installed
if ! command -v gdb >/dev/null 2>&1; then
    printf "ERROR: gdb is not installed\n" >&2
    exit 1
fi

# Check debug symbols
printf "Checking debug symbols...\n"
objdump -h bin/test_web_server | grep debug

# Verify debug information
printf "Verifying debug information...\n"
gdb -batch -ex "info sources" -ex "list main" -ex "quit" bin/test_web_server

printf "Debug symbols and information verified successfully\n"
