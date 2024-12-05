# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: 	AGPL-3.0-or-later

#!/bin/sh

# Define directories
SRC_DIR="src"
INCLUDE_DIR="include"
BIN_DIR="bin"
OUTPUT_FILE="$BIN_DIR/web_app"

# Create bin directory if it doesn't exist
mkdir -p "$BIN_DIR"

# Compiler and linker flags
CFLAGS="-std=c11 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -I$INCLUDE_DIR -Wall -Werror -g -ffunction-sections -fdata-sections -march=x86-64"
LDFLAGS="-static -L/usr/lib"

# Compile source files
tcc $CFLAGS $SRC_DIR/*.c -o $OUTPUT_FILE $LDFLAGS

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Executable created at $OUTPUT_FILE"
else
    echo "Compilation failed."
    exit 1
fi
