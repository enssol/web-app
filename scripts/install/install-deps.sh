#!/bin/sh

#######################################
# Install project dependencies
#
# Usage: ./install-deps.sh
#######################################

# Strict mode
set -e
set -u

# Load common configuration and utilities
. ../config/common-config.sh
. ../utils/shared-utils.sh

# Install dependencies
log_info "Installing project dependencies..."

# Required dependencies
for dep in cunit bearssl-dev musl-dev; do
    if ! apk info -e "${dep}" >/dev/null 2>&1; then
        log_info "Installing ${dep}..."
        apk add --no-cache "${dep}" || error "Failed to install ${dep}"
    fi
done

log_info "Project dependencies installation completed successfully"
