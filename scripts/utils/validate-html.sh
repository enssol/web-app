#!/bin/sh

# HTML structure validator script
# Validates HTML files against HTML 4.01 standard structure requirements

check_html_structure() {
    file="$1"
    errors=0

    # Check DOCTYPE
    if ! grep -q '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">' "$file"; then
        echo "ERROR: $file - Missing or incorrect DOCTYPE declaration"
        errors=$((errors + 1))
    fi

    # Check html tag
    if ! grep -q '<html>' "$file"; then
        echo "ERROR: $file - Missing <html> tag"
        errors=$((errors + 1))
    fi

    # Check head tag
    if ! grep -q '<head>' "$file"; then
        echo "ERROR: $file - Missing <head> tag"
        errors=$((errors + 1))
    fi

    # Check body tag
    if ! grep -q '<body>' "$file"; then
        echo "ERROR: $file - Missing <body> tag"
        errors=$((errors + 1))
    fi

    # Check meta charset
    if ! grep -q '<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">' "$file"; then
        echo "ERROR: $file - Missing ISO-8859-1 charset declaration"
        errors=$((errors + 1))
    fi

    return $errors
}

# Process all HTML files
find /home/appuser/web-app/public/views -type f -name "*.html" | while read -r file; do
    check_html_structure "$file"
done
