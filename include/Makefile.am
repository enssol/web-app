# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: 	AGPL-3.0-or-later

# include/Makefile.am

.POSIX: # Enforce POSIX mode for portability

# Include common rules
include ../common-rules.am

# Define the header files for the include directory
include_HEADERS = config.h config_loader.h env_loader.h error_handler.h garbage_collector.h hello.h logger.h parser.h validator.h gettext.h

# Add generate_tags to the build process
all: generate_tags

# Add clean_tags to the clean process
clean: clean_tags
