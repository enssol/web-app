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

CONFIG_FILE="/home/appuser/web-app/scripts/validate/css-rules.conf"
ENCODING="ISO-8859-1"
TEMP_DIR="/tmp/css-validate"

# Create temporary directory
mkdir -p "${TEMP_DIR}"

# Function to check file encoding
check_encoding() {
    file -i "$1" | grep -q "charset=${ENCODING}"
}

# Function to extract and validate CSS
validate_css() {
    local file="$1"
    local temp_css="${TEMP_DIR}/temp.css"

    # Check encoding
    if ! check_encoding "$file"; then
        echo "ERROR: Invalid encoding in $file - must be ${ENCODING}"
        return 1
    }

    # Extract CSS content between style tags
    sed -n '/<style/,/<\/style>/p' "$file" > "$temp_css"

    # Check style tag has type attribute
    if ! grep -q '<style.*type="text/css"' "$temp_css"; then
        echo "ERROR: Missing or invalid style type in $file"
        return 1
    }

    # Check for disallowed features
    while IFS= read -r pattern; do
        if grep -q "$pattern" "$temp_css"; then
            echo "ERROR: Disallowed CSS feature in $file: $pattern"
            return 1
        fi
    done < "$CONFIG_FILE"

    # Validate selectors and properties
    awk '/^[^\/]/ && /{/{
        # Check for CSS3 selectors
        if ($0 ~ /::(before|after|placeholder|selection)/) {
            print "ERROR: CSS3 pseudo-element found in " FILENAME
            exit 1
        }
        # Check for vendor prefixes
        if ($0 ~ /^[-]+(webkit|moz|ms|o)-/) {
            print "ERROR: Vendor prefix found in " FILENAME
            exit 1
        }
    }' "$temp_css"

    echo "OK: $file passes CSS2.1 validation"
    return 0
}

# Main validation loop
for dir in ${HTML_DIRS}; do
    if [ -d "$dir" ]; then
        for file in "$dir"/*.html; do
            [ -f "$file" ] || continue
            validate_css "$file"
        done
    fi
done

# Cleanup
rm -rf "${TEMP_DIR}"
