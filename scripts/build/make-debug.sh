#!/bin/sh

#######################################
# Create debug build
#
# Usage: ./make-debug.sh
#######################################

# Strict mode
set -e
set -u

# Load common configuration
. ../config/common-config.sh
. ../utils/compile-flags.sh

# Function to configure debug flags
configure_debug_flags() {
    CFLAGS="${CFLAGS} ${DEBUG_FLAGS}"
}

# Function to clean and build with debug settings
clean_and_build_debug() {
    make clean
    make all CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}"
}

# Main function to create debug build
make_debug() {
    configure_debug_flags
    clean_and_build_debug
    printf "Debug build completed successfully\n"
}

# Execute main function
make_debug
