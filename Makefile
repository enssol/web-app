# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: AGPL-3.0-or-later

# Package version
VERSION=1.0.0

# Compiler selection
CC=gcc

# Environment-specific settings
ifeq ($(ENV),prod)
	# Production: Static linking, full optimization
	OPTFLAGS=-O3 -march=native -mtune=native \
			 -fdata-sections -ffunction-sections \
			 -flto -fno-common -fstrict-aliasing
	LDFLAGS=-static -lbearssl -Wl,--gc-sections -Wl,-O1
	LIBS=
else
	# Development: Dynamic linking, debugging
	OPTFLAGS=-O0 -g3 -fanalyzer
	LDFLAGS=-lbearssl -Wl,-rpath,/usr/lib
	LIBS=
endif

# Base flags (always included)
BASEFLAGS=-std=c90 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=500 \
		  -Wall -Wextra -pedantic -Werror -Wshadow \
		  -Wconversion -Wstrict-prototypes -Wmissing-prototypes

# Security flags
SECFLAGS=-fstack-protector-strong -fstack-check \
		 -Warray-bounds -Wstack-protector \
		 -Wformat=2 -Wformat-security \
		 -Wformat-overflow=2 -Wformat-truncation=2 \
		 -Walloca -Wvla -fno-omit-frame-pointer

# Combined CFLAGS
CFLAGS=$(BASEFLAGS) $(OPTFLAGS) $(SECFLAGS) -I/usr/include -fPIC
CFLAGS += -D_GNU_SOURCE -std=gnu99
CFLAGS += -Wno-error=declaration-after-statement
CFLAGS += -Wno-error=c90-c99-compat

# Test-specific flags
TEST_LDFLAGS=-L/usr/lib -lbearssl -Wl,-rpath,/usr/lib
TEST_LIBS=-lcunit

# Includes
INCLUDES=-I./include

# Directories
SRCDIR=src
OBJDIR=obj
BINDIR=bin

# Sources
SERVER_SOURCES=$(SRCDIR)/server.c $(SRCDIR)/bearssl_wrapper.c
SERVER_OBJECTS=$(SERVER_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

MAIN_SOURCES=$(SRCDIR)/main.c
MAIN_OBJECTS=$(MAIN_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

TARGET=$(BINDIR)/webserver
TEST_TARGET=$(BINDIR)/test_server

# Package name and version
PACKAGE_NAME = webserver
PACKAGE_VERSION = 1.0.0
PACKAGE_FILE = $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz

.PHONY: all clean test package prod prepare

all: $(TARGET)

$(TARGET): $(SERVER_OBJECTS) $(MAIN_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $^ -o $@ $(LDFLAGS) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TEST_TARGET): $(SERVER_OBJECTS) test/test_server.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -DTEST_BUILD $^ -o $@ $(LDFLAGS) $(TEST_LIBS)

clean:
	rm -rf $(OBJDIR) $(BINDIR) webserver-*.tar*

test: CFLAGS += -DTEST_BUILD
test: prepare
	$(CC) $(CFLAGS) -DTEST_BUILD \
		test/test_server.c \
		src/server.c \
		src/bearssl_wrapper.c \
		-o test/test_server $(TEST_LDFLAGS) $(TEST_LIBS)

# Package target
package: $(TARGET)
	@echo "Creating package webserver-$(VERSION).tar.gz..."
	@rm -rf pkg
	@mkdir -p pkg || exit 1
	@cp $(TARGET) pkg/ || exit 1
	@cp -r www pkg/ || exit 1
	@tar -czf webserver-$(VERSION).tar.gz pkg/ || exit 1
	@rm -rf pkg
	@echo "Package created: webserver-$(VERSION).tar.gz"

# Production build with package
prod: CFLAGS += -O3
prod: $(TARGET) package
