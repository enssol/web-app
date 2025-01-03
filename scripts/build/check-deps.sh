#!/bin/sh

#######################################
# Check development dependencies
#
# Usage: ./check-deps.sh [--help]
#
# Options:
#   --help    Display help message
#######################################

# Strict mode
set -e

# Load common configuration
. ../config/shared-config.sh

#######################################
# Functions
#######################################

# Print error message to stderr
log_error() {
    printf "ERROR: %s\n" "${1}" >&2
}

# Print info message to stdout
log_info() {
    printf "INFO: %s\n" "${1}"
}

# Add global variables for environment requirements
REQUIRED_LANG="${REQUIRED_LANG:-en_AU.ISO8859-1}"
REQUIRED_LC_ALL="${REQUIRED_LC_ALL:-en_AU.ISO8859-1}"
REQUIRED_CHARSET="${REQUIRED_CHARSET:-ISO-8859-1}"

# Update parse_deps_file function
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

# Add this helper function for version comparison
version_greater_equal() {
    printf '%s\n%s\n' "$1" "$2" | sort -V
}

#######################################
# Validate environment settings match deps.txt
# Returns:
#   0 if environment matches, 1 if not
#######################################
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

#######################################
# Display help message
#######################################
show_help() {
    cat << EOF
Usage: ${0} [--help]

Check development dependencies and environment settings against deps.txt requirements.

Options:
  --help    Display this help message and exit

Environment:
  LANG      Must be en_AU.ISO8859-1
  LC_ALL    Must be en_AU.ISO8859-1
  CHARSET   Must be ISO-8859-1
EOF
}

#######################################
# Main execution
#######################################
main() {
    if [ "$1" = "--help" ]; then
        show_help
        exit "${EXIT_SUCCESS}"
    fi

    log_info "Checking installed development tools and versions..."

    # Parse deps file
    if ! parse_deps_file; then
        exit "${EXIT_FAILURE}"
    fi

    # Validate environment settings
    if ! validate_deps_env; then
        log_error "Environment settings do not match deps.txt requirements"
        exit "${EXIT_FAILURE}"
    fi

    # Check tool versions
    if ! check_versions; then
        exit "${EXIT_FAILURE}"
    fi

    log_info "All dependency checks passed successfully"
    exit "${EXIT_SUCCESS}"
}

# Execute main directly (POSIX-compliant)
case ${0##*/} in
    check-deps.sh) main "$@" ;;
    *) log_error "Script must be run as check-deps.sh"; exit "${EXIT_FAILURE}" ;;
esac

# vim: set ft=sh ts=4 sw=4 et:
