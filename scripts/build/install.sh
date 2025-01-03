#!/bin/sh

#######################################
# Install application
#
# Usage: ./install.sh
#######################################

# Strict mode
set -e
set -u

# Load environment
. ./setup-env.sh

# Load common configuration
. ../config/common-config.sh

# Define PREFIX variable
PREFIX="/usr/local"

# Function to check system requirements
check_system_requirements() {
    check_cmd "install"
}

# Function to install files to system
install_files() {
    install -d "${PREFIX}/bin"
    install -m 755 "${BINDIR}/web-app" "${PREFIX}/bin/"
}

# Main function to install application
install_app() {
    check_system_requirements
    install_files
    printf "Installation completed successfully\n"
}

# Execute main function
install_app
