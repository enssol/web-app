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

# Strict mode
set -e  # Exit on error
set -u  # Exit on undefined variables

#######################################
# Constants
#######################################

# Version info
VERSION_FILE="VERSION"
if [ -f "${VERSION_FILE}" ]; then
    VERSION=$(cat "${VERSION_FILE}")
else
    VERSION="0.0.2"
fi

# Parse version components
MAJOR_VERSION=$(echo "${VERSION}" | cut -d. -f1)
MINOR_VERSION=$(echo "${VERSION}" | cut -d. -f2)
PATCH_VERSION=$(echo "${VERSION}" | cut -d. -f3)

# Export version info
export VERSION
export MAJOR_VERSION
export MINOR_VERSION
export PATCH_VERSION

# Base directories
export SRCDIR="src"
export TESTDIR="test"
export INCLUDEDIR="include"
export LOGDIR="var/log/scripts/build"
export BUILDDIR="build"
export BINDIR="bin"
export LIBDIR="lib"
export MANDIR="man"
export DOCSDIR="docs"
export CONFIGDIR="config"
export SCRIPTDIR="scripts"
export CACHEDIR="cache"
export PIDDIR="pid"
export TMPDIR="tmp"
export VARDIR="var"
export PRIVATEDIR="private"
export PUBLICDIR="public"

# Build directories
export DEBUG_DIR="${BUILDDIR}/debug"
export RELEASE_DIR="${BUILDDIR}/release"
export DEBUG_DEPS_DIR="${DEBUG_DIR}/deps"
export DEBUG_OBJ_DIR="${DEBUG_DIR}/obj"
export RELEASE_DEPS_DIR="${RELEASE_DIR}/deps"
export RELEASE_OBJ_DIR="${RELEASE_DIR}/obj"
export DIST_DIR="${BUILDDIR}/dist"

# Library directories
export PKGCONFIG_DIR="${LIBDIR}/pkgconfig"
export STATIC_LIB_DIR="${LIBDIR}/static"

# Documentation directories
export MAN1_DIR="${MANDIR}/man1"
export MAN3_DIR="${MANDIR}/man3"
export MAN5_DIR="${MANDIR}/man5"
export MAN8_DIR="${MANDIR}/man8"

# Data directories
export CHARSET_DIR="charset"
export CHECKSUMS_DIR="checksums"
export ARCHIVE_DIR="archive"
export BACKUP_DIR="backup"

# Runtime directories
export VAR_DB_DIR="${VARDIR}/db"
export VAR_LOG_DIR="${VARDIR}/log"
export VAR_DATA_DIR="${VARDIR}/data"
export VAR_RECORDS_DIR="${VARDIR}/records"

# SSL directories
export SSL_DIR="${PRIVATEDIR}/ssl"
export SSL_CERTS_DIR="${SSL_DIR}/certs"

# Public web directories
export PUBLIC_ASSETS_DIR="${PUBLICDIR}/assets"
export PUBLIC_VIEWS_DIR="${PUBLICDIR}/views"

# Compiler settings
: "${CC:=gcc}"
: "${CFLAGS:=-std=c90 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=500 -Wall -ansi -Wextra -pedantic}"
: "${LDFLAGS:=-static -pthread}"

export CC
export CFLAGS
export LDFLAGS

#######################################
# Functions
#######################################

# Print error message and exit
error() {
    printf "Error: %s\n" "$1" >&2
    exit 1
}

# Check if a command exists
check_cmd() {
    command -v "$1" >/dev/null 2>&1 || error "Required command $1 not found"
}

# Create directory if it doesn't exist
ensure_dir() {
    [ -d "$1" ] || mkdir -p "$1" || error "Could not create directory $1"
}

# Setup build infrastructure
setup_build_dirs() {
    # Create all required build directories
    ensure_dir "${BUILDDIR}"
    ensure_dir "${DEBUG_DIR}"
    ensure_dir "${DEBUG_DEPS_DIR}"
    ensure_dir "${DEBUG_OBJ_DIR}"
    ensure_dir "${RELEASE_DIR}"
    ensure_dir "${RELEASE_DEPS_DIR}"
    ensure_dir "${RELEASE_OBJ_DIR}"
    ensure_dir "${DIST_DIR}"

    # Create library directories
    ensure_dir "${LIBDIR}"
    ensure_dir "${PKGCONFIG_DIR}"
    ensure_dir "${STATIC_LIB_DIR}"

    # Create documentation directories
    ensure_dir "${DOCSDIR}"
    ensure_dir "${MAN1_DIR}"
    ensure_dir "${MAN3_DIR}"
    ensure_dir "${MAN5_DIR}"
    ensure_dir "${MAN8_DIR}"

    # Create data directories
    ensure_dir "${CHARSET_DIR}"
    ensure_dir "${CHECKSUMS_DIR}"
    ensure_dir "${ARCHIVE_DIR}"
    ensure_dir "${BACKUP_DIR}"

    # Create runtime directories
    ensure_dir "${VAR_DB_DIR}"
    ensure_dir "${VAR_LOG_DIR}"
    ensure_dir "${VAR_DATA_DIR}"
    ensure_dir "${VAR_RECORDS_DIR}"

    # Create SSL directories
    ensure_dir "${SSL_DIR}"
    ensure_dir "${SSL_CERTS_DIR}"

    # Create public web directories
    ensure_dir "${PUBLIC_ASSETS_DIR}"
    ensure_dir "${PUBLIC_VIEWS_DIR}"

    # Create temporary directories
    ensure_dir "${TMPDIR}"
    ensure_dir "${CACHEDIR}"
    ensure_dir "${PIDDIR}"
}

# Validate environment
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

    # Setup all other directories
    setup_build_dirs

    # Check compiler works
    if ! "${CC}" -v >/dev/null 2>&1; then
        error "Compiler ${CC} is not working"
    fi
}

# Setup build environment
setup_env() {
    # Check for root user
    user_id=$(id -u) || error "Failed to get user ID"
    if [ "${user_id}" = "0" ]; then
        error "This script should not be run as root"
    fi

    # Validate environment
    validate_env

    # Create build config
    cat > "${BUILDDIR}/config.mk" << EOF
# Generated by setup-env.sh
VERSION := ${VERSION}
MAJOR_VERSION := ${MAJOR_VERSION}
MINOR_VERSION := ${MINOR_VERSION}
PATCH_VERSION := ${PATCH_VERSION}

CC := ${CC}
CFLAGS := ${CFLAGS}
LDFLAGS := ${LDFLAGS}

# Base directories
SRCDIR := ${SRCDIR}
TESTDIR := ${TESTDIR}
INCLUDEDIR := ${INCLUDEDIR}
LOGDIR := ${LOGDIR}
BUILDDIR := ${BUILDDIR}
BINDIR := ${BINDIR}
LIBDIR := ${LIBDIR}
MANDIR := ${MANDIR}
DOCSDIR := ${DOCSDIR}
CONFIGDIR := ${CONFIGDIR}
SCRIPTDIR := ${SCRIPTDIR}
CACHEDIR := ${CACHEDIR}
PIDDIR := ${PIDDIR}
TMPDIR := ${TMPDIR}
VARDIR := ${VARDIR}
PRIVATEDIR := ${PRIVATEDIR}
PUBLICDIR := ${PUBLICDIR}

# Build directories
DEBUG_DIR := ${DEBUG_DIR}
DEBUG_DEPS_DIR := ${DEBUG_DEPS_DIR}
DEBUG_OBJ_DIR := ${DEBUG_OBJ_DIR}
RELEASE_DIR := ${RELEASE_DIR}
RELEASE_DEPS_DIR := ${RELEASE_DEPS_DIR}
RELEASE_OBJ_DIR := ${RELEASE_OBJ_DIR}
DIST_DIR := ${DIST_DIR}

# Library directories
PKGCONFIG_DIR := ${PKGCONFIG_DIR}
STATIC_LIB_DIR := ${STATIC_LIB_DIR}

# Documentation directories
MAN1_DIR := ${MAN1_DIR}
MAN3_DIR := ${MAN3_DIR}
MAN5_DIR := ${MAN5_DIR}
MAN8_DIR := ${MAN8_DIR}

# Data directories
CHARSET_DIR := ${CHARSET_DIR}
CHECKSUMS_DIR := ${CHECKSUMS_DIR}
ARCHIVE_DIR := ${ARCHIVE_DIR}
BACKUP_DIR := ${BACKUP_DIR}

# Runtime directories
VAR_DB_DIR := ${VAR_DB_DIR}
VAR_LOG_DIR := ${VAR_LOG_DIR}
VAR_DATA_DIR := ${VAR_DATA_DIR}
VAR_RECORDS_DIR := ${VAR_RECORDS_DIR}

# SSL directories
SSL_DIR := ${SSL_DIR}
SSL_CERTS_DIR := ${SSL_CERTS_DIR}

# Public web directories
PUBLIC_ASSETS_DIR := ${PUBLIC_ASSETS_DIR}
PUBLIC_VIEWS_DIR := ${PUBLIC_VIEWS_DIR}
EOF

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

#######################################
# Main
#######################################

setup_env

# vim: set ft=sh ts=4 sw=4 et:
