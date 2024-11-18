# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: 	AGPL-3.0-or-later

# Variables
ENV_FILE = .env
INI = config.ini
CONF = config.conf
BUILD_DIR = ../build
PREPROCESS_DIR = $(BUILD_DIR)/in
ENV_VARS_MK = $(BUILD_DIR)/env_vars.mk
CONF_VARS_MK = $(BUILD_DIR)/conf_vars.mk
COMB_VARS_MK = $(BUILD_DIR)/combined_vars.mk
SOURCES_FILE = sources.txt
TEST_SOURCES_FILE = test_sources.txt
GCC_SPEC_FILE = gcc.spec
SOURCES_MK = $(BUILD_DIR)/sources.mk
TESTS_MK = $(BUILD_DIR)/tests.mk
GCC_SPECS_MK = $(BUILD_DIR)/gcc_specs.mk

# Create build directory
$(shell mkdir -p $(BUILD_DIR))
$(shell mkdir -p $(PREPROCESS_DIR))

# Always run the targets to update .mk files
.PHONY: all set-env set-conf combine-configs $(ENV_VARS_MK) $(CONF_VARS_MK) $(SOURCES_MK) $(TESTS_MK) $(GCC_SPECS_MK)

# Default target
all: combine-configs $(SOURCES_MK) $(TESTS_MK) $(GCC_SPECS_MK) preprocess

# Read, parse and write key-value pairs from the .env file into the env_vars.mk file
set-env: $(ENV_VARS_MK)

$(ENV_VARS_MK): $(ENV_FILE)
	@echo "Writing key-value pairs from $(ENV_FILE) to $(ENV_VARS_MK)"
	@echo "# Environment variables from .env" > $(ENV_VARS_MK)
	@grep -v '^#' $(ENV_FILE) | grep -v '^$$' | while IFS='=' read -r key value; do \
		key_upper=$$(echo $$key | tr '[:lower:]' '[:upper:]'); \
		echo "$$key_upper := $$value" >> $(ENV_VARS_MK); \
	done

# Read, parse and write key-value pairs from the config.ini and config.conf files into the conf_vars.mk file
set-conf: $(CONF_VARS_MK)

$(CONF_VARS_MK): $(INI) $(CONF)
	@echo "Writing key-value pairs from $(INI) and $(CONF) to $(CONF_VARS_MK)"
	@echo "# Configuration variables from config.ini and config.conf" > $(CONF_VARS_MK)
	@cat $(INI) $(CONF) | grep -v '^#' | grep -v '^$$' | grep -v '^\[' | while IFS='=' read -r key value; do \
		key_upper=$$(echo $$key | tr '[:lower:]' '[:upper:]'); \
		echo "$$key_upper := $$value" >> $(CONF_VARS_MK); \
	done

# Write .env variables first, then add config.ini variables if they don't already exist, and lastly overwrite any existing variables with config.conf variables only if they don't already exist
combine-configs: $(ENV_VARS_MK) $(CONF_VARS_MK)
	@echo "Combining key-value pairs from $(ENV_FILE), $(INI) and $(CONF) into $(COMB_VARS_MK)"
	@echo "# Combined environment and configuration variables" > $(COMB_VARS_MK)
	@cat $(ENV_VARS_MK) >> $(COMB_VARS_MK)
	@cat $(CONF_VARS_MK) | while IFS='=' read -r key value; do \
		key_upper=$$(echo $$key | tr '[:lower:]' '[:upper:]'); \
		sed -i "/^$$key_upper :=/d" $(COMB_VARS_MK); \
		echo "$$key_upper := $$value" >> $(COMB_VARS_MK); \
	done

# Include the combined_vars.mk to load the variables
-include $(COMB_VARS_MK)

# Read, parse and then export $(SOURCES_FILE) into build directory as sources.mk
$(SOURCES_MK): $(SOURCES_FILE)
	@echo "Writing sources from $(SOURCES_FILE) to $(SOURCES_MK)"
	@echo "SOURCES = \\" > $(SOURCES_MK)
	@cat $(SOURCES_FILE) | grep -v '^#' | grep -v '^$$' | while read -r line; do \
		echo "  $$line \\" >> $(SOURCES_MK); \
	done
	@sed -i '$$ s/\\$$//' $(SOURCES_MK) # Remove the trailing backslash from the last line

# For tests: read, parse and then export $(TEST_SOURCES_FILE) into build directory as tests.mk
$(TESTS_MK): $(TEST_SOURCES_FILE)
	@echo "Writing test sources from $(TEST_SOURCES_FILE) to $(TESTS_MK)"
	@echo "TEST_SOURCES = \\" > $(TESTS_MK)
	@cat $(TEST_SOURCES_FILE) | grep -v '^#' | grep -v '^$$' | while read -r line; do \
		echo "  $$line \\" >> $(TESTS_MK); \
	done
	@sed -i '$$ s/\\$$//' $(TESTS_MK) # Remove the trailing backslash from the last line

# Read, parse and then export $(GCC_SPEC_FILE) into build directory as gcc_specs.mk
$(GCC_SPECS_MK): $(GCC_SPEC_FILE)
	@echo "Writing GCC specs from $(GCC_SPEC_FILE) to $(GCC_SPECS_MK)"
	@echo "GCC_SPECS = \\" > $(GCC_SPECS_MK)
	@cat $(GCC_SPEC_FILE) | grep -v '^#' | grep -v '^$$' | while read -r line; do \
		echo "  $$line \\" >> $(GCC_SPECS_MK); \
	done
	@sed -i '$$ s/\\$$//' $(GCC_SPECS_MK) # Remove the trailing backslash from the last line

# Include the generated sources.mk, tests.mk, and gcc_specs.mk
-include $(SOURCES_MK)
-include $(TESTS_MK)
-include $(GCC_SPECS_MK)

# Compiler
CC = gcc

# Preprocessing (Source Code to Preprocessed Code)
#
# Description: The preprocessor expands macros, handles include directives, removes comments, and prepares the source for compilation.
#
# Files:
#
# Input: ..src/source_file.c
# Output: ..build/in/source_file.i
# File Type: .i (Preprocessed source file)
#
# Folders:
#
# Input: ..src/ (C source code files)
# Output: ..build/in (Intermediate files created by preprocessing)
#
# In C program compilation and preprocessing, the .i files are generated as output. These files, with a .i extension, are the preprocessed code and serve as input to the compiler.

# Preprocessing rule
$(PREPROCESS_DIR)/%.i: ../src/%.c
	@echo "Preprocessing $< to $@"
	@gcc -E $< -o $@

# Preprocess all source files
preprocess: $(patsubst ../src/%.c,$(PREPROCESS_DIR)/%.i,$(wildcard src/*.c))
