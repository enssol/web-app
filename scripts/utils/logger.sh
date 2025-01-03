#!/bin/sh

# Log access requests to track HTTP traffic
log_access() {
    ip_addr="${1}"
    method="${2}"
    path="${3}"
    timestamp=$(date '+%Y-%m-%d %H:%M:%S')
    
    printf '[%s] %s %s %s\n' \
        "${timestamp}" "${ip_addr}" "${method}" "${path}" \
        >> "/var/log/webapp/access.log"
}

# Log system events for monitoring
log_event() {
    event_type="${1}"
    event_msg="${2}"
    timestamp=$(date '+%Y-%m-%d %H:%M:%S')
    
    printf '[%s] %s: %s\n' \
        "${timestamp}" "${event_type}" "${event_msg}" \
        >> "/var/log/webapp/events.log"
}

# Log errors for debugging
log_error() {
    error_type="${1}"
    error_msg="${2}"
    timestamp=$(date '+%Y-%m-%d %H:%M:%S')
    
    printf '[%s] %s: %s\n' \
        "${timestamp}" "${error_type}" "${error_msg}" \
        >> "/var/log/webapp/error.log"
}

# Log audit events for security
log_audit() {
    user="${1}"
    action="${2}"
    resource="${3}"
    timestamp=$(date '+%Y-%m-%d %H:%M:%S')
    
    printf '[%s] User %s performed %s on %s\n' \
        "${timestamp}" "${user}" "${action}" "${resource}" \
        >> "/var/log/webapp/audit.log"
}
