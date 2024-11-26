# Root Makefile.am

.POSIX: # Enforce POSIX mode for portability

# Include common rules
include common-rules.am

# Define the project name and options
AUTOMAKE_OPTIONS = gnu subdir-objects dist-xz
ACLOCAL_AMFLAGS = -I m4

# Define subdirectories
SUBDIRS = src include etc docs m4 po bin build deps dist tmp lib objects logs m4

# Compiler and linker definitions
CFLAGS = -Iinclude -I/usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include -I/opt/homebrew/include/glib-2.0 -I/opt/homebrew/lib/glib-2.0/include -O0 -pedantic -D_POSIX_C_SOURCE=2024L -D_XOPEN_SOURCE=800 -std=gnu23 -Wall -Wextra -Werror -DPIC -MMD -MP
AM_CFLAGS = $(CFLAGS)

# Linker flags definition
LDFLAGS = @LDFLAGS@
AM_LDFLAGS = $(LDFLAGS)

# Define glib-2.0 libraries
LIBS = -lglib-2.0 -lm

# Additional libraries
LDADD = -lm

# Include directories for glib-2.0
AM_CPPFLAGS = -I/usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include -I/opt/homebrew/include/glib-2.0 -I/opt/homebrew/lib/glib-2.0/include

# Define VPATH to search for source files in multiple directories
VPATH = src:include:lib:deps:build:etc:po:docs:dist:tmp:objects:logs:m4:bin

# Define the directory for configuration files
configdir = $(CONFIG_DIR)

# Install configuration files
dist_config_DATA = $(CONFIG_INI_FILE) $(CONFIG_CONF_FILE) $(GCC_SPEC_FILE)

# Source files
SRC_FILES = src/validator.c src/env_loader.c src/error_handler.c src/garbage_collector.c src/hello.c src/logger.c src/main.c src/config_loader.c

# Object files
OBJS = objects/validator.o objects/env_loader.o objects/error_handler.o objects/garbage_collector.o objects/hello.o objects/logger.o objects/main.o objects/config_loader.o

# Define the path to otool
OTOOL = /usr/bin/otool

# Consolidated 'all' target
all: all-am all-local
    @echo "Building all targets..."

# Compilation rules for each source file
objects/validator.o: src/validator.c
    $(CC) $(CFLAGS) -c src/validator.c -o objects/validator.o

objects/env_loader.o: src/env_loader.c
    $(CC) $(CFLAGS) -c src/env_loader.c -o objects/env_loader.o

objects/error_handler.o: src/error_handler.c
    $(CC) $(CFLAGS) -c src/error_handler.c -o objects/error_handler.o

objects/garbage_collector.o: src/garbage_collector.c
    $(CC) $(CFLAGS) -c src/garbage_collector.c -o objects/garbage_collector.o

objects/hello.o: src/hello.c
    $(CC) $(CFLAGS) -c src/hello.c -o objects/hello.o

objects/logger.o: src/logger.c
    $(CC) $(CFLAGS) -c src/logger.c -o objects/logger.o

objects/main.o: src/main.c
    $(CC) $(CFLAGS) -c src/main.c -o objects/main.o

objects/config_loader.o: src/config_loader.c
    $(CC) $(CFLAGS) -c src/config_loader.c -o objects/config_loader.o

# Rule to use otool to display shared library dependencies
otool_deps:
    @echo "Displaying shared library dependencies with otool..."
    $(OTOOL) -L $(OBJS)

# Include dependency files
-include $(DEPS)

# Consolidated 'clean' target
clean: clean-am clean-local
    @echo "Cleaning build artifacts..."
    rm -f cppcheck.xml cppcheck_errors.log

# Custom rule to generate a version file
version.h: $(SRC_DIR)/version.txt
    @echo "#define VERSION \"`cat $(SRC_DIR)/version.txt`\"" > $(INCLUDE_DIR)/version.h

# Clean up files
DISTCLEANFILES = Makefile.in aclocal.m4 configure config.h.in

# Add gtk-doc support
GTK_DOC_USE_LIBTOOL = 1
include $(top_srcdir)/gtk-doc.make

# Add rules for generating .mo files
$(LINGUAS:%=%.mo): %.mo: %.po
    msgfmt -o $@ $<

# Consolidated 'install-data-local' target
install-data-local:
    @installfiles=`echo $(builddir)/html/*`; \
    if test "$$installfiles" = '$(builddir)/html/*'; then \
        echo 1>&2 'Nothing to install' ; \
    else \
        if test -n "$(DOC_MODULE_VERSION)"; then \
            installdir="$(DESTDIR)$(TARGET_DIR)-$(DOC_MODULE_VERSION)"; \
        else \
            installdir="$(DESTDIR)$(TARGET_DIR)"; \
        fi; \
        $(mkinstalldirs) $${installdir} ; \
        for i in $$installfiles; do \
            echo ' $(INSTALL_DATA) '$$i ; \
            $(INSTALL_DATA) $$i $${installdir}; \
        done; \
        if test -n "$(DOC_MODULE_VERSION)"; then \
            mv -f $${installdir}/$(DOC_MODULE).devhelp2 \
                $${installdir}/$(DOC_MODULE)-$(DOC_MODULE_VERSION).devhelp2; \
        fi; \
        $(GTKDOC_REBASE) --relative --dest-dir=$(DESTDIR) --html-dir=$${installdir}; \
    fi

# Consolidated 'uninstall-local' target
uninstall-local:
    @if test -n "$(DOC_MODULE_VERSION)"; then \
        installdir="$(DESTDIR)$(TARGET_DIR)-$(DOC_MODULE_VERSION)"; \
    else \
        installdir="$(DESTDIR)$(TARGET_DIR)"; \
    fi; \
    rm -rf $${installdir}

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

# Consolidated 'profile' target
profile:
    @echo "Profiling the application..."
    gprof $(binary) > profile.txt

# Coverage analysis with gcov
coverage:
    @echo "Running gcov for coverage analysis..."
    find $(srcdir) -name '*.c' -exec gcov {} \;

# Define paths and other variables
APP_NAME = web-app
BIN_DIR = bin
BUILD_DIR = build
COMPLEXITY = complexity
DEPS_DIR = deps
DIST_DIR = dist
DOCS_DIR = docs
DUMPBIN = tmp
INCLUDE_DIR = include
LIB_DIR = lib
LOCALES_DIR = po
LOGS_DIR = logs
MACROS_DIR = m4
OBJ_DIR = objects
SRC_DIR = src
TESTS_DIR = tests
TMP_DIR = tmp

# Handle undefined variables
LEXLIB = -lfl
LIBOBJS =
LTLIBOBJS =
MANIFEST_TOOL = manifest-tool
PACKAGE_URL = https://github.com/enssol/web-app
PKG_CONFIG_LIBDIR = /usr/local/lib/pkgconfig:/opt/homebrew/lib/pkgconfig
SET_MAKE = $(MAKE)
YFLAGS = -d
ac_ct_DUMPBIN = dumpbin
am__quote = \"
am__rm_f_notfound = rm -f
build_alias =
host_alias =
top_build_prefix =
