#!/bin/sh
set -e

# Configuration
CONTAINER_PATH="/home/ubuntu/.local/share/containers/storage/volumes/pod_vol/_data/web-app"
INSTALL_DIR="/opt/webserver"
SSL_DIR="${INSTALL_DIR}/ssl"
WWW_DIR="${INSTALL_DIR}/www"
BIN_DIR="${INSTALL_DIR}/bin"
SERVICE_NAME="webserver"
SERVICE_USER="webserver"
SERVICE_FILE="/etc/systemd/system/${SERVICE_NAME}.service"
LOG_DIR="/var/log"

# Function to check required files
check_required_files() {
    local required_files=(
        "${CONTAINER_PATH}/cert.pem"
        "${CONTAINER_PATH}/key.pem"
        "${CONTAINER_PATH}/origin_ca_rsa_root.pem"
        "${CONTAINER_PATH}/webserver-1.0.0.tar.gz"
    )

    for file in "${required_files[@]}"; do
        if [ ! -f "$file" ]; then
            echo "Error: Required file not found: $file"
            exit 1
        fi
    done
}

# Create service user if not exists
create_user() {
    echo "Creating service user..."
    id -u "${SERVICE_USER}" >/dev/null 2>&1 || useradd -r -s /sbin/nologin "${SERVICE_USER}"
}

# Prepare directories
prepare_directories() {
    echo "Preparing directories..."
    rm -rf "${INSTALL_DIR:?}/"*
    mkdir -p "${BIN_DIR}" "${WWW_DIR}" "${SSL_DIR}"
    chmod 755 "${INSTALL_DIR}"
    chmod 755 "${BIN_DIR}"
    chmod 755 "${WWW_DIR}"
    chmod 700 "${SSL_DIR}"
}

# Deploy SSL files
deploy_ssl() {
    echo "Deploying SSL files..."
    cp "${CONTAINER_PATH}/cert.pem" "${SSL_DIR}/cert.pem"
    cp "${CONTAINER_PATH}/key.pem" "${SSL_DIR}/key.pem"
    cp "${CONTAINER_PATH}/origin_ca_rsa_root.pem" "${SSL_DIR}/origin_ca_rsa_root.pem"

    chmod 600 "${SSL_DIR}/cert.pem"
    chmod 600 "${SSL_DIR}/key.pem"
    chmod 644 "${SSL_DIR}/origin_ca_rsa_root.pem"

    chown -R "${SERVICE_USER}:${SERVICE_USER}" "${SSL_DIR}"
}

# Deploy application files
deploy_files() {
    echo "Deploying files..."
    cd "${CONTAINER_PATH}"
    if ! tar -xzf webserver-1.0.0.tar.gz; then
        echo "Error: Failed to extract webserver archive"
        exit 1
    fi

    mv pkg/webserver "${BIN_DIR}/" || exit 1
    mv pkg/www/* "${WWW_DIR}/" || exit 1
}

# Configure permissions
configure_permissions() {
    echo "Setting permissions..."
    chown -R "${SERVICE_USER}:${SERVICE_USER}" "${INSTALL_DIR}"
    chmod 755 "${BIN_DIR}/webserver"
    setcap cap_net_bind_service=+ep "${BIN_DIR}/webserver"
}

# Create systemd service
create_service() {
    echo "Creating service file..."
    [ -f "${SERVICE_FILE}" ] && mv "${SERVICE_FILE}" "${SERVICE_FILE}.bak"

    cat > "${SERVICE_FILE}" << EOF
[Unit]
Description=EnvEng Web Server
After=network.target

[Service]
Type=simple
User=${SERVICE_USER}
Group=${SERVICE_USER}
WorkingDirectory=${WWW_DIR}
Environment=PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
ExecStart=${BIN_DIR}/webserver
Restart=on-failure
RestartSec=5
StandardOutput=append:${LOG_DIR}/webserver.log
StandardError=append:${LOG_DIR}/webserver.error.log
AmbientCapabilities=CAP_NET_BIND_SERVICE
LimitNOFILE=65535
Environment=SERVER_HOST=0.0.0.0
Environment=SERVER_PORT=443

[Install]
WantedBy=multi-user.target
EOF
}

# Verify installation
verify_installation() {
    echo "Verifying installation..."

    # Check directories
    [ ! -d "${WWW_DIR}" ] && { echo "Error: WWW directory not found"; exit 1; }
    [ ! -d "${SSL_DIR}" ] && { echo "Error: SSL directory not found"; exit 1; }

    # Check files
    [ ! -f "${WWW_DIR}/index.html" ] && { echo "Error: index.html not found"; exit 1; }
    [ ! -f "${SSL_DIR}/cert.pem" ] && { echo "Error: cert.pem not found"; exit 1; }
    [ ! -f "${SSL_DIR}/key.pem" ] && { echo "Error: key.pem not found"; exit 1; }
    [ ! -f "${SSL_DIR}/origin_ca_rsa_root.pem" ] && { echo "Error: origin_ca_rsa_root.pem not found"; exit 1; }
    [ ! -f "${BIN_DIR}/webserver" ] && { echo "Error: webserver binary not found"; exit 1; }
}

# Start service
start_service() {
    echo "Starting service..."
    systemctl daemon-reload
    systemctl enable "${SERVICE_NAME}"
    systemctl restart "${SERVICE_NAME}"
    systemctl status "${SERVICE_NAME}"
}

# Cleanup
cleanup() {
    echo "Cleaning up..."
    rm -rf pkg
}

# Main execution
main() {
    check_required_files
    create_user
    systemctl stop "${SERVICE_NAME}" || true
    prepare_directories
    deploy_ssl
    deploy_files
    configure_permissions
    create_service
    verify_installation
    start_service
    cleanup
    echo "Deployment completed successfully"
}

# Execute main function
main
