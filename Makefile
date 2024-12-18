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
		  -DBR_USE_UNIX_TIME -DBR_SLOW_MUL15 -DBR_LOMUL \
          -DBR_NO_INLINE_ASM -DBR_STRICT_ANSI_C -DBR_SAFE_ALIASES \
		  -Dinline= -Wall -Wextra -pedantic -Werror -Wshadow \
		  -Wconversion -Wstrict-prototypes -Wmissing-prototypes

# Security flags
SECFLAGS=-fstack-protector-strong -fstack-check \
		 -Warray-bounds -Wstack-protector \
		 -Wformat=2 -Wformat-security \
		 -Wformat-overflow=2 -Wformat-truncation=2 \
		 -Walloca -Wvla -fno-omit-frame-pointer

# Combined CFLAGS
CFLAGS=$(BASEFLAGS) $(OPTFLAGS) $(SECFLAGS) -I/usr/include -fPIC

# Test-specific flags
TEST_LDFLAGS=-L/usr/lib -lbearssl -Wl,-rpath,/usr/lib
TEST_LIBS=$(LIBS) -lcunit
TEST_CFLAGS = $(CFLAGS) -DTEST_BUILD -DDEBUG

# Includes
INCLUDES=-I./include

# Directories
SRCDIR=src
OBJDIR=obj
BINDIR=bin

# Test directories and files
TEST_SRCDIR=test
TEST_OBJDIR=$(OBJDIR)/test
TEST_BINDIR=$(BINDIR)
TEST_TARGET=$(TEST_BINDIR)/test_server
TEST_SOURCES=$(TEST_SRCDIR)/test_server.c
TEST_OBJECTS=$(TEST_SOURCES:$(TEST_SRCDIR)/%.c=$(TEST_OBJDIR)/%.o)

# Sources
SERVER_SOURCES=$(SRCDIR)/server.c
SERVER_OBJECTS=$(SERVER_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

MAIN_SOURCES=$(SRCDIR)/main.c
MAIN_OBJECTS=$(MAIN_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

TARGET=$(BINDIR)/webserver

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

# Test build rules (update)
$(TEST_TARGET): CFLAGS+=-DTEST_BUILD -DDEBUG
$(TEST_TARGET): $(TEST_OBJECTS) $(filter-out $(OBJDIR)/main.o,$(SERVER_OBJECTS))
	@mkdir -p $(TEST_BINDIR)
	$(CC) $(TEST_CFLAGS) $(INCLUDES) $^ -o $@ $(TEST_LDFLAGS) $(TEST_LIBS)

$(TEST_OBJDIR)/%.o: $(TEST_SRCDIR)/%.c
	@mkdir -p $(TEST_OBJDIR)
	$(CC) $(TEST_CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(TARGET) $(SERVER_OBJECTS) $(MAIN_OBJECTS) $(TEST_OBJECTS) $(TEST_TARGET)
	rm -rf $(OBJDIR) $(BINDIR)

test: $(TEST_TARGET)
	@mkdir -p $(TEST_BINDIR)
	@echo "Running tests..."
	@./$(TEST_TARGET)

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
