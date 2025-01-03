#!/bin/sh

# Directory for storing .roff source files
ROFF_DIR="./man/roff"

# Function to validate .roff files
validate_roff() {
    for file in "$ROFF_DIR"/*.roff; do
        [ -e "$file" ] || continue
        if ! groff -man "$file" > /dev/null; then
            echo "Validation failed for $file"
            return 1
        fi
    done
    echo "All .roff files are valid."
}

# Main script logic
validate_roff
