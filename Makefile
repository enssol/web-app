# Compiler and flags
CC = musl-gcc
CFLAGS = -std=c90 -D_POSIX_C_SOURCE=1 -D_XOPEN_SOURCE=500 \
         -Wall -Wextra -pedantic -static -O2
LDFLAGS = -static -pthread -lssl -lcrypto

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
DOC_DIR = docs
ETC_DIR = etc

# Files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/app
CONFIG = $(ETC_DIR)/env/.env

# SSL files
SSL_CERT = $(ETC_DIR)/ssl/cert.pem
SSL_KEY = $(ETC_DIR)/ssl/privkey.pem

# Database files
DB_FILE = $(ETC_DIR)/db/records.rec
DB_SCHEMA = $(ETC_DIR)/db/database.desc

# Authentication
AUTH_FILE = $(ETC_DIR)/auth/passwd

.PHONY: all clean install docs package

all: dirs $(TARGET)

# Create necessary directories
dirs:
    @mkdir -p $(OBJ_DIR) $(BIN_DIR) $(ETC_DIR)/log $(ETC_DIR)/db \
        $(ETC_DIR)/auth $(ETC_DIR)/ssl $(ETC_DIR)/env

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
    $(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Link object files
$(TARGET): $(OBJS)
    $(CC) $(OBJS) $(LDFLAGS) -o $@
    @chmod 755 $@

# Install the application
install: all
    @mkdir -p /usr/local/bin
    @cp $(TARGET) /usr/local/bin/
    @mkdir -p /etc/app
    @cp -r $(ETC_DIR)/* /etc/app/
    @chmod 600 /etc/app/ssl/privkey.pem
    @chmod 644 /etc/app/ssl/cert.pem
    @chmod 600 /etc/app/auth/passwd
    @chmod 600 /etc/app/env/.env

# Generate documentation
docs:
    @mkdir -p $(DOC_DIR)
    @echo "Generating documentation..."
    # Add documentation generation commands here

# Create distribution package
package: all
    @echo "Creating package..."
    tar -cJf $(BIN_DIR)/app.tar.xz \
        -C $(BIN_DIR) app \
        -C ../$(ETC_DIR) .

# Clean build artifacts
clean:
    rm -rf $(OBJ_DIR) $(BIN_DIR)

# Check SSL certificate
check-ssl:
    @test -f $(SSL_CERT) || { echo "Missing $(SSL_CERT)"; exit 1; }
    @test -f $(SSL_KEY) || { echo "Missing $(SSL_KEY)"; exit 1; }
    @openssl verify $(SSL_CERT)

# Security checks
security-check:
    @echo "Checking file permissions..."
    @test $$(stat -c %a $(AUTH_FILE)) -eq 600 || \
        { echo "$(AUTH_FILE) permissions should be 600"; exit 1; }
