# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: 	AGPL-3.0-or-later

# etc/Makefile.am

.POSIX: # Enforce POSIX mode for portability

# Include common rules
include ../common-rules.am

# Add generate_tags to the build process
all: generate_tags

# Add clean_tags to the clean process
clean: clean_tags

# Define the configuration files for the etc directory
configdir = $(sysconfdir)
dist_config_DATA = config.conf config.ini gcc.spec header_sources.txt sources.txt .autoindent process_files.gawk
