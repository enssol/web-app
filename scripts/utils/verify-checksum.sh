#!/bin/sh

# Source shared utilities and config
. ../config/shared-config.sh  # Load paths first
. ./checksum-utils.sh

# Define checksum file variables
RELEASE_CHECKSUM_FILE="path/to/release_checksum_file"
LIBS_CHECKSUM_FILE="path/to/libs_checksum_file"

# Use paths from shared config
validate_checksum_paths

# Track verification results
failures=0

# Verify each checksum file
for checksum_file in "${RELEASE_CHECKSUM_FILE}" "${LIBS_CHECKSUM_FILE}"; do
    if ! verify_checksum_file "${checksum_file}"; then
        failures=$((failures + 1))
        log_error "Failed verifying: ${checksum_file}"
    fi
done

# Return overall status
if [ "${failures}" -gt 0 ]; then
    log_error "${failures} checksum verification(s) failed"
    exit 1
fi

log_info "All checksums verified successfully"
exit 0
