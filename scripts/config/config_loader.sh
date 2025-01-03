#!/bin/sh

# Load and validate configuration
load_config() {
    config_file="${1}"
    
    # Check file exists and is readable
    [ -r "${config_file}" ] || return 1
    
    # Parse INI-style config
    while IFS='=' read -r key value; do
        # Skip comments and empty lines
        case "${key}" in
            ""|\#*) continue ;;
        esac
        # Clean value of any trailing whitespace
        value=$(echo "${value}" | sed 's/[[:space:]]*$//')
        # Export values
        export "${key}=${value}"
    done < "${config_file}"
}
