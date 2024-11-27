#!/bin/bash
# Generate dependencies for all C files in the src/ directory

# Loop through all C source files
for src_file in src/*.c; do
    # Get the corresponding dependency file name
    dep_file="${src_file%.c}.d"

    # Use gcc to generate the dependencies for each source file
    gcc -M "${src_file}" > "${dep_file}"

    # Optionally, you can include other flags or dependencies
    # Example: gcc -M -Iinclude $src_file > $dep_file
done
