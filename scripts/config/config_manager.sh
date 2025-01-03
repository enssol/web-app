#!/bin/sh

# Ensure config directory exists
[ -d "/etc/webapp" ] || mkdir -p "/etc/webapp"

# Load and validate configuration
load_config() {
    config_file="${1}"

    # Check file exists and is readable
    [ -r "${config_file}" ] || {
        printf "Error: Config file %s not readable\n" "${config_file}" >&2
        return 1
    }

    # Parse INI-style config with basic validation
    while IFS='=' read -r key value; do
        # Skip comments and empty lines
        case "${key}" in
            ""|\#*) continue ;;
        esac

        # Trim whitespace from key and value
        key=$(printf '%s' "${key}" | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')
        value=$(printf '%s' "${value}" | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')

        # Validate key format
        case "${key}" in
            *[!a-zA-Z0-9_]*) continue ;;
        esac

        # Export values
        export "${key}=${value}"
    done < "${config_file}"
}
