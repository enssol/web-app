# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: 	AGPL-3.0-or-later

# docs/Makefile.am

.POSIX: # Enforce POSIX mode for portability

# Include common rules
include ../common-rules.am

# Add generate_docs to the build process
all: generate_docs generate_html_docs

# Add clean_html_docs to the clean process
clean: clean_html_docs

# Define the documentation files for the docs directory
dist_doc_DATA = user_guide.md developer_guide.md api_reference.md

# Specify the directory for the documentation files
docdir = $(datadir)/doc/$(PACKAGE)

# Texinfo source files
TEXINFO_FILES = manual.texi

# Generated documentation files
HTML_FILES = manual.html

# Add the generated documentation files to the distribution
EXTRA_DIST = $(HTML_FILES)

# Clean up generated files
CLEANFILES = $(HTML_FILES)
