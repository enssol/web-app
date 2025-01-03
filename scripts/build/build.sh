#!/bin/sh

#######################################
# Main build orchestration script
#
# Usage: ./build.sh [command]
# Commands:
#   setup-env   - Setup build environment
#   make-debug  - Create debug build
#   make-release - Create release build
#   make-dist   - Create distribution package
#   clean       - Clean build artifacts
#######################################

# Strict mode
set -e
set -u

# Load common configuration
. ../config/common-config.sh
. ../utils/compile-flags.sh

# Define DEPFILES variable
DEPFILES="*.d"

# Function to setup build environment
setup_env() {
    # Check for root user
    user_id=$(id -u) || error "Failed to get user ID"
    if [ "${user_id}" = "0" ]; then
        error "This script should not be run as root"
    fi

    # Validate environment
    validate_env

    # Create build config
    generate_config "config/mkconfig/config.h.in" "${BUILDDIR}/config.h"

    # Create directories
    ./config/mkdirs.sh

    # Get current date/time
    current_time=$(date) || error "Failed to get current time"

    # Log setup completion
    mkdir -p "${LOGDIR}"
    {
        printf "Build environment setup completed at %s\n" "${current_time}"
        printf "Using compiler: %s\n" "${CC}"
        printf "Version: %s\n" "${VERSION}"
    } >> "${LOGDIR}/setup.log"

    printf "Build environment setup completed successfully\n"
}

# Function to create debug build
make_debug() {
    make clean
    make all
    printf "Debug build completed successfully\n"
}

# Function to create release build
make_release() {
    make clean
    make all
    ./generate-checksums.sh
    ./verify-checksum.sh
    printf "Release build completed successfully\n"
}

# Function to create distribution package
make_dist() {
    ./verify-checksum.sh
    DIST_FILE="${DIST_NAME}.tar.gz"
    tar -czf "${DIST_FILE}" -C "${BUILDDIR}" .
    printf "Distribution package created: %s\n" "${DIST_FILE}"
}

# Function to clean build artifacts
clean() {
    make clean
    rm -f "${DEPFILES}"
    printf "Clean completed successfully\n"
}

# Main
case "$1" in
    setup-env)
        setup_env
        ;;
    make-debug)
        make_debug
        ;;
    make-release)
        make_release
        ;;
    make-dist)
        make_dist
        ;;
    clean)
        clean
        ;;
    *)
        echo "Usage: $0 {setup-env|make-debug|make-release|make-dist|clean}"
        exit 1
        ;;
esac

# vim: set ft=sh ts=4 sw=4 et:
