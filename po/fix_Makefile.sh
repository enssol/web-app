#!/bin/bash

file="po/Makefile.in.in"

# Check if the file exists
if [[ ! -f "${file}" ]]; then
    echo "File ${file} not found! Skipping fix."
    exit 0
fi

# Remove blank lines following trailing backslashes
sed -i ':a;N;$!ba;s/\\\n\n/\\\n/g' "${file}"

# Replace .SUFFIXES target with SUFFIXES variable
sed -i 's/^\(\.SUFFIXES:\)/#\1/' "${file}"
sed -i 's/^\(\.SUFFIXES: .po .gmo .sed .sin .nop .po-create .po-update\)/SUFFIXES = .po .gmo .sed .sin .nop .po-create .po-update/' "${file}"

echo "Fixes applied to ${file}"
