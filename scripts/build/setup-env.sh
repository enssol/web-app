#!/bin/sh

#######################################
# Setup build environment for project
#
# Usage: ./setup-env.sh
#
# Environment variables:
#   CC        - C compiler to use (defaults to gcc)
#   CFLAGS    - C compiler flags
#   LDFLAGS   - Linker flags
#######################################

# Source common configuration
. ../config/shared-config.sh

# Function to check for root user
check_root_user() {
    user_id=$(id -u) || log_error "Failed to get user ID"
    if [ "${user_id}" = "0" ]; then
        log_error "This script should not be run as root"
        exit 1
    fi
}

# Function to validate environment
validate_environment() {
    validate_env
}

# Function to generate build config
generate_build_config() {
    generate_config "${CONFIG_TEMPLATE}" "${BUILDDIR}/config.h"
}

# Function to log setup completion
log_setup_completion() {
    current_time=$(date) || log_error "Failed to get current time"
    mkdir -p "${LOGDIR}"
    {
        printf "Build environment setup completed at %s\n" "${current_time}"
        printf "Using compiler: %s\n" "${CC}"
        printf "Version: %s\n" "${VERSION}"
    } >> "${LOGDIR}/setup.log"
    printf "Build environment setup completed successfully\n"
}

# Main function to setup build environment
setup_env() {
    check_root_user
    validate_environment
    generate_build_config
    log_setup_completion
}

#######################################
# Main
#######################################

setup_env

# vim: set ft=sh ts=4 sw=4 et:
