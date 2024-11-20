# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: AGPL-3.0-or-later

# Define the project name and options
AUTOMAKE_OPTIONS = gnu subdir-objects dist-xz
ACLOCAL_AMFLAGS = -I m4
bin_PROGRAMS = web-app

# Include the .env file if it exists
# Include the .env file if it exists
-include .env
export $(shell cat .env | grep -v '#' | awk '/=/ {print $$1}')

# Source and header files
include $(SOURCES_FILE)
include $(HEADER_FILE)

# Define the directory for the binary
web_app_SOURCES = $(SRC_FILES) $(BUILT_SOURCES)
web_app_HEADERS = $(HEADER_FILES)

# Define the directory for headers
web_appdir = $(INCLUDE_DIR)

# Include directories
AM_CPPFLAGS = -I$(INCLUDE_DIR)

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
BUILT_SOURCES = $(SRC_DIR)/parser.c $(INCLUDE_DIR)/parser.h
CLEANFILES = $(SRC_DIR)/parser.c $(INCLUDE_DIR)/parser.h

$(SRC_DIR)/parser.c $(INCLUDE_DIR)/parser.h: $(SRC_DIR)/parser.y
	bison -d $(SRC_DIR)/parser.y
	mv parser.tab.c $(SRC_DIR)/parser.c
	mv parser.tab.h $(INCLUDE_DIR)/parser.h

# Custom rule to generate a version file
version.h: $(SRC_DIR)/version.txt
	@echo "#define VERSION \"`cat $(SRC_DIR)/version.txt`\"" > $(INCLUDE_DIR)/version.h

# Add version.h to the list of built sources
BUILT_SOURCES += $(INCLUDE_DIR)/version.h

# Ensure version.h is cleaned up
CLEANFILES += $(INCLUDE_DIR)/version.h

# Include the src directory
SUBDIRS = $(SRC_DIR) $(LIB_DIR) $(INCLUDE_DIR) $(CONFIG_DIR) $(BIN_DIR) $(OBJ_DIR) $(DIST_DIR) $(DOCS_DIR) $(TMP_DIR) $(DOCS_DIR) $(DEPS_DIR) $(BUILD_DIR) $(MACROS_DIR)

# Optional: Files to include in the distribution tarball.
# This ensures these files are included when running `make dist`.
EXTRA_DIST = README.md LICENSE INSTALL AUTHORS ChangeLog NEWS etc/config.conf etc/config.ini etc/gcc.spec docs/

# Use VPATH to specify the directories where the source files are located
VPATH = $(srcdir):$(srcdir)/$(SRC_DIR):$(srcdir)/$(INCLUDE_DIR)
