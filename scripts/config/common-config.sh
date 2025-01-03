#!/bin/sh

#######################################
# Common configuration for build scripts
#######################################

# Strict mode
set -e
set -u

# Source shared utilities
. ../utils/shared-utils.sh

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

# Source machine-specific settings
. ../../config/build/host.h

# Source compiler flags
. ../../config/build/gcc.specs

# Validate environment
validate_env
