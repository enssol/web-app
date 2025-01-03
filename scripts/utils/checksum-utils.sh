#!/bin/sh

# Exit immediately if a command exits with a non-zero status
set -e

# Source shared utilities for logging and error handling
. ./shared-utils.sh

# Function to generate checksums
generate_checksums() {
    dir="$1"
    output_file="$2"

    # Check if the directory exists
    if [ ! -d "${dir}" ]; then
        error "Directory ${dir} not found."
    fi

    # Generate checksums and write to the output file
    if ! find "${dir}" -type f -exec sha256sum {} + > "${output_file}"; then
        error "Failed to generate checksums for ${dir}."
    fi

    log_info "Generated checksums for ${dir} in ${output_file}"
    return 0
}

# Validate checksum paths
validate_checksum_paths() {
    if [ ! -d "${CHECKSUMS_DIR}" ]; then
        error "Checksums directory not found: ${CHECKSUMS_DIR}"
    fi

    if [ ! -w "${CHECKSUMS_DIR}" ]; then
        error "Checksums directory not writable: ${CHECKSUMS_DIR}"
    fi
}

# Enhanced checksum verification
verify_checksum_file() {
    checksum_file="$1"

    # Check file exists and is readable
    if [ ! -f "${checksum_file}" ] || [ ! -r "${checksum_file}" ]; then
        error "Cannot read checksum file: ${checksum_file}"
    fi

    # Verify checksums with detailed error reporting
    if ! sha256sum -c "${checksum_file}" 2>/dev/null; then
        log_error "Checksum verification failed for: ${checksum_file}"
        return 1
    fi

    log_info "Verified checksums in: ${checksum_file}"
    return 0
}
