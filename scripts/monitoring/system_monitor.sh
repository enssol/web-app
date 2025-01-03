#!/bin/sh

# Ensure log directory exists
[ -d "/var/log/webapp" ] || mkdir -p "/var/log/webapp"

# Monitor system resources and log activity
monitor_system() {
    log_file="/var/log/webapp/monitor.log"
    alerts_file="/var/log/webapp/alerts.log"

    # Create log files if they don't exist
    touch "${log_file}" "${alerts_file}"

    # Set secure permissions
    chmod 640 "${log_file}" "${alerts_file}"

    # Log system stats with error handling
    {
        printf "=== System Status Report ===\n"
        printf "Date: %s\n" "$(date)"
        printf "\nMemory Usage:\n"
        free 2>/dev/null || printf "Error: Could not get memory usage\n"
        printf "\nDisk Usage:\n"
        df -h 2>/dev/null || printf "Error: Could not get disk usage\n"
        printf "\nProcess Count: "
        ps aux 2>/dev/null | wc -l || printf "Error: Could not get process count\n"
        printf "\n========================\n\n"
    } >> "${log_file}"

    # Check system thresholds
    memory_used=$(free 2>/dev/null | awk '/Mem:/ {print $3/$2 * 100.0}')
    disk_used=$(df -h / 2>/dev/null | awk 'NR==2 {print $5}' | tr -d '%')

    # Alert on high memory usage
    if [ "${memory_used%.*}" -gt 90 ]; then
        printf "[%s] WARNING: High memory usage: %s%%\n" "$(date)" "${memory_used}" >> "${alerts_file}"
    fi

    # Alert on high disk usage
    if [ "${disk_used}" -gt 90 ]; then
        printf "[%s] WARNING: High disk usage: %s%%\n" "$(date)" "${disk_used}" >> "${alerts_file}"
    fi
}
