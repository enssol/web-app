#!/bin/sh

#######################################
# Check code style using shellcheck
#
# Usage: ./check-style.sh
#######################################

# Strict mode
set -e
set -u

# Load common configuration
. ../config/common-config.sh

# Check if shellcheck is installed
if ! command -v shellcheck >/dev/null 2>&1; then
    printf "ERROR: shellcheck is not installed\n" >&2
    exit 1
fi

# Define directories to check
SCRIPT_DIRS="scripts utils"

# Run shellcheck on all shell scripts
for dir in ${SCRIPT_DIRS}; do
    find "${dir}" -type f -name "*.sh" | while read -r script; do
        printf "Checking %s\n" "${script}"
        shellcheck "${script}"
    done
done

printf "Code style check completed successfully\n"
