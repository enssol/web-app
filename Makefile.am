# Define the project name
AUTOMAKE_OPTIONS = gnu subdir-objects
bin_PROGRAMS = web_app
TESTS = tests/test_program

# Source and header files
include config/sources.txt
include config/header_sources.txt
include config/test_sources.txt

web_app_SOURCES = $(SRC_FILES)
web_app_HEADERS = $(HEADER_FILES)

# Define the test source files
check_PROGRAMS = $(TEST_SRC_FILES)

# Define the directory for headers
web_appdir = $(includedir)/web_app

# Define the directory for test programs
check_PROGRAMSdir = $(bindir)/tests

# Include directories
AM_CPPFLAGS = -I$(srcdir)/include

# Additional libraries
LDADD = -lm

# Compiler and linker flags from gcc.spec
AM_CFLAGS = @CC1_FLAGS@
AM_LDFLAGS = @LINK_FLAGS@

# Define the directory for configuration files
configdir = $(sysconfdir)/config

# Install configuration files
dist_config_DATA = config/config.conf config/config.ini config/gcc.spec
