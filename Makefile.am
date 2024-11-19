# Include the .env file if it exists
if [ -f config/.env ]; then \
  include config/.env; \
  export $(shell cat config/.env | grep -v '#' | awk '/=/ {print $$1}'); \
fi

# Define the project name
AUTOMAKE_OPTIONS = gnu subdir-objects
bin_PROGRAMS = web_app
TESTS = tests/test_program

# Source and header files
include $(CONFIG_DIR)/sources.txt
include $(CONFIG_DIR)/header_sources.txt
include $(CONFIG_DIR)/test_sources.txt

web_app_SOURCES = $(SRC_FILES) $(BUILT_SOURCES)
web_app_HEADERS = $(HEADER_FILES)

# Define the test source files
check_PROGRAMS = $(TEST_SRC_FILES)

# Define the directory for headers
web_appdir = $(includedir)/web_app

# Define the directory for test programs
check_PROGRAMSdir = $(bindir)/tests

# Include directories
AM_CPPFLAGS = -I$(INCLUDE_DIR)

# Additional libraries
LDADD = -lm

# Compiler and linker flags from gcc.spec
AM_CFLAGS = @CC1_FLAGS@
AM_LDFLAGS = @LINK_FLAGS@ -static

# Define the directory for configuration files
configdir = $(sysconfdir)/config

# Install configuration files
dist_config_DATA = $(CONFIG_DIR)/config.conf $(CONFIG_DIR)/config.ini $(CONFIG_DIR)/gcc.spec

# Bison rules
BUILT_SOURCES = parser.c parser.h
CLEANFILES = parser.c parser.h

parser.c parser.h: parser.y
	bison -d parser.y
	mv parser.tab.c parser.c
	mv parser.tab.h parser.h

# Include the src and tests directories
SUBDIRS = src tests
