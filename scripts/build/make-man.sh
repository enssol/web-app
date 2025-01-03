#!/bin/sh

# Directory for storing .roff source files
ROFF_DIR="./man/roff"
# Directory for storing generated manual pages
MAN_DIR="./man/man1"

# Ensure directories exist
mkdir -p "$ROFF_DIR"
mkdir -p "$MAN_DIR"

# Function to create a new .roff file
create_roff() {
    local name="$1"
    local file="$ROFF_DIR/$name.roff"
    if [ -f "$file" ]; then
        echo "File $file already exists."
        return 1
    fi
    cat <<EOF > "$file"
.TH "$name" 1
.SH NAME
$name \- brief description
.SH SYNOPSIS
.B $name
.SH DESCRIPTION
Description of $name.
.SH ENVIRONMENT
Environment variables.
.SH FILES
Files used.
.SH STANDARDS
Standards compliance.
.SH HISTORY
History of changes.
.SH NOTES
Additional notes.
.SH CAVEATS
Caveats.
.SH BUGS
Known bugs.
.SH EXAMPLES
Examples of usage.
.SH AUTHORS
Authors of the manual page.
.SH SEE ALSO
Related manual pages.
EOF
    echo "Created $file."
}

# Function to read a .roff file
read_roff() {
    local name="$1"
    local file="$ROFF_DIR/$name.roff"
    if [ ! -f "$file" ]; then
        echo "File $file does not exist."
        return 1
    fi
    cat "$file"
}

# Function to update a .roff file
update_roff() {
    local name="$1"
    local file="$ROFF_DIR/$name.roff"
    if [ ! -f "$file" ]; then
        echo "File $file does not exist."
        return 1
    fi
    ${EDITOR:-vi} "$file"
}

# Function to delete a .roff file
delete_roff() {
    local name="$1"
    local file="$ROFF_DIR/$name.roff"
    if [ ! -f "$file" ]; then
        echo "File $file does not exist."
        return 1
    fi
    rm -f "$file"
    echo "Deleted $file."
}

# Function to generate manual pages
generate_man() {
    for file in "$ROFF_DIR"/*.roff; do
        [ -e "$file" ] || continue
        local name=$(basename "$file" .roff)
        groff -man "$file" > "$MAN_DIR/$name.1"
        echo "Generated $MAN_DIR/$name.1"
    done
}

# Main script logic
case "$1" in
    create)
        create_roff "$2"
        ;;
    read)
        read_roff "$2"
        ;;
    update)
        update_roff "$2"
        ;;
    delete)
        delete_roff "$2"
        ;;
    generate)
        generate_man
        ;;
    *)
        echo "Usage: $0 {create|read|update|delete|generate} [name]"
        exit 1
        ;;
esac
