# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: 	AGPL-3.0-or-later

# src/Makefile.am

.POSIX: # Enforce POSIX mode for portability

# Include common rules
include ../common-rules.am

# Define the source files for the src directory
lib_LTLIBRARIES = libwebapp.la
libwebapp_la_SOURCES = config_loader.c env_loader.c error_handler.c garbage_collector.c hello.c logger.c main.c validator.c parser.c lexer.c
libwebapp_la_LDFLAGS = -static

# Flex and Bison rules
BUILT_SOURCES = parser.c lexer.c
CLEANFILES = parser.c lexer.c parser.h
parser.c parser.h: parser.y
    bison -d -o $(OBJ_DIR)/parser.c $(SRC_DIR)/parser.y
lexer.c: lexer.l parser.h
    flex -o $(OBJ_DIR)/lexer.c $(SRC_DIR)/lexer.l

# Add autoindent to the build process
all: autoindent generate_tags generate_metadata process_metadata include_metadata splint cscope

# Define splint target
.PHONY: splint
splint:
    @echo "Running splint static code analysis..."
    splint $(SRC_DIR)/*.c

# Define cscope target
.PHONY: cscope
cscope:
    @echo "Generating cscope database..."
    cscope -b -R -s $(SRC_DIR)

# Add gettext rules
all-local: generate_mo_files
install-data-local: install_mo_files

# Add clean_tags to the clean process
clean: clean_tags

# Profiling with gprof
profile:
    @echo "Running gprof for profiling..."
    ./web-app
    gprof ./web-app gmon.out > gprof.out

# Coverage analysis with gcov
coverage:
    @echo "Running gcov for coverage analysis..."
    find . -name '*.c' -exec gcov {} \;
