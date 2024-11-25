# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: 	AGPL-3.0-or-later

# lib/Makefile.am

.POSIX: # Enforce POSIX mode for portability

# Include common rules
include ../common-rules.am

# Define the library files for the lib directory
lib_LIBRARIES = libunity.a
libunity_a_SOURCES = unity.c

# Add generate_tags to the build process
all: generate_tags

# Add clean_tags to the clean process
clean: clean_tags
