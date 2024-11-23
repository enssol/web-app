# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: AGPL-3.0-or-later

# objects/Makefile.am

.POSIX: # Enforce POSIX mode for portability

# Include common rules
include ../common-rules.am

# Use libtool to create static libraries
lib_LTLIBRARIES = libenveng.la libwebapp.la

# Explicitly list source files to avoid wildcard issues
libenveng_la_SOURCES = src/config_loader.c src/env_loader.c src/error_handler.c \
                       src/garbage_collector.c src/hello.c src/logger.c \
                       src/main.c src/validator.c src/parser.y src/lexer.l
libenveng_la_LDFLAGS = -static

libwebapp_la_SOURCES = $(libenveng_la_SOURCES)
libwebapp_la_LDFLAGS = -static

# Rules for Bison and Flex
BUILT_SOURCES = src/parser.c src/lexer.c
CLEANFILES = src/parser.c src/lexer.c

src/parser.c src/parser.h: src/parser.y
    $(AM_V_GEN)bison -d -o src/parser.c src/parser.y

src/lexer.c: src/lexer.l src/parser.h
    $(AM_V_GEN)flex -o src/lexer.c src/lexer.l

# Add autoindent to the build process
all: autoindent generate_tags

# Add clean_tags to the clean process
clean: clean_tags
