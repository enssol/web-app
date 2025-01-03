#!/bin/sh

#######################################
# Create distribution package
#
# Usage: ./make-dist.sh
#######################################

# Strict mode
set -e
set -u

# Load common configuration
. ../config/common-config.sh

# Function to verify checksums
verify_checksums() {
    ./verify-checksum.sh
}

# Function to create distribution package
create_dist_package() {
    DIST_FILE="${DIST_NAME}-${VERSION}.tar.gz"
    tar -czf "${DIST_FILE}" -C "${BUILDDIR}" .
    printf "Distribution package created: %s\n" "${DIST_FILE}"
}

# Main function to create distribution package
make_dist() {
    verify_checksums
    create_dist_package
}

# Execute main function
make_dist
