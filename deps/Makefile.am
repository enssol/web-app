# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: 	AGPL-3.0-or-later

# deps/Makefile.am

# Include the .env file if it exists
-include .env
export $(shell cat .env | grep -v '#' | awk '/=/ {print $$1}')

# Generate the dependency files list
gen_deps_list = $(srcdir)/deps/generate_deps.sh

# Define the dependency files for the deps directory
DEPS_FILES = $(depsdir)/deps_list.txt

# Specify the directory for the dependency files
depsdir = $(srcdir)/deps

# Install the dependency files
dist_noinst_DATA = $(DEPS_FILES)

# Hook to generate the dependency files list before building
all: $(gen_deps_list)
	@$(gen_deps_list)
