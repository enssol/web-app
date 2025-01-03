#!/bin/bash

# Load common configuration
. ../config/common-config.sh
. ../utils/shared-utils.sh

# Function to clean build artifacts
clean_build_artifacts() {
    echo "Cleaning build artifacts..."
    rm -rf "${BUILDDIR}" "${DISTDIR}" ./*.egg-info
    echo "Build artifacts cleaned."
}

# Function to remove temporary files
clean_temp_files() {
    echo "Removing temporary files..."
    rm -rf "${TMPDIR}"
    echo "Temporary files removed."
}

# Main function to clean
clean_main() {
    clean_build_artifacts
    clean_temp_files
    reset_build_env
    verify_cleanup
}

# Execute main function
clean_main
