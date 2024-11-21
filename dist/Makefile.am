# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: 	AGPL-3.0-or-later

# dist/Makefile.am

# Include the .env file if it exists
-include .env
export $(shell cat .env | grep -v '#' | awk '/=/ {print $$1}')

# Define the distribution files for the dist directory
dist_noinst_DATA = README.md LICENSE INSTALL AUTHORS ChangeLog NEWS
