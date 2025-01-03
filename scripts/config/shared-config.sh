#!/bin/sh

# Default values for variables
VERSION="1.0.0"
MAJOR_VERSION="1"
MINOR_VERSION="0"
PATCH_VERSION="0"
CC="gcc"
CFLAGS="-O2"
LDFLAGS="-lm"
SRCDIR="src"
TESTDIR="test"
INCLUDEDIR="include"
LOGDIR="log"
BUILDDIR="build"
BINDIR="bin"
LIBDIR="lib"
MANDIR="man"
DOCSDIR="docs"
CONFIGDIR="config"
SCRIPTDIR="scripts"
CACHEDIR="cache"
PIDDIR="pid"
TMPDIR="tmp"
VARDIR="var"
PRIVATEDIR="private"
PUBLICDIR="public"
DEBUG_DIR="debug"
DEBUG_DEPS_DIR="debug/deps"
DEBUG_OBJ_DIR="debug/obj"
RELEASE_DIR="release"
RELEASE_DEPS_DIR="release/deps"
RELEASE_OBJ_DIR="release/obj"
DIST_DIR="dist"
PKGCONFIG_DIR="pkgconfig"
STATIC_LIB_DIR="static_lib"
MAN1_DIR="man1"
MAN3_DIR="man3"
MAN5_DIR="man5"
MAN8_DIR="man8"
CHARSET_DIR="charset"
CHECKSUMS_DIR="checksums"
ARCHIVE_DIR="archive"
BACKUP_DIR="backup"
VAR_DB_DIR="var/db"
VAR_LOG_DIR="var/log"
VAR_DATA_DIR="var/data"
VAR_RECORDS_DIR="var/records"
SSL_DIR="ssl"
SSL_CERTS_DIR="ssl/certs"
PUBLIC_ASSETS_DIR="public/assets"
PUBLIC_VIEWS_DIR="public/views"
DEPS_FILE="path/to/deps.txt"

# Environment setup
LANG=en_AU.ISO8859-1
LC_ALL=en_AU.ISO8859-1
CHARSET=ISO-8859-1
export LANG LC_ALL CHARSET

# Exit codes
EXIT_SUCCESS=0
EXIT_FAILURE=1

# Known tools
KNOWN_TOOLS="gcc git valgrind shellcheck cppcheck enca doxygen groff bearssl make gdb recsel rec2csv recdel recfix recfmt recinf recins recset ar as ld nm objcopy objdump ranlib strip"

# Build flags
CC="${CC:-gcc}"
PROD_CFLAGS="-std=c90 -D_POSIX_C_SOURCE=1 -D_XOPEN_SOURCE=500 -Wall -Wextra -pedantic -static"
PROD_LDFLAGS="-static -pthread -Wl,-O1 -Wl,--gc-sections -lssl -lcrypto"
TEST_CFLAGS="-O0 -ggdb3 -fprofile-arcs -ftest-coverage"
TEST_LDFLAGS="-lgcov -lcunit -lcrypt -lpthread -lrt"

# Version info - moved from Makefile
VERSION="0.0.2"
MAJOR_VERSION="0"
MINOR_VERSION="0"
PATCH_VERSION="2"

# Build artifact names
PROD_TARGET="web_server"
TEST_TARGET="test_web_server"

# Distribution settings
DIST_NAME="web-app-${VERSION}"
RELEASE_NAME="${DIST_NAME}"

# Export all variables
export CC PROD_CFLAGS PROD_LDFLAGS TEST_CFLAGS TEST_LDFLAGS
export VERSION MAJOR_VERSION MINOR_VERSION PATCH_VERSION
export PROD_TARGET TEST_TARGET DIST_NAME RELEASE_NAME

#######################################
# Generate configuration file
# Arguments:
#   $1 - Input template file
#   $2 - Output config file
#######################################
generate_config() {
    sed -e "s|@VERSION@|${VERSION}|g" \
        -e "s|@MAJOR_VERSION@|${MAJOR_VERSION}|g" \
        -e "s|@MINOR_VERSION@|${MINOR_VERSION}|g" \
        -e "s|@PATCH_VERSION@|${PATCH_VERSION}|g" \
        -e "s|@CC@|${CC}|g" \
        -e "s|@CFLAGS@|${CFLAGS}|g" \
        -e "s|@LDFLAGS@|${LDFLAGS}|g" \
        -e "s|@SRCDIR@|${SRCDIR}|g" \
        -e "s|@TESTDIR@|${TESTDIR}|g" \
        -e "s|@INCLUDEDIR@|${INCLUDEDIR}|g" \
        -e "s|@LOGDIR@|${LOGDIR}|g" \
        -e "s|@BUILDDIR@|${BUILDDIR}|g" \
        -e "s|@BINDIR@|${BINDIR}|g" \
        -e "s|@LIBDIR@|${LIBDIR}|g" \
        -e "s|@MANDIR@|${MANDIR}|g" \
        -e "s|@DOCSDIR@|${DOCSDIR}|g" \
        -e "s|@CONFIGDIR@|${CONFIGDIR}|g" \
        -e "s|@SCRIPTDIR@|${SCRIPTDIR}|g" \
        -e "s|@CACHEDIR@|${CACHEDIR}|g" \
        -e "s|@PIDDIR@|${PIDDIR}|g" \
        -e "s|@TMPDIR@|${TMPDIR}|g" \
        -e "s|@VARDIR@|${VARDIR}|g" \
        -e "s|@PRIVATEDIR@|${PRIVATEDIR}|g" \
        -e "s|@PUBLICDIR@|${PUBLICDIR}|g" \
        -e "s|@DEBUG_DIR@|${DEBUG_DIR}|g" \
        -e "s|@DEBUG_DEPS_DIR@|${DEBUG_DEPS_DIR}|g" \
        -e "s|@DEBUG_OBJ_DIR@|${DEBUG_OBJ_DIR}|g" \
        -e "s|@RELEASE_DIR@|${RELEASE_DIR}|g" \
        -e "s|@RELEASE_DEPS_DIR@|${RELEASE_DEPS_DIR}|g" \
        -e "s|@RELEASE_OBJ_DIR@|${RELEASE_OBJ_DIR}|g" \
        -e "s|@DIST_DIR@|${DIST_DIR}|g" \
        -e "s|@PKGCONFIG_DIR@|${PKGCONFIG_DIR}|g" \
        -e "s|@STATIC_LIB_DIR@|${STATIC_LIB_DIR}|g" \
        -e "s|@MAN1_DIR@|${MAN1_DIR}|g" \
        -e "s|@MAN3_DIR@|${MAN3_DIR}|g" \
        -e "s|@MAN5_DIR@|${MAN5_DIR}|g" \
        -e "s|@MAN8_DIR@|${MAN8_DIR}|g" \
        -e "s|@CHARSET_DIR@|${CHARSET_DIR}|g" \
        -e "s|@CHECKSUMS_DIR@|${CHECKSUMS_DIR}|g" \
        -e "s|@ARCHIVE_DIR@|${ARCHIVE_DIR}|g" \
        -e "s|@BACKUP_DIR@|${BACKUP_DIR}|g" \
        -e "s|@VAR_DB_DIR@|${VAR_DB_DIR}|g" \
        -e "s|@VAR_LOG_DIR@|${VAR_LOG_DIR}|g" \
        -e "s|@VAR_DATA_DIR@|${VAR_DATA_DIR}|g" \
        -e "s|@VAR_RECORDS_DIR@|${VAR_RECORDS_DIR}|g" \
        -e "s|@SSL_DIR@|${SSL_DIR}|g" \
        -e "s|@SSL_CERTS_DIR@|${SSL_CERTS_DIR}|g" \
        -e "s|@PUBLIC_ASSETS_DIR@|${PUBLIC_ASSETS_DIR}|g" \
        -e "s|@PUBLIC_VIEWS_DIR@|${PUBLIC_VIEWS_DIR}|g" \
        "$1" > "$2"
}

# Print error message to stderr
log_error() {
    printf "ERROR: %s\n" "${1}" >&2
}

# Print info message to stdout
log_info() {
    printf "INFO: %s\n" "${1}"
}

# Version comparison function
version_greater_equal() {
    printf '%s\n%s\n' "$1" "$2" | sort -V
}

# Parse dependencies file
parse_deps_file() {
    if [ ! -f "${DEPS_FILE}" ]; then
        log_error "Dependencies file not found: ${DEPS_FILE}"
        return "${EXIT_FAILURE}"
    fi

    while IFS= read -r line; do
        # Skip comments and empty lines
        if [ -z "${line}" ] || [ "${line#\#}" != "${line}" ]; then
            continue
        fi

        case "${line}" in
            "LANG="*)
                REQUIRED_LANG="${line#LANG=}"
                ;;
            "LC_ALL="*)
                REQUIRED_LC_ALL="${line#LC_ALL=}"
                ;;
            "CHARSET="*)
                REQUIRED_CHARSET="${line#CHARSET=}"
                ;;
            *"binutils >= "*)     MIN_BINUTILS=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
            *"gcc >= "*)          MIN_GCC=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
            *"git >= "*)          MIN_GIT=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
            *"valgrind >= "*)     MIN_VALGRIND=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
            *"shellcheck >= "*)   MIN_SHELLCHECK=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
            *"cppcheck >= "*)     MIN_CPPCHECK=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
            *"enca >= "*)         MIN_ENCA=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
            *"doxygen >= "*)      MIN_DOXYGEN=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
            *"groff >= "*)        MIN_GROFF=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
            *"bearssl >= "*)      MIN_BEARSSL=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
            *"make >= "*)         MIN_MAKE=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
            *"gdb >= "*)          MIN_GDB=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
            *"recutils >= "*)     MIN_RECUTILS=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
            *"gh >= "*)           MIN_GH=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
            *)
                # Only log error for non-environment variables
                if ! echo "${line}" | grep -q "^[A-Z_]*="; then
                    tool=$(echo "${line}" | awk '{print $1}')
                    if echo "${KNOWN_TOOLS}" | grep -qw "${tool}"; then
                        log_error "Unknown dependency: ${line}"
                        return "${EXIT_FAILURE}"
                    fi
                fi
                ;;
        esac
    done < "${DEPS_FILE}"

    return "${EXIT_SUCCESS}"
}

# Validate environment settings match deps.txt
validate_deps_env() {
    # Check LANG
    if [ "${LANG}" != "${REQUIRED_LANG}" ]; then
        log_error "LANG must be ${REQUIRED_LANG} (current: ${LANG})"
        return "${EXIT_FAILURE}"
    fi

    # Check LC_ALL
    if [ "${LC_ALL}" != "${REQUIRED_LC_ALL}" ]; then
        log_error "LC_ALL must be ${REQUIRED_LC_ALL} (current: ${LC_ALL})"
        return "${EXIT_FAILURE}"
    fi

    # Check CHARSET
    if [ "${CHARSET}" != "${REQUIRED_CHARSET}" ]; then
        log_error "CHARSET must be ${REQUIRED_CHARSET} (current: ${CHARSET})"
        return "${EXIT_FAILURE}"
    fi

    return "${EXIT_SUCCESS}"
}

# Add version parsing functions
get_valgrind_version() {
    valgrind --version | awk '{print $2}'
}

get_shellcheck_version() {
    shellcheck --version | awk '/version:/ {print $2}'
}

get_cppcheck_version() {
    cppcheck --version | awk '{print $2}'
}

get_enca_version() {
    enca --version | awk '/Version/ {print $2}'
}

get_doxygen_version() {
    doxygen --version
}

get_groff_version() {
    groff --version | awk 'NR==1 {print $4}'
}

get_make_version() {
    make --version | awk 'NR==1 {print $3}'
}

get_gdb_version() {
    gdb --version | awk 'NR==1 {print $4}'
}

get_recutils_version() {
    recsel --version | awk 'NR==1 {print $4}'
}

get_gh_version() {
    gh --version | awk '{print $3}'
}

# Add function to check BearSSL library path
get_bearssl_version() {
    # First try command-line tool
    if command -v bearssl >/dev/null 2>&1; then
        bearssl version | awk '{print $2}'
        return 0
    fi

    # Fall back to checking library paths
    for lib in \
        /lib/libbearssl.so.* \
        /usr/lib/libbearssl.so.* \
        /usr/local/lib/libbearssl.so.*; do
        if [ -f "${lib}" ]; then
            # Extract version from library name
            echo "${lib##*.}"
            return 0
        fi
    done

    return 1
}

# Check versions of tools
check_versions() {
    # Check binutils version
    if ! command -v ar >/dev/null 2>&1 || \
       ! command -v as >/dev/null 2>&1 || \
       ! command -v ld >/dev/null 2>&1; then
        log_error "binutils >= ${MIN_BINUTILS} is required"
        return "${EXIT_FAILURE}"
    fi

    # Check gcc version
    if ! command -v gcc >/dev/null 2>&1; then
        log_error "gcc >= ${MIN_GCC} is required"
        return "${EXIT_FAILURE}"
    fi
    gcc_version=$(gcc -dumpversion)
    if ! version_greater_equal "${gcc_version}" "${MIN_GCC}"; then
        log_error "gcc ${gcc_version} is too old, >= ${MIN_GCC} is required"
        return "${EXIT_FAILURE}"
    fi

    # Check git version
    if ! command -v git >/dev/null 2>&1; then
        log_error "git >= ${MIN_GIT} is required"
        return "${EXIT_FAILURE}"
    fi
    git_version=$(git --version | awk '{print $3}')
    if ! version_greater_equal "${git_version}" "${MIN_GIT}"; then
        log_error "git ${git_version} is too old, >= ${MIN_GIT} is required"
        return "${EXIT_FAILURE}"
    fi

    # Check versions of other tools with optional flag for bearssl
    BEARSSL_REQUIRED=false # Can be set to true if SSL is needed

    for tool in valgrind shellcheck cppcheck enca doxygen groff make gdb recsel; do
        if ! command -v "${tool}" >/dev/null 2>&1; then
            case "${tool}" in
                valgrind)    version="${MIN_VALGRIND}" ;;
                shellcheck)  version="${MIN_SHELLCHECK}" ;;
                cppcheck)    version="${MIN_CPPCHECK}" ;;
                enca)       version="${MIN_ENCA}" ;;
                doxygen)    version="${MIN_DOXYGEN}" ;;
                groff)      version="${MIN_GROFF}" ;;
                make)       version="${MIN_MAKE}" ;;
                gdb)        version="${MIN_GDB}" ;;
                recsel)     version="${MIN_RECUTILS}" ;;
                gh)         version="${MIN_GH}" ;;
                *)          log_error "Unknown tool: ${tool}"; return "${EXIT_FAILURE}" ;;
            esac
            log_error "${tool} >= ${version} is required"
            return "${EXIT_FAILURE}"
        fi

        # Get installed version and check
        case "${tool}" in
            valgrind)    current_version=$(get_valgrind_version) ;;
            shellcheck)  current_version=$(get_shellcheck_version) ;;
            cppcheck)    current_version=$(get_cppcheck_version) ;;
            enca)       current_version=$(get_enca_version) ;;
            doxygen)    current_version=$(get_doxygen_version) ;;
            groff)      current_version=$(get_groff_version) ;;
            make)       current_version=$(get_make_version) ;;
            gdb)        current_version=$(get_gdb_version) ;;
            recsel)     current_version=$(get_recutils_version) ;;
            gh)         current_version=$(get_gh_version) ;;
            *)          log_error "Unknown tool: ${tool}"; return "${EXIT_FAILURE}" ;;
        esac

        # Compare versions
        if ! version_greater_equal "${current_version}" "${version}"; then
            log_error "${tool} ${current_version} is too old, >= ${version} is required"
            return "${EXIT_FAILURE}"
        fi
    done

    # Check BearSSL version
    if command -v bearssl >/dev/null 2>&1 || get_bearssl_version >/dev/null 2>&1; then
        bearssl_version=$(get_bearssl_version)
        if ! version_greater_equal "${bearssl_version}" "${MIN_BEARSSL}"; then
            if [ "${BEARSSL_REQUIRED}" = "true" ]; then
                log_error "BearSSL ${bearssl_version} is too old, >= ${MIN_BEARSSL} is required"
                return "${EXIT_FAILURE}"
            else
                log_info "WARNING: BearSSL ${bearssl_version} is older than recommended version ${MIN_BEARSSL}"
            fi
        fi
    else
        if [ "${BEARSSL_REQUIRED}" = "true" ]; then
            log_error "BearSSL >= ${MIN_BEARSSL} is required for SSL/TLS support"
            return "${EXIT_FAILURE}"
        else
            log_info "WARNING: BearSSL not found. SSL/TLS features will be disabled"
        fi
    fi

    return "${EXIT_SUCCESS}"
}
