#!/bin/sh

# Handle errors and logging
log_error() {
    error_msg="${1}"
    error_code="${2:-500}"
    timestamp=$(date '+%Y-%m-%d %H:%M:%S')

    # Log error
    printf '[%s] Error %s: %s\n' "${timestamp}" "${error_code}" "${error_msg}" >> "/var/log/webapp/error.log"
}

# Generate error response
generate_error_response() {
    error_code="${1}"
    error_msg="${2}"

    # Create error page
    cat << EOF
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head><title>Error ${error_code}</title></head>
<body><h1>Error ${error_code}</h1><p>${error_msg}</p></body>
</html>
EOF
}
