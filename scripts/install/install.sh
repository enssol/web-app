#!/bin/sh

#######################################
# Install application
#
# Usage: ./install.sh
#######################################

# Strict mode
set -e
set -u

# Load common configuration and utilities
. ../config/common-config.sh
. ../utils/shared-utils.sh

# Define and validate prefix
PREFIX="/usr/local"
ensure_dir "${PREFIX}/bin"
ensure_dir "${PREFIX}/etc"

# Check permissions
if [ ! -w "${PREFIX}/bin" ] || [ ! -w "${PREFIX}/etc" ]; then
    error "Insufficient permissions for installation directories"
fi

# Install binary
install -m 755 "${BINDIR}/web-app" "${PREFIX}/bin/" || error "Failed to install binary"

# Install configuration
install -m 644 "${CONFIGDIR}/web-app.conf" "${PREFIX}/etc/" || error "Failed to install config"

# Verify installation
if [ ! -x "${PREFIX}/bin/web-app" ]; then
    error "Installation verification failed"
fi

log_info "Installation completed successfully"
