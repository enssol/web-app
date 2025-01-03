#!/bin/sh

# Maximum log size in bytes (5MB)
MAX_LOG_SIZE=5242880

# Rotate a log file if it exceeds the size limit
rotate_log() {
    log_file="${1}"

    if [ -f "${log_file}" ]; then
        size=$(stat -f%z "${log_file}" 2>/dev/null || stat -c%s "${log_file}")

        if [ "${size}" -gt "${MAX_LOG_SIZE}" ]; then
            timestamp=$(date '+%Y%m%d-%H%M%S')
            mv "${log_file}" "${log_file}.${timestamp}"
            touch "${log_file}"
            chmod 640 "${log_file}"
        fi
    fi
}

# Rotate all logs
rotate_all_logs() {
    rotate_log "/var
