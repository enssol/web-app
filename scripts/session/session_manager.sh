#!/bin/sh

# Ensure session directory exists
[ -d "/var/lib/webapp/sessions" ] || mkdir -p "/var/lib/webapp/sessions"

# Handle user sessions
manage_session() {
    session_id="${1}"
    action="${2}"

    # Validate session ID format
    case "${session_id}" in
        *[!a-zA-Z0-9-]*)
            printf "Error: Invalid session ID format\n" >&2
            return 1
            ;;
    esac

    session_file="/var/lib/webapp/sessions/${session_id}"

    case "${action}" in
        "create")
            # Create new session with proper permissions
            umask 077
            date +%s > "${session_file}" || {
                printf "Error: Could not create session file\n" >&2
                return 1
            }
            ;;
        "validate")
            # Check session validity
            [ -f "${session_file}" ] || {
                printf "Error: Session file not found\n" >&2
                return 1
            }
            # Check session age (1 hour timeout)
            session_age=$(($(date +%s) - $(cat "${session_file}")))
            [ "${session_age}" -lt 3600 ] || {
                rm -f "${session_file}"
                printf "Error: Session expired\n" >&2
                return 1
            }
            ;;
        "destroy")
            # Remove session securely
            rm -f "${session_file}"
            ;;
        *)
            printf "Error: Invalid action specified\n" >&2
            return 1
            ;;
    esac
}
