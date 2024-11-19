# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: AGPL-3.0-or-later

# src/Makefile.am

# Define the project name
AUTOMAKE_OPTIONS = gnu subdir-objects
bin_PROGRAMS = web_app

# Source and header files
include ../config/sources.txt
include ../config/header_sources.txt

web_app_SOURCES = $(SRC_FILES)
web_app_HEADERS = $(HEADER_FILES)

# Define the directory for headers
web_appdir = $(includedir)/web_app

# Include directories
AM_CPPFLAGS = -I$(srcdir)/include

# Additional libraries
LDADD = -lm

# Compiler and linker flags from gcc.spec
AM_CFLAGS = @CC1_FLAGS@
AM_LDFLAGS = @LINK_FLAGS@

# Install configuration files
dist_config_DATA = ../config/config.conf ../config/config.ini ../config/gcc.spec \
                   ../config/sources.txt ../config/test_sources.txt ../config/header_sources.txt

# Install directories
configdir = $(sysconfdir)/web_app
config_DATA = $(dist_config_DATA)
