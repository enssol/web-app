# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: 	AGPL-3.0-or-later

# bin/Makefile.am

.POSIX: # Enforce POSIX mode for portability

# Include common rules
include ../common-rules.am

# Include the .env file if it exists
-include .env
export $(shell cat .env | grep -v '#' | awk '/=/ {print $$1}')

# Define the binary files for the bin directory
bin_PROGRAMS = webapp
webapp_SOURCES = main.c
webapp_LDADD = ../lib/libwebapp.la

# Add clib_install, perf, generate_tags, cflow, and stow to the build process
all: clib_install perf generate_tags cflow stow

# Add clean_tags to the clean process
clean: clean_tags
