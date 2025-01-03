#!/bin/sh

#######################################
# Run tests
#
# Usage: ./test.sh
#######################################

# Strict mode
set -e
set -u

# Load common configuration
. ../config/common-config.sh
. ../utils/compile-flags.sh

# Build and run tests
make clean
make test

printf "Tests completed successfully\n"
