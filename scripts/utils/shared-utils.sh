#!/bin/sh

#######################################
# Shared utility functions
#######################################

# Source shared configuration
. "../config/shared-config.sh"

# Enhanced error handling with logging
error() {
    log_error "$1" >&2
    exit 1
}

# Unified logging with levels
log_error() {
    printf "[ERROR] %s\n" "$1"
}

log_info() {
    printf "[INFO] %s\n" "$1"
}

log_debug() {
    if [ "${DEBUG:-0}" = "1" ]; then
        printf "[DEBUG] %s\n" "$1"
    fi
}

# Check if a command exists
check_cmd() {
    command -v "$1" >/dev/null 2>&1 || error "Required command $1 not found"
}

# Ensure a directory exists
ensure_dir() {
    [ -d "$1" ] || mkdir -p "$1" || error "Could not create directory $1"
}

# Version comparison logic
compare_versions() {
    if [ "$1" = "$2" ]; then
        return 0
    fi

    IFS=.
    set -- "$1"
    ver1_major=$1
    ver1_minor=$2
    ver1_patch=$3

    set -- "$2"
    ver2_major=$1
    ver2_minor=$2
    ver2_patch=$3

    if [ "${ver1_major}" -gt "${ver2_major}" ]; then
        return 1
    elif [ "${ver1_major}" -lt "${ver2_major}" ]; then
        return 2
    fi

    if [ "${ver1_minor}" -gt "${ver2_minor}" ]; then
        return 1
    elif [ "${ver1_minor}" -lt "${ver2_minor}" ]; then
        return 2
    fi

    if [ "${ver1_patch}" -gt "${ver2_patch}" ]; then
        return 1
    elif [ "${ver1_patch}" -lt "${ver2_patch}" ]; then
        return 2
    fi

    return 0
}

# Environment validation
validate_env() {
    # Check required commands
    check_cmd "${CC}"
    check_cmd "make"

    # Validate base directories
    ensure_dir "${SRCDIR}"
    ensure_dir "${TESTDIR}"
    ensure_dir "${BINDIR}"
    ensure_dir "${INCLUDEDIR}"
    ensure_dir "${LOGDIR}"
    ensure_dir "${CONFIGDIR}"
    ensure_dir "${SCRIPTDIR}"

    # Check compiler works
    if ! "${CC}" -v >/dev/null 2>&1; then
        error "Compiler ${CC} is not working"
    fi
}

# Reset build environment
reset_build_env() {
    log_info "Resetting build environment..."
    rm -rf build/ dist/ tmp/
    log_info "Build environment reset."
}

# Verify cleanup success
verify_cleanup() {
    if [ ! -d build ] && [ ! -d dist ] && [ ! -d tmp ]; then
        log_info "Cleanup verified successfully."
    else
        error "Cleanup verification failed."
    fi
}
