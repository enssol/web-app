#!/bin/sh

# Source all script components
. ./config/config_loader.sh
. ./session/session_manager.sh
. ./security/security_validator.sh
. ./monitoring/system_monitor.sh

# Initialize application
init_application() {
    # Load main configuration
    load_config "/etc/webapp/config.ini" || {
        echo "Failed to load configuration"
        exit 1
    }

    # Start system monitoring
    monitor_system

    # Log initialization
    echo "$(date): Application initialized successfully" >> "/var/log/webapp/events.log"
}

# Run initialization
init_application
