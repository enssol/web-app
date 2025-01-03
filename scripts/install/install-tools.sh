#!/bin/sh

#######################################
# Install development tools
#
# Usage: ./install-tools.sh
#######################################

# Strict mode
set -e
set -u

# Load common configuration and utilities
. ../config/common-config.sh
. ../utils/shared-utils.sh

# Install tools
log_info "Installing development tools..."

# Required tools check
for tool in shellcheck cppcheck enca; do
    if ! command -v "${tool}" >/dev/null 2>&1; then
        log_info "Installing ${tool}..."
        apk add --no-cache "${tool}" || error "Failed to install ${tool}"
    fi
done

log_info "Development tools installation completed successfully"
