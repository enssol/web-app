#!/bin/sh

#######################################
# Create release build
#
# Usage: ./make-release.sh
#######################################

# Strict mode
set -e
set -u

# Load common configuration
. ../config/common-config.sh
. ../utils/compile-flags.sh

# Function to clean and build
clean_and_build() {
    make clean
    make all
}

# Function to generate and verify checksums
generate_and_verify_checksums() {
    if ! ./generate-checksums.sh; then
        log_error "Failed to generate checksums"
        exit 1
    fi

    if ! ./verify-checksum.sh; then
        log_error "Failed to verify checksums"
        exit 1
    fi
}

# Main function to create release build
make_release() {
    clean_and_build
    generate_and_verify_checksums
    printf "Release build and checksums completed successfully\n"
}

# Execute main function
make_release
