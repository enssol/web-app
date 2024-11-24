# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: AGPL-3.0-or-later

# Main Root Makefile.am

.POSIX: # Enforce POSIX mode for portability

# Include common rules
include common-rules.am

# Define the project name and options
AUTOMAKE_OPTIONS = gnu subdir-objects dist-xz
ACLOCAL_AMFLAGS = -I m4

# Source and header files
bin_PROGRAMS = web-app
web_app_SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/config_loader.c $(SRC_DIR)/env_loader.c $(SRC_DIR)/error_handler.c $(SRC_DIR)/garbage_collector.c $(SRC_DIR)/hello.c $(SRC_DIR)/lexer.l $(SRC_DIR)/logger.c $(SRC_DIR)/parser.y $(SRC_DIR)/validator.c
web_app_HEADERS = $(HEADER_FILES)

# Define the source files
SRC_DIR = src
OBJ_DIR = objects
SRC_FILES = $(SRC_DIR)/config_loader.c $(SRC_DIR)/env_loader.c $(SRC_DIR)/main.c $(SRC_DIR)/error_handler.c $(SRC_DIR)/garbage_collector.c $(SRC_DIR)/hello.c $(SRC_DIR)/lexer.l $(SRC_DIR)/logger.c $(SRC_DIR)/parser.y $(SRC_DIR)/validator.c
OBJ_FILES = $(SRC_FILES:.c=.o)

# Define the directory for headers
web_appdir = $(INCLUDE_DIR)

# Include directories
AM_CPPFLAGS = -I$(INCLUDE_DIR) $(LIBS_CFLAGS)

# Define subdirectories
SUBDIRS = src include etc docs m4 po bin build deps dist tmp lib objects logs m4

# Additional libraries
LDADD = -lm

# Define the directory for configuration files
configdir = $(CONFIG_DIR)

# Install configuration files
dist_config_DATA = $(CONFIG_INI_FILE) $(CONFIG_CONF_FILE) $(GCC_SPEC_FILE)

# Custom rule to generate a version file
version.h: $(SRC_DIR)/version.txt
    @echo "#define VERSION \"`cat $(SRC_DIR)/version.txt`\"" > $(INCLUDE_DIR)/version.h

# Add common rules to the build process
all: generate_tags compile docs

clean: clean_tags clean_html_docs

# Clean up files
DISTCLEANFILES = Makefile.in aclocal.m4 configure config.h.in

# Add gtk-doc support
GTK_DOC_USE_LIBTOOL = 1
include $(top_srcdir)/gtk-doc.make

# Add intltool support
INTLTOOL_FILES = intltool-extract.in intltool-merge.in intltool-update.in
DISTCLEANFILES += intltool-extract intltool-merge intltool-update

# Add rules for generating .mo files
$(LINGUAS:%=%.mo): %.mo: %.po
    msgfmt -o $@ $<

# Add rules for installing .mo files
install-data-local:
    $(mkinstalldirs) $(DESTDIR)$(datadir)/locale
    for lang in $(LINGUAS); do \
        $(INSTALL_DATA) $$lang.mo $(DESTDIR)$(datadir)/locale/$$lang/LC_MESSAGES/$(PACKAGE).mo; \
    done

uninstall-local:
    for lang in $(LINGUAS); do \
        rm -f $(DESTDIR)$(datadir)/locale/$$lang/LC_MESSAGES/$(PACKAGE).mo; \
    done
