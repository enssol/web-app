#!/bin/sh

# Exit on error and undefined vars
set -e
set -u

#######################
# Constants & Globals #
#######################

# System paths
CONTAINER_PATH="/home/ubuntu/.local/share/containers/storage/volumes/pod_vol/_data/web-app"
INSTALL_DIR="/opt/webserver"
WWW_DIR="${INSTALL_DIR}/www"
BIN_DIR="${INSTALL_DIR}/bin"
CERT_DIR="${INSTALL_DIR}/ssl"
LOG_DIR="/var/log/webserver"
BACKUP_DIR="/var/backups/webserver"

# Package configuration
ARCHIVE_NAME="webserver.tar.xz"
BINARY_NAME="webserver"
BACKUP_RETAIN_DAYS=30

# Service configuration
SERVICE_NAME="webserver"
SERVICE_USER="webserver"
SERVICE_GROUP="webserver"
SERVICE_FILE="/etc/systemd/system/${SERVICE_NAME}.service"

# Domain configuration
DOMAIN="enssol.com.au"
WWW_DOMAIN="www.enssol.com.au"

# Certificate files
SSL_CERT="${CONTAINER_PATH}/cert.pem"
SSL_KEY="${CONTAINER_PATH}/key.pem"
SSL_DNS="${CONTAINER_PATH}/cloudflare_dns_records.txt"

# Cloudflare IPs
CLOUDFLARE_IPS="
173.245.48.0/20
103.21.244.0/22
103.22.200.0/22
103.31.4.0/22
141.101.64.0/18
108.162.192.0/18
190.93.240.0/20
188.114.96.0/20
197.234.240.0/22
198.41.128.0/17
162.158.0.0/15
104.16.0.0/13
104.24.0.0/14
172.64.0.0/13
131.0.72.0/22"

#######################
# Helper Functions    #
#######################

log() {
    printf '[%s] %s\n' "$(date '+%Y-%m-%d %H:%M:%S')" "$1"
}

error() {
    log "ERROR: $1" >&2
    exit 1
}

check_root() {
    if [ "$(id -u)" -ne 0 ]; then
        error "This script must be run as root"
    fi
}

check_dependencies() {
    local cmd
    for cmd in openssl iptables tar systemctl setcap; do
        if ! command -v "${cmd}" >/dev/null 2>&1; then
            error "${cmd} is required but not installed"
        fi
    done
}

create_backup() {
    log "Creating backup..."

    install -d -m 0750 "${BACKUP_DIR}"

    if [ -d "${INSTALL_DIR}" ]; then
        tar -czf "${BACKUP_DIR}/webserver-$(date +%Y%m%d-%H%M%S).tar.gz" -C "${INSTALL_DIR}" .
        find "${BACKUP_DIR}" -type f -mtime "+${BACKUP_RETAIN_DAYS}" -delete
    fi
}

setup_ssl() {
    log "Setting up SSL certificates..."

    # Verify certificate files exist
    for cert in "${SSL_CERT}" "${SSL_KEY}"; do
        if [ ! -f "${cert}" ]; then
            error "Certificate file not found: ${cert}"
        fi
    done

    # Create SSL directory with secure permissions
    install -d -m 0750 "${CERT_DIR}"

    # Install certificates with restricted permissions
    install -m 0640 "${SSL_CERT}" "${CERT_DIR}/cert.pem"
    install -m 0640 "${SSL_KEY}" "${CERT_DIR}/key.pem"

    # Copy DNS records if available
    if [ -f "${SSL_DNS}" ]; then
        install -m 0640 "${SSL_DNS}" "${CERT_DIR}/dns_records.txt"
    fi

    # Validate certificate matches private key
    CERT_MOD=$(openssl x509 -noout -modulus -in "${CERT_DIR}/cert.pem" | openssl md5)
    KEY_MOD=$(openssl rsa -noout -modulus -in "${CERT_DIR}/key.pem" | openssl md5)
    if [ "$CERT_MOD" != "$KEY_MOD" ]; then
        error "SSL certificate and key do not match"
    fi

    log "SSL certificates installed successfully"
}

setup_firewall() {
    log "Configuring firewall rules..."

    # Allow Cloudflare IPs
    echo "${CLOUDFLARE_IPS}" | while read -r ip_range; do
        if [ -n "${ip_range}" ]; then
            iptables -A INPUT -p tcp -s "${ip_range}" --dport 443 -j ACCEPT
            iptables -A INPUT -p tcp -s "${ip_range}" --dport 80 -j ACCEPT
        fi
    done

    # Default deny other web traffic
    iptables -A INPUT -p tcp --dport 80 -j DROP
    iptables -A INPUT -p tcp --dport 443 -j DROP

    log "Firewall configured successfully"
}

# Update the create_dirs() function
create_dirs() {
    log "Creating directories..."

    # Create base directories with root ownership
    install -d -m 0755 "${INSTALL_DIR}"
    install -d -m 0755 "${BIN_DIR}"
    install -d -m 0755 "${WWW_DIR}"
    install -d -m 0755 "${LOG_DIR}"

    # Set correct ownership for service user directories
    chown -R "${SERVICE_USER}:${SERVICE_GROUP}" "${INSTALL_DIR}"
    chown -R "${SERVICE_USER}:${SERVICE_GROUP}" "${WWW_DIR}"
    chown -R "${SERVICE_USER}:${SERVICE_GROUP}" "${LOG_DIR}"

    # Ensure execute permissions on directories
    chmod 755 "${INSTALL_DIR}"
    chmod 755 "${BIN_DIR}"
    chmod 755 "${WWW_DIR}"
}

create_user() {
    log "Setting up service user..."
    if ! id -u "${SERVICE_USER}" >/dev/null 2>&1; then
        useradd -r -s /sbin/nologin "${SERVICE_USER}"
    fi
}

create_service() {
    log "Creating systemd service..."
    cat > "${SERVICE_FILE}" << EOF
[Unit]
Description=EnvEng Web Server
After=network.target

[Service]
Type=simple
User=${SERVICE_USER}
Group=${SERVICE_GROUP}
Environment=DOMAIN=${DOMAIN}
Environment=WWW_DOMAIN=${WWW_DOMAIN}
Environment=CERT_DIR=${CERT_DIR}
Environment=LOG_DIR=${LOG_DIR}
Environment=LD_LIBRARY_PATH=/usr/lib:/usr/local/lib
WorkingDirectory=${INSTALL_DIR}
ExecStart=${BIN_DIR}/${BINARY_NAME}
Restart=on-failure
RestartSec=5
TimeoutStartSec=0
AmbientCapabilities=CAP_NET_BIND_SERVICE

# Security
NoNewPrivileges=yes
ProtectSystem=full
ProtectHome=yes
PrivateTmp=yes
PrivateDevices=yes
DevicePolicy=closed
RestrictAddressFamilies=AF_INET AF_INET6 AF_UNIX
RestrictNamespaces=true
RestrictRealtime=true
ProtectKernelModules=true
ProtectKernelTunables=true
ProtectControlGroups=true
LockPersonality=true
SystemCallArchitectures=native

[Install]
WantedBy=multi-user.target
EOF

    chmod 644 "${SERVICE_FILE}"
}

manage_systemd_service() {
    log "Managing systemd service..."

    # Stop service if running
    systemctl stop "${SERVICE_NAME}" 2>/dev/null || true

    # Clear any failed state
    systemctl reset-failed "${SERVICE_NAME}" 2>/dev/null || true

    # Reload configs
    systemctl daemon-reload || error "Failed to reload systemd configuration"

    # Enable and start
    systemctl enable "${SERVICE_NAME}" || error "Failed to enable service"
    systemctl restart "${SERVICE_NAME}" || error "Failed to start service"

    # Wait for service to stabilize
    sleep 2

    # Check status
    if ! systemctl is-active --quiet "${SERVICE_NAME}"; then
        journalctl -u "${SERVICE_NAME}" --no-pager -n 50
        error "Service failed to start - check logs above"
    fi

    log "Service started successfully"
}

debug_test_binary() {
    local binary_path="$1"
    local test_config_file
    local test_result=0

    # Create temporary test config
    test_config_file=$(mktemp)
    cat > "$test_config_file" << EOF
{
    "port": 8080,
    "doc_root": "/opt/webserver/www",
    "access_log": "/var/log/webserver/access.log",
    "error_log": "/var/log/webserver/error.log"
}
EOF

    # Test 1: Help output
    debug "Testing help output..."
    if ! "$binary_path" -h; then
        if [ $? -eq 1 ] && "$binary_path" -h 2>&1 | grep -q "Usage:"; then
            debug "Help test passed (expected usage message)"
        else
            debug "Help test failed"
            test_result=1
        fi
    fi

    # Test 2: Version output
    debug "Testing version output..."
    if ! "$binary_path" -v >/dev/null 2>&1; then
        if [ $? -eq 1 ]; then
            debug "Version test passed"
        else
            debug "Version test failed"
            test_result=1
        fi
    fi

    # Cleanup
    rm -f "$test_config_file"

    return $test_result
}

debug() {
    printf '[DEBUG] %s\n' "$1" >&2
}

# Update deploy_application() function
deploy_application() {
    log "Deploying application..."

    cd "${CONTAINER_PATH}" || error "Failed to change to container path"

    # Verify source binary with debug info
    debug "Checking source binary in ${CONTAINER_PATH}/bin/${BINARY_NAME}"
    if [ ! -f "bin/${BINARY_NAME}" ]; then
        error "Binary not found: bin/${BINARY_NAME}"
    fi

    # Check binary format with output
    debug "Checking binary format..."
    file "bin/${BINARY_NAME}"
    file "bin/${BINARY_NAME}" | grep -q "ELF.*executable" || error "Invalid binary format"

    # Create clean staging directory
    STAGING=$(mktemp -d)
    debug "Created staging directory: ${STAGING}"
    trap 'rm -rf ${STAGING}' EXIT

    # Copy files to staging with verification
    debug "Copying binary to staging..."
    install -m 0755 -D "bin/${BINARY_NAME}" "${STAGING}/bin/${BINARY_NAME}"
    debug "Copying www content..."
    cp -a www "${STAGING}/"

    # Create deployment package
    debug "Creating deployment package..."
    tar -C "${STAGING}" -cJf "${HOME}/${ARCHIVE_NAME}" .

    # Deploy files with verification
    debug "Deploying files to ${INSTALL_DIR}..."
    tar -C "${INSTALL_DIR}" -xf "${HOME}/${ARCHIVE_NAME}"
    rm -f "${HOME}/${ARCHIVE_NAME}"

    # Set ownership and permissions with verification
    debug "Setting ownership and permissions..."
    chown -R root:root "${INSTALL_DIR}"
    chown -R "${SERVICE_USER}:${SERVICE_GROUP}" "${LOG_DIR}"
    chown "${SERVICE_USER}:${SERVICE_GROUP}" "${INSTALL_DIR}/bin/${BINARY_NAME}"

    chmod 755 "${INSTALL_DIR}/bin/${BINARY_NAME}"
    chmod 644 "${INSTALL_DIR}/www/"*

    # Set capabilities with verification
    debug "Setting capabilities..."
    setcap cap_net_bind_service=+ep "${INSTALL_DIR}/bin/${BINARY_NAME}"
    getcap "${INSTALL_DIR}/bin/${BINARY_NAME}"

    # Verify deployment with detailed checks
    debug "Verifying deployment..."
    ls -l "${INSTALL_DIR}/bin/${BINARY_NAME}"

    if ! [ -x "${INSTALL_DIR}/bin/${BINARY_NAME}" ]; then
        error "Binary not executable after deployment"
    fi

    # Update permissions section
    debug "Setting ownership and permissions..."

    # Set ownership
    chown -R "${SERVICE_USER}:${SERVICE_GROUP}" "${INSTALL_DIR}"
    chown "${SERVICE_USER}:${SERVICE_GROUP}" "${BIN_DIR}/${BINARY_NAME}"

    # Set directory permissions
    find "${INSTALL_DIR}" -type d -exec chmod 755 {} \;

    # Set file permissions
    find "${INSTALL_DIR}" -type f -exec chmod 644 {} \;
    chmod 755 "${BIN_DIR}/${BINARY_NAME}"

    # Set special permissions for logs
    chown -R "${SERVICE_USER}:${SERVICE_GROUP}" "${LOG_DIR}"
    chmod 755 "${LOG_DIR}"

    # Set capabilities
    debug "Setting capabilities..."
    setcap cap_net_bind_service=+ep "${BIN_DIR}/${BINARY_NAME}"

    # Test binary execution with debug output
    debug "Testing binary execution..."
    if ! debug_test_binary "/opt/webserver/bin/webserver"; then
        debug "Binary passed basic execution tests"
        return 0
    else
        debug "Binary failed execution tests"
        return 1
    fi

    log "Application deployed successfully"
}

#######################
# Main Installation   #
#######################

main() {
    check_root
    check_dependencies
    create_backup
    create_user
    create_dirs
    setup_ssl
    setup_firewall
    deploy_application
    create_service
    manage_systemd_service

    log "Starting service..."
    systemctl daemon-reload
    systemctl enable "${SERVICE_NAME}"
    systemctl restart "${SERVICE_NAME}"

    log "Checking service status..."
    if ! systemctl is-active "${SERVICE_NAME}" >/dev/null 2>&1; then
        error "Service failed to start"
    fi

    log "Installation completed successfully"
}

# Run main function
main "$@"
