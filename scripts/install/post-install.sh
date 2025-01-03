#!/bin/sh

#######################################
# Post-installation setup
#
# Usage: ./post-install.sh
#######################################

# Strict mode
set -e
set -u

# Load common configuration and utilities
. ../config/common-config.sh
. ../utils/shared-utils.sh

# Perform post-installation tasks
log_info "Performing post-installation tasks..."

# Ensure log directory exists with correct permissions
ensure_dir "${VAR_LOG_DIR}"
chmod 755 "${VAR_LOG_DIR}"
chown root:root "${VAR_LOG_DIR}"

log_info "Post-installation tasks completed successfully"
