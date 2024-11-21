# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: 	AGPL-3.0-or-later

# build/Makefile.am

# Include the .env file if it exists
-include .env
export $(shell cat .env | grep -v '#' | awk '/=/ {print $$1}')

# Define the auxiliary files for the build directory
EXTRA_DIST = build_script.sh

# Specify the directory for the auxiliary files
builddir = $(srcdir)/build

# Install the auxiliary files
dist_noinst_SCRIPTS = build_script.sh
