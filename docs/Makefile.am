# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: 	AGPL-3.0-or-later

# docs/Makefile.am

# Include the .env file if it exists
-include .env
export $(shell cat .env | grep -v '#' | awk '/=/ {print $$1}')

# Define the documentation files for the docs directory
dist_doc_DATA = user_guide.md developer_guide.md api_reference.md

# Specify the directory for the documentation files
docdir = $(datadir)/doc/$(PACKAGE)
