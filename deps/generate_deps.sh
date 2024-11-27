#!/bin/sh
# Generate dependencies for all C files in the src/ directory

# Loop through all C source files
for src_file in src/*.c; do
# Check if the glob pattern didn't match any files
if [ ! -e "${src_file}" ]; then
    echo "No C source files found in src/ directory."
    exit 1
fi

# Get the corresponding dependency file name
dep_file="deps/$(basename "${src_file%.c}.d")"

# Use gcc to generate the dependencies for each source file
gcc -M "${src_file}" > "${dep_file}"

    # Use gcc to generate the dependencies for each header file
gcc -M -Iinclude "${src_file}" > "${dep_file}"
done
