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

RULES_FILE="/home/appuser/web-app/scripts/validate/html-rules.conf"

# Configuration constants
ENCODING="ISO-8859-1"
TEMP_DIR="/tmp/html-validate"

# Create temporary directory
mkdir -p "${TEMP_DIR}"

# Function to check file encoding
check_encoding() {
    local file="$1"
    file -i "$file" | grep -q "charset=${ENCODING}"
}

# Function to validate an HTML file
validate_html() {
    local file="$1"
    local temp_file="${TEMP_DIR}/temp.html"
    local errors=0

    # Check encoding
    if ! check_encoding "$file"; then
        echo "ERROR: Invalid encoding in $file - must be ${ENCODING}"
        return 1
    }

    # Copy file to temp for processing
    cp "$file" "$temp_file"

    # Check for DOCTYPE declaration
    if ! grep -q "^<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\"" "$temp_file"; then
        echo "ERROR: Missing or invalid DOCTYPE in $file"
        errors=$((errors + 1))
    fi

    # Check for required meta tags
    if ! grep -q "<meta[^>]*http-equiv=\"Content-Type\"[^>]*content=\"text/html; charset=ISO-8859-1\"" "$temp_file"; then
        echo "ERROR: Missing or invalid Content-Type meta tag in $file"
        errors=$((errors + 1))
    fi

    # Check for banned elements
    while IFS= read -r pattern; do
        # Skip comments and empty lines
        case "$pattern" in
            \#*|"") continue ;;
        esac
        if grep -qi "$pattern" "$temp_file"; then
            echo "ERROR: Banned element found in $file: $pattern"
            errors=$((errors + 1))
        fi
    done < "$RULES_FILE"

    # Check element nesting and closing
    awk '
    BEGIN { stack_size = 0 }
    /<[^\/][^>]*[^\/]>/ {
        # Extract tag name
        match($0, /<([^ >]+)/, tag)
        if (tag[1] !~ /^!/) {
            stack[stack_size++] = tag[1]
        }
    }
    /<\/[^>]+>/ {
        match($0, /<\/([^>]+)/, tag)
        if (stack_size == 0 || stack[stack_size-1] != tag[1]) {
            print "ERROR: Mismatched or improperly nested tags near: " $0
            exit 1
        }
        stack_size--
    }
    END {
        if (stack_size > 0) {
            print "ERROR: Unclosed tags remain"
            exit 1
        }
    }' "$temp_file" || errors=$((errors + 1))

    # Check for required attributes
    if grep -q "<img[^>]*>" "$temp_file"; then
        if ! grep -q "<img[^>]*alt=\"[^\"]*\"[^>]*>" "$temp_file"; then
            echo "ERROR: Image missing alt attribute in $file"
            errors=$((errors + 1))
        fi
    fi

    if grep -q "<script[^>]*>" "$temp_file"; then
        if ! grep -q "<script[^>]*type=\"text/javascript\"[^>]*>" "$temp_file"; then
            echo "ERROR: Script missing type attribute in $file"
            errors=$((errors + 1))
        fi
    fi

    if grep -q "<style[^>]*>" "$temp_file"; then
        if ! grep -q "<style[^>]*type=\"text/css\"[^>]*>" "$temp_file"; then
            echo "ERROR: Style missing type attribute in $file"
            errors=$((errors + 1))
        fi
    fi

    # Check form elements
    if grep -q "<form[^>]*>" "$temp_file"; then
        if ! grep -q "<form[^>]*action=\"[^\"]*\"[^>]*>" "$temp_file"; then
            echo "ERROR: Form missing action attribute in $file"
            errors=$((errors + 1))
        fi
    fi

    # Check for unquoted attributes
    if grep -q "[a-zA-Z]=[^\"'][^ >]*[ >]" "$temp_file"; then
        echo "ERROR: Unquoted attributes found in $file"
        errors=$((errors + 1))
    fi

    # Check for lowercase element and attribute names
    if grep -q "<[A-Z]" "$temp_file" || grep -q " [A-Z][a-zA-Z]*=" "$temp_file"; then
        echo "ERROR: Uppercase element or attribute names found in $file"
        errors=$((errors + 1))
    fi

    # Check table structure
    if grep -q "<table[^>]*>" "$temp_file"; then
        if ! grep -q "<tbody[^>]*>" "$temp_file"; then
            echo "ERROR: Table missing tbody element in $file"
            errors=$((errors + 1))
        fi
    fi

    if [ "$errors" -eq 0 ]; then
        echo "OK: $file passes HTML 4.01 Strict validation"
        return 0
    else
        echo "FAIL: $file has $errors validation errors"
        return 1
    fi
}

# Main validation loop
validation_failed=0

for dir in ${HTML_DIRS}; do
    if [ -d "$dir" ]; then
        for file in "$dir"/*.html; do
            [ -f "$file" ] || continue
            if ! validate_html "$file"; then
                validation_failed=1
            fi
        done
    fi
done

# Cleanup
rm -rf "${TEMP_DIR}"

# Exit with status
exit $validation_failed
