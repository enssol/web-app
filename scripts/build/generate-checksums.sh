#!/bin/sh

# Source shared utilities
. ../config/shared-config.sh
. ../utils/checksum-utils.sh

# Function to set directories and files
set_directories_and_files() {
    RELEASE_DIR="${RELEASE_DIR:-/home/appuser/web-app/release}"
    LIBS_DIR="${LIBS_DIR:-/home/appuser/web-app/libs}"
    CHECKSUMS_DIR="${CHECKSUMS_DIR:-/home/appuser/web-app/checksums}"
    RELEASE_CHECKSUM_FILE="${CHECKSUMS_DIR}/release.sha256"
    LIBS_CHECKSUM_FILE="${CHECKSUMS_DIR}/libs.sha256"
    TMP_RELEASE_CHECKSUM_FILE="${RELEASE_CHECKSUM_FILE}.tmp"
    TMP_LIBS_CHECKSUM_FILE="${LIBS_CHECKSUM_FILE}.tmp"
}

# Function to cleanup temporary files
cleanup() {
    rm -f "${TMP_RELEASE_CHECKSUM_FILE}" "${TMP_LIBS_CHECKSUM_FILE}"
}

# Function to generate and verify checksums
generate_and_verify_checksums() {
    if generate_checksums "${RELEASE_DIR}" "${TMP_RELEASE_CHECKSUM_FILE}" && \
       verify_checksum_file "${TMP_RELEASE_CHECKSUM_FILE}" && \
       generate_checksums "${LIBS_DIR}" "${TMP_LIBS_CHECKSUM_FILE}" && \
       verify_checksum_file "${TMP_LIBS_CHECKSUM_FILE}"; then
        mv "${TMP_RELEASE_CHECKSUM_FILE}" "${RELEASE_CHECKSUM_FILE}"
        mv "${TMP_LIBS_CHECKSUM_FILE}" "${LIBS_CHECKSUM_FILE}"
        echo "Checksum files updated successfully."
    else
        echo "Failed to generate checksums." >&2
        exit 1
    fi
}

# Main function to generate checksums
generate_checksums_main() {
    set_directories_and_files
    trap cleanup EXIT INT TERM
    generate_and_verify_checksums
}

# Execute main function
generate_checksums_main
