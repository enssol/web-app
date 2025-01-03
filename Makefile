# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: AGPL-3.0-or-later

# Detect environment and set compiler
ifeq ($(shell which gcc 2>/dev/null),)
	$(error gcc not found)
endif

# Check if we're on Alpine/musl system
LIBC_CHECK := $(shell ldd --version 2>&1 | grep -i musl)
ifneq ($(LIBC_CHECK),)
	# On Alpine/musl system, use gcc directly
	CC = gcc
else
	# On other systems, require musl-gcc
	ifeq ($(shell which musl-gcc 2>/dev/null),)
		$(error musl-gcc not found)
	endif
	CC = musl-gcc
endif

# Source common settings
include config/shared-config.sh

SRCDIR = src
TESTDIR = test
OBJDIR = obj
BINDIR = bin
INCLUDEDIR = include

# Add version info
VERSION ?= $(shell . ./scripts/config/shared-config.sh && echo $$VERSION)
MAJOR_VERSION ?= $(shell . ./scripts/config/shared-config.sh && echo $$MAJOR_VERSION)
MINOR_VERSION ?= $(shell . ./scripts/config/shared-config.sh && echo $$MINOR_VERSION)
PATCH_VERSION ?= $(shell . ./scripts/config/shared-config.sh && echo $$PATCH_VERSION)

# Combine all CFLAGS
PROD_CFLAGS ?= $(shell . ./scripts/config/shared-config.sh && echo $$PROD_CFLAGS)
PROD_CFLAGS += $(LANG_FLAGS) $(WARN_FLAGS) $(SEC_FLAGS) $(OPT_FLAGS) -static

# Combine all test CFLAGS
TEST_CFLAGS ?= $(shell . ./scripts/config/shared-config.sh && echo $$TEST_CFLAGS)
TEST_CFLAGS += $(TEST_BASIC) $(TEST_DEBUG) $(TEST_COVERAGE) $(filter-out -O3,$(TEST_ANALYSIS))

# Source files
SRC = $(wildcard $(SRCDIR)/*.c)
TEST_SRC = $(wildcard $(TESTDIR)/*.c)
PROD_SRC = $(SRC)

# Add header files tracking
HDRS = $(wildcard $(INCLUDEDIR)/*.h)

# Object files
PROD_OBJ = $(PROD_SRC:$(SRCDIR)/%.c=$(OBJDIR)/prod/%.o)
TEST_SRC_NO_MAIN = $(filter-out $(SRCDIR)/main.c,$(SRC))
TEST_OBJ = $(TEST_SRC_NO_MAIN:$(SRCDIR)/%.c=$(OBJDIR)/test/%.o) \
		   $(TEST_SRC:$(TESTDIR)/%.c=$(OBJDIR)/test/%.o)

# Dependency files
DEPFILES := $(PROD_OBJ:.o=.d) $(TEST_OBJ:.o=.d)

PROD_TARGET ?= $(shell . ./scripts/config/shared-config.sh && echo $$PROD_TARGET)
PROD_TARGET = $(BINDIR)/web_server
TEST_TARGET ?= $(shell . ./scripts/config/shared-config.sh && echo $$TEST_TARGET)
TEST_TARGET = $(BINDIR)/test_web_server

# Build directories
OBJDIRS = $(OBJDIR)/prod $(OBJDIR)/test
BINDIRS = $(BINDIR)
ALLDIRS = $(OBJDIRS) $(BINDIRS)

.PHONY: all prod test dist clean-dist release clean check uninstall debug help distclean reset verify-cleanup

all: prod

prod: $(PROD_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

check: $(TEST_TARGET)
	./$(TEST_TARGET) --no-coverage

debug: $(TEST_TARGET)
	gdb $(TEST_TARGET)

help:
	@echo "Available targets:"
	@echo "  all        - Build production binary (default)"
	@echo "  prod       - Build production binary"
	@echo "  test       - Build and run tests with coverage"
	@echo "  check      - Build and run tests without coverage"
	@echo "  debug      - Build tests and launch GDB"
	@echo "  clean      - Remove build artifacts"
	@echo "  install    - Install the application"
	@echo "  uninstall  - Uninstall the application"
	@echo "  dist       - Create distribution package"
	@echo "  release    - Create versioned release package"
	@echo "  help       - Show this help message"

distclean: clean clean-dist
	rm -f $(DEPFILES)

$(OBJDIR)/prod:
	mkdir -p $@

$(OBJDIR)/test:
	mkdir -p $@

$(BINDIR):
	mkdir -p $@

$(PROD_TARGET): $(ALLDIRS) $(PROD_OBJ)
	$(CC) $(PROD_OBJ) -o $@ $(PROD_LDFLAGS)

$(TEST_TARGET): $(ALLDIRS) $(TEST_OBJ)
	$(CC) $(TEST_OBJ) -o $@ $(TEST_LDFLAGS) $(TEST_LIBS)

-include $(DEPFILES)

$(OBJDIR)/prod/%.o: $(SRCDIR)/%.c
	$(CC) -MMD -MP $(PROD_CFLAGS) -c $< -o $@

$(OBJDIR)/test/%.o: $(SRCDIR)/%.c
	$(CC) -MMD -MP $(TEST_CFLAGS) -I$(INCLUDEDIR) -c $< -o $@

$(OBJDIR)/test/%.o: $(TESTDIR)/%.c
	$(CC) -MMD -MP $(TEST_CFLAGS) -I$(INCLUDEDIR) -c $< -o $@

$(TEST_OBJ): | $(OBJDIR)/test

# Ensure the production object directory exists before building prod objects
$(PROD_OBJ): | $(OBJDIR)/prod

# Add header dependencies
$(PROD_OBJ): $(HDRS)
$(TEST_OBJ): $(HDRS)

# Include generated dependency files
-include $(PROD_OBJ:.o=.d)
-include $(TEST_OBJ:.o=.d)

# Generate dependencies
$(OBJDIR)/%.d: $(SRCDIR)/%.c
	@set -e; rm -f $@; \
	$(CC) -MM -I$(INCLUDEDIR) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OBJDIR)/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(OBJDIR)/test_%.d: $(TESTDIR)/%.c
	@set -e; rm -f $@; \
	$(CC) -MM -I$(INCLUDEDIR) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OBJDIR)/test_\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# Ensure dependency files can be generated in obj/prod
$(OBJDIR)/prod/%.d: $(SRCDIR)/%.c | $(OBJDIR)/prod
	@set -e; rm -f $@; \
	$(CC) -MM -I$(INCLUDEDIR) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OBJDIR)/prod/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# Ensure dependency files can be generated in obj/test
$(OBJDIR)/test/%.d: $(TESTDIR)/%.c | $(OBJDIR)/test
	@set -e; rm -f $@; \
	$(CC) -MM -I$(INCLUDEDIR) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OBJDIR)/test/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# Add install target
prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin

.PHONY: install
install: prod
	install -d $(DESTDIR)$(bindir)
	install -m 755 $(PROD_TARGET) $(DESTDIR)$(bindir)

# Add uninstall target
.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(bindir)/$(notdir $(PROD_TARGET))

# Add profile-guided optimization targets
.PHONY: pgo-generate pgo-use

pgo-generate: PROD_CFLAGS += -fprofile-generate
pgo-generate: clean prod
	./$(PROD_TARGET) # Run your program with typical workload

pgo-use: PROD_CFLAGS += -fprofile-use -fprofile-correction
pgo-use: clean prod

# Optimized production build
opt: pgo-generate pgo-use

# Add new target for t4g builds
.PHONY: t4g-release

# Release configuration
VERSION ?= 0.0.2
RELEASE_NAME = web-app-$(VERSION)
BUILDDIR = build
DISTDIR = $(BUILDDIR)/dist
TMPDIR = $(BUILDDIR)/tmp/$(RELEASE_NAME)

# Define required runtime files and their destinations
RUNTIME_FILES = \
	$(BINDIR)/web_server:web_server \
	etc/auth.passwd:etc/auth.passwd \
	var/records/ms1180.rec:var/records/ms1180.rec \
	var/records/obligation_number.txt:var/records/obligation_number.txt \
	var/records/schema.desc:var/records/schema.desc \
	var/records/scjv.rec:var/records/scjv.rec \
	var/records/w6946.rec:var/records/w6946.rec \
	www/audit.html:www/audit.html \
	www/crud_profile.html:www/crud_profile.html

t4g-release: clean-dist prod
	@echo "Creating minimal release package..."
	@rm -rf $(TMPDIR)
	@mkdir -p $(TMPDIR)

	@# Create required directories
	@mkdir -p $(TMPDIR)/etc
	@mkdir -p $(TMPDIR)/var/log
	@mkdir -p $(TMPDIR)/var/records
	@mkdir -p $(TMPDIR)/www

	@# Copy only runtime files
	@for pair in $(RUNTIME_FILES); do \
		src=$$(echo $$pair | cut -d: -f1); \
		dst=$$(echo $$pair | cut -d: -f2); \
		cp $$src $(TMPDIR)/$$dst; \
	done

	@# Create empty log file
	@touch $(TMPDIR)/var/log/audit.log

	@# Set correct permissions
	@find $(TMPDIR) -type d -exec chmod 755 {} \;
	@find $(TMPDIR) -type f -exec chmod 644 {} \;
	@chmod 755 $(TMPDIR)/web_server

	@# Create minimal tarball
	@mkdir -p $(DISTDIR)
	@cd $(BUILDDIR)/tmp && tar czf $(CURDIR)/$(DISTDIR)/$(RELEASE_NAME).tar.gz $(RELEASE_NAME)
	@echo "Minimal release package created: $(DISTDIR)/$(RELEASE_NAME).tar.gz"

clean:
	rm -rf $(BUILDDIR) $(OBJDIR) $(BINDIR)/*.o $(BINDIR)/web_server $(BINDIR)test_web_server

# Add checksum verification target
.PHONY: verify-checksums
verify-checksums:
	./scripts/utils/verify-checksum.sh

# Add checksum generation target
.PHONY: generate-checksums
generate-checksums:
	./scripts/build/generate-checksums.sh

# Add checksum generation to the release target
release: clean-dist prod generate-checksums
	@echo "Creating minimal release package..."
	@rm -rf $(TMPDIR)
	@mkdir -p $(TMPDIR)
	# ...existing code...
	@echo "Minimal release package created: $(DISTDIR)/$(RELEASE_NAME).tar.gz"

reset:
	./scripts/utils/shared-utils.sh reset_build_env

verify-cleanup:
	./scripts/utils/shared-utils.sh verify_cleanup

# Target to generate manual pages
.PHONY: man
man:
	./scripts/build/make-man.sh generate
