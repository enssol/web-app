# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: AGPL-3.0-or-later

# Define the project name and options
AUTOMAKE_OPTIONS = gnu subdir-objects dist-xz
ACLOCAL_AMFLAGS = -I m4
bin_PROGRAMS = web-app

# Include the .env file if it exists
-include .env
export $(shell cat .env | grep -v '#' | awk '/=/ {print $$1}')

# Source and header files
include $(SOURCES_FILE)
include $(HEADER_FILE)

# Define the source files for the src directory
lib_LTLIBRARIES = libwebapp.la
libwebapp_la_SOURCES = $(SRC_DIR)/config_loader.c $(SRC_DIR)/env_loader.c $(SRC_DIR)/error_handler.c $(SRC_DIR)/garbage_collector.c $(SRC_DIR)/hello.c $(SRC_DIR)/logger.c $(SRC_DIR)/main.c $(SRC_DIR)/validator.c
libwebapp_la_LDFLAGS = -static

# Define the directory for the binary
web_app_SOURCES = $(SRC_FILES) $(BUILT_SOURCES)
web_app_HEADERS = $(HEADER_FILES)

# Define the directory for headers
web_appdir = $(INCLUDE_DIR)

# Include directories
AM_CPPFLAGS = -I$(INCLUDE_DIR)

# Define subdirectories
SUBDIRS = src include etc docs m4 po bin build deps dist tmp lib objects logs m4

# Compiler and linker flags from gcc.spec
AM_CFLAGS = @CC1_FLAGS@
AM_LDFLAGS = @LINK_FLAGS@ -static

# Additional libraries
LDADD = -lm

# Define the directory for configuration files
configdir = $(CONFIG_DIR)

# Install configuration files
dist_config_DATA = $(CONFIG_INI_FILE) $(CONFIG_CONF_FILE) $(GCC_SPEC_FILE)

# Bison rules
BUILT_SOURCES = $(SRC_DIR)/parser.c $(INCLUDE_DIR)/parser.h $(INCLUDE_DIR)/version.h
CLEANFILES = $(SRC_DIR)/parser.c $(INCLUDE_DIR)/parser.h $(INCLUDE_DIR)/version.h

# Ensure parser.c is built only once
noinst_LIBRARIES = libparser.a
libparser_a_SOURCES = $(SRC_DIR)/parser.c

$(SRC_DIR)/parser.c $(INCLUDE_DIR)/parser.h: $(SRC_DIR)/parser.y
	bison -d $(SRC_DIR)/parser.y
	mv parser.tab.c $(SRC_DIR)/parser.c
	mv parser.tab.h $(INCLUDE_DIR)/parser.h

# Custom rule to generate a version file
version.h: $(SRC_DIR)/version.txt
	@echo "#define VERSION \"`cat $(SRC_DIR)/version.txt`\"" > $(INCLUDE_DIR)/version.h

# Optional: Files to include in the distribution tarball.
# This ensures these files are included when running `make dist`.
EXTRA_DIST = README.md LICENSE INSTALL AUTHORS ChangeLog NEWS etc/config.conf etc/config.ini etc/gcc.spec docs/

# Use VPATH to specify the directories where the source files are located
VPATH = $(srcdir):$(srcdir)/$(SRC_DIR):$(srcdir)/$(INCLUDE_DIR)

# Include gettext files
GETTEXT_PACKAGE = enveng-webapp

# For flex
LEX = flex
LEX_OUTPUT_ROOT = lex.yy

# Flex rules
parser.c: parser.l
	$(LEX) -o$@ $<

# Clean up files
DISTCLEANFILES = Makefile.in aclocal.m4 configure config.h.in
