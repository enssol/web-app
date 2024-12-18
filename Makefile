# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: 	AGPL-3.0-or-later
CC=gcc
CFLAGS=-std=c90 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=500 -Wall -ansi \
	   -Wextra -pedantic -Werror -Wshadow -Wconversion -Wstrict-prototypes \
	   -Wmissing-prototypes -fanalyzer -fstack-protector-strong -fstack-check \
	   -fdata-sections -ffunction-sections -fno-common -fstrict-aliasing \
	   -Warray-bounds -Wstack-protector -Wformat=2 -Wformat-security \
	   -Wformat-overflow=2 -Wformat-truncation=2 -Walloca -Wvla \
	   -fno-omit-frame-pointer
LDFLAGS=-static
INCLUDES=-I./include

SRCDIR=src
OBJDIR=obj
BINDIR=bin

SERVER_SOURCES=$(SRCDIR)/server.c
SERVER_OBJECTS=$(SERVER_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

MAIN_SOURCES=$(SRCDIR)/main.c
MAIN_OBJECTS=$(MAIN_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

TARGET=$(BINDIR)/webserver
TEST_TARGET=$(BINDIR)/test_server

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(SERVER_OBJECTS) $(MAIN_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TEST_TARGET): $(SERVER_OBJECTS) test/test_server.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -DTEST_BUILD $^ -o $@ -lcunit

clean:
	rm -rf $(OBJDIR) $(BINDIR)

test: $(TEST_TARGET)
	./$(TEST_TARGET)
