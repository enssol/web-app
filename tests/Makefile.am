# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: 	AGPL-3.0-or-later

# tests/Makefile.am

# Include the .env file if it exists
if [ -f ../config/.env ]; then \
  include ../config/.env; \
  export $(shell cat ../config/.env | grep -v '#' | awk '/=/ {print $$1}'); \
fi

# Define the test programs
TESTS = test_config_loader test_env_loader test_error_handler test_garbage_collector test_hello test_logger test_main test_validator

# Source and header files for tests
include ../config/test_sources.txt
include ../config/header_sources.txt

# Define the test source files
check_PROGRAMS = $(TESTS)

# Include directories
AM_CPPFLAGS = -I$(srcdir)/../include

# Additional libraries
LDADD = -lm

# Compiler and linker flags from gcc.spec
AM_CFLAGS = @CC1_FLAGS@
AM_LDFLAGS = @LINK_FLAGS@ -static

# Define the directory for headers
test_config_loader_SOURCES = test_config_loader.c $(HEADER_FILES)
test_env_loader_SOURCES = test_env_loader.c $(HEADER_FILES)
test_error_handler_SOURCES = test_error_handler.c $(HEADER_FILES)
test_garbage_collector_SOURCES = test_garbage_collector.c $(HEADER_FILES)
test_hello_SOURCES = test_hello.c $(HEADER_FILES)
test_logger_SOURCES = test_logger.c $(HEADER_FILES)
test_main_SOURCES = test_main.c $(HEADER_FILES)
test_validator_SOURCES = test_validator.c $(HEADER_FILES)

# Define the directory for test programs
check_PROGRAMSdir = $(bindir)/tests

# Install directories
configdir = $(sysconfdir)/web_app
config_DATA = $(dist_config_DATA)

# Install configuration files
dist_config_DATA = ../config/config.conf ../config/config.ini ../config/gcc.spec \
				   ../config/sources.txt ../config/test_sources.txt ../config/header_sources.txt

