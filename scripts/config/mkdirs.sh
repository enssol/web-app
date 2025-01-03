#!/bin/sh

#######################################
# Create project directory structure
#
# Usage: ./mkdirs.sh
#######################################

# Source common configuration
. ./shared-config.sh

# Function to ensure directory exists
ensure_dir() {
    if [ ! -d "$1" ]; then
        mkdir -p "$1"
    fi
}

# Create directories
ensure_dir "${SRCDIR}"
ensure_dir "${TESTDIR}"
ensure_dir "${INCLUDEDIR}"
ensure_dir "${LOGDIR}"
ensure_dir "${BUILDDIR}"
ensure_dir "${BINDIR}"
ensure_dir "${LIBDIR}"
ensure_dir "${MANDIR}"
ensure_dir "${DOCSDIR}"
ensure_dir "${CONFIGDIR}"
ensure_dir "${SCRIPTDIR}"
ensure_dir "${CACHEDIR}"
ensure_dir "${PIDDIR}"
ensure_dir "${TMPDIR}"
ensure_dir "${VARDIR}"
ensure_dir "${PRIVATEDIR}"
ensure_dir "${PUBLICDIR}"
ensure_dir "${DEBUG_DIR}"
ensure_dir "${DEBUG_DEPS_DIR}"
ensure_dir "${DEBUG_OBJ_DIR}"
ensure_dir "${RELEASE_DIR}"
ensure_dir "${RELEASE_DEPS_DIR}"
ensure_dir "${RELEASE_OBJ_DIR}"
ensure_dir "${DIST_DIR}"
ensure_dir "${PKGCONFIG_DIR}"
ensure_dir "${STATIC_LIB_DIR}"
ensure_dir "${MAN1_DIR}"
ensure_dir "${MAN3_DIR}"
ensure_dir "${MAN5_DIR}"
ensure_dir "${MAN8_DIR}"
ensure_dir "${CHARSET_DIR}"
ensure_dir "${CHECKSUMS_DIR}"
ensure_dir "${ARCHIVE_DIR}"
ensure_dir "${BACKUP_DIR}"
ensure_dir "${VAR_DB_DIR}"
ensure_dir "${VAR_LOG_DIR}"
ensure_dir "${VAR_DATA_DIR}"
ensure_dir "${VAR_RECORDS_DIR}"
ensure_dir "${SSL_DIR}"
ensure_dir "${SSL_CERTS_DIR}"
ensure_dir "${PUBLIC_ASSETS_DIR}"
ensure_dir "${PUBLIC_VIEWS_DIR}"

printf "Directory structure created successfully\n"
