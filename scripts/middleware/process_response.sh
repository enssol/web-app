#!/bin/sh

# Process outgoing responses
process_response() {
    status_code="${1}"
    content_type="${2:-text/html; charset=ISO-8859-1}"

    # Generate headers
    printf "Status: %s\n" "${status_code}"
    printf "Content-Type: %s\n\n" "${content_type}"
}

# Handle static file response
handle_static_response() {
    file_path="${1}"

    if [ -f "${file_path}" ]; then
        process_response "200"
        cat "${file_path}"
    else
        . ./scripts/middleware/handle_error.sh
        log_error "File not found: ${file_path}" "404"
        generate_error_response "404" "File not found"
    fi
}
