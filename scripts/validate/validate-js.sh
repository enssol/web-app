#!/bin/sh

# Set strict error handling
set -e
set -u

# Configuration
HTML_DIRS="/home/appuser/web-app/public/views/
/home/appuser/web-app/public/views/includes/
/home/appuser/web-app/public/views/shared/
/home/appuser/web-app/public/views/pages/
/home/appuser/web-app/public/views/layout/
/home/appuser/web-app/public/views/auth/
/home/appuser/web-app/public/views/errors/"

CONFIG_FILE="/home/appuser/web-app/scripts/validate/js-rules.conf"
ENCODING="ISO-8859-1"
TEMP_DIR="/tmp/js-validate"

# Create temporary directory
mkdir -p "${TEMP_DIR}"

# Function to check file encoding
check_encoding() {
    file -i "$1" | grep -q "charset=${ENCODING}"
}

# Function to extract and validate JavaScript
validate_js() {
    local file="$1"
    local temp_js="${TEMP_DIR}/temp.js"

    # Check encoding
    if ! check_encoding "$file"; then
        echo "ERROR: Invalid encoding in $file - must be ${ENCODING}"
        return 1
    }

    # Extract JavaScript content between script tags
    sed -n '/<script/,/<\/script>/p' "$file" > "$temp_js"

    # Check script tag has type attribute
    if ! grep -q '<script.*type="text/javascript"' "$temp_js"; then
        echo "ERROR: Missing or invalid script type in $file"
        return 1
    }

    # Check for external scripts
    if grep -q 'src="' "$temp_js"; then
        echo "ERROR: External scripts not allowed in $file"
        return 1
    }

    # Check for "use strict" directive
    if ! grep -q '"use strict";' "$temp_js"; then
        echo "ERROR: Missing 'use strict' directive in $file"
        return 1
    }

    # Check for disallowed features
    while IFS= read -r pattern; do
        # Skip comments
        case "$pattern" in
            \#*) continue ;;
        esac
        if grep -q "$pattern" "$temp_js"; then
            echo "ERROR: Disallowed JavaScript feature in $file: $pattern"
            return 1
        fi
    done < "$CONFIG_FILE"

    # Additional syntax checks
    awk '/^[^\/]/ {
        # Check for undeclared variables
        if ($0 ~ /[^.]forEach\(/ || $0 ~ /[^.]map\(/) {
            print "ERROR: Modern array methods found in " FILENAME
            exit 1
        }
        # Check for modern object methods
        if ($0 ~ /Object\.(keys|values|entries)/) {
            print "ERROR: Modern Object methods found in " FILENAME
            exit 1
        }
    }' "$temp_js"

    echo "OK: $file passes ECMAScript 3 validation"
    return 0
}

# Main validation loop
for dir in ${HTML_DIRS}; do
    if [ -d "$dir" ]; then
        for file in "$dir"/*.html; do
            [ -f "$file" ] || continue
            validate_js "$file"
        done
    fi
done

# Cleanup
rm -rf "${TEMP_DIR}"
