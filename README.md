# EnvEng-WebApp-Development

Development of the EnvEng web application using 5S methodology for project management and Data-Oriented Programming (DOP) for development.

To combine your directory structure with the **step-by-step breakdown** of the static build process, we'll walk through each step, linking it to your existing directory structure, file types, and the output generated at each stage.

Here’s how your **directory structure** and **build process** integrate at each step of creating a **completely static executable**:

---

### **Step-by-Step Breakdown of the Static Build Process with Directory Integration**

#### **1. Preprocessing (Source Code to Preprocessed Code)**

**Command:**

```bash
gcc -E src/source_file.c -o build/source_file.i
```

- **Description**: The preprocessor expands macros, handles `#include` directives, removes comments, and prepares the source for compilation.
- **Files**:
  - **Input**: `src/source_file.c`
  - **Output**: `build/source_file.i`
  - **File Type**: `.i` (Preprocessed source file)
- **Folders**:
  - **Input**: `src/` (C source code files)
  - **Output**: `build/` (Intermediate files created by preprocessing)
    - Example: `build/config_loader.i`, `build/main.i`

---

#### **2. Compilation (Preprocessed Code to Assembly)**

**Command:**

```bash
gcc -S build/source_file.i -o build/source_file.s
```

- **Description**: This step converts preprocessed code to assembly language, specific to the target architecture.
- **Files**:
  - **Input**: `build/source_file.i` (preprocessed file)
  - **Output**: `build/source_file.s` (assembly code)
  - **File Type**: `.s` (Assembly source code)
- **Folders**:
  - **Input**: `build/` (Preprocessed files)
  - **Output**: `build/` (Assembly code output)
    - Example: `build/config_loader.s`, `build/main.s`

---

#### **3. Assembling (Assembly to Object Code with Dependency Files)**

**Command:**

```bash
gcc -c -MMD -MF deps/source_file.d build/source_file.s -o obj/source_file.o
```

- **Description**: The assembler converts the assembly code into machine code (object code), but without any linking yet. Additionally, it generates dependency files to track header dependencies.
- **Files**:
  - **Input**: `build/source_file.s` (assembly file)
  - **Output**: `obj/source_file.o` (object file), `deps/source_file.d` (dependency file)
  - **File Type**: `.o` (Object file, machine code), `.d` (Dependency file)
- **Folders**:
  - **Input**: `build/` (Assembly files)
  - **Output**: `obj/` (Object files), `deps/` (Dependency files)
    - Example: `obj/config_loader.o`, `obj/main.o`, `deps/config_loader.d`, `deps/main.d`

---

#### **4. Static Linking (Object Files to Executable)**

**Command:**

```bash
gcc -static -o bin/executable_name obj/*.o
```

- **Description**: Links object files into a fully static executable. Ensures that no dynamic/shared libraries are needed at runtime.
- **Files**:
  - **Input**: `obj/*.o` (object files)
  - **Output**: `bin/executable_name` (final static executable)
  - **File Type**: No extension or `.out` (final executable)
- **Folders**:
  - **Input**: `obj/` (Object files)
  - **Output**: `bin/` (Final executable)
    - Example: `bin/executable_name`

---

#### **5. Verify Static Linking (Check for Dynamic Dependencies)**

**Command:**

```bash
ldd bin/executable_name
```

- **Description**: This verifies that the final executable has no dependencies on shared libraries. The output should show "not a dynamic executable" if it is fully static.
- **Files**:
  - **Input**: `bin/executable_name` (final executable)
  - **Output**: Output from `ldd`, confirming whether the executable has dynamic dependencies.
- **Folders**:
  - **No output files** are generated, but the confirmation appears in the terminal.

---

#### **6. Add Debugging Symbols (Optional)**

**Command:**

```bash
gcc -static -g -o debug/executable_name source_file.c
```

- **Description**: Adds debugging symbols to the executable, which helps during debugging (e.g., with `gdb`).
- **Files**:
  - **Input**: `src/source_file.c` (C source file)
  - **Output**: `debug/executable_name` (Debuggable executable with debugging symbols)
  - **File Type**: `.out` or no extension (executable file with debug symbols)
- **Folders**:
  - **Input**: `src/` (C source files)
  - **Output**: `debug/` (Debuggable executable)
    - Example: `debug/executable_name` with debug symbols

---

#### **7. Optimizing the Executable (Optional)**

**Command:**

```bash
gcc -static -O3 -o bin/executable_name source_file.c
```

- **Description**: This step uses optimization flags (e.g., `-O3`) to optimize the executable for performance, potentially making it faster but larger.
- **Files**:
  - **Input**: `src/source_file.c` (C source file)
  - **Output**: `bin/executable_name` (Optimized executable)
  - **File Type**: `.out` or no extension (optimized executable)
- **Folders**:
  - **Input**: `src/` (C source files)
  - **Output**: `bin/` (Optimized executable)
    - Example: `bin/executable_name`

---

#### **8. Cross-compiling (Optional)**

If you are cross-compiling for a different architecture (e.g., ARM, embedded systems), you would use a cross-compiler.

**Command (for ARM):**

```bash
arm-linux-gnueabihf-gcc -static -o bin/executable_name source_file.c
```

- **Description**: Compiles the program for a different architecture (e.g., ARM), ensuring that the executable is still statically linked.
- **Files**:
  - **Input**: `src/source_file.c` (C source file)
  - **Output**: `bin/executable_name` (Cross-compiled executable for a different architecture)
  - **File Type**: `.out` or no extension (cross-compiled executable)
- **Folders**:
  - **Input**: `src/` (C source files)
  - **Output**: `bin/` (Cross-compiled executable)
    - Example: `bin/executable_name` (ARM architecture)

---

#### **9. Final Verification (Ensure Self-Containment)**

**Command:**

```bash
ldd bin/executable_name
```

- **Description**: Verifies once more that there are no external dynamic dependencies. The result should indicate "not a dynamic executable," confirming that it is fully static.
- **Files**:
  - **Input**: `bin/executable_name` (final executable)
  - **Output**: Confirmation from `ldd` that the executable is statically linked.
- **Folders**:
  - **No output files** generated; this step confirms the static nature of the final executable.

---

### **Final Directory Structure After Full Build**

Here’s what the directory structure would look like, incorporating each of the build steps above:

```plaintext
web-app/
├── bin/                # Final executables
│   ├── executable_name   # Fully static executable
│   └── (optional) debug/executable_name  # Debuggable or optimized executable
├── build/              # Intermediate build files
│   ├── config_loader.i   # Preprocessed file
│   ├── main.i            # Preprocessed file
│   ├── config_loader.s   # Assembly file
│   └── main.s            # Assembly file
├── config/             # Configuration files
│   ├── config.conf     # App config
│   ├── config.ini      # App config
│   ├── gcc.spec        # GCC build config
│   ├── sources.txt     # List of source files
│   └── test_sources.txt# List of test files
├── deps/               # Dependencies (empty in your case)
├── include/            # Header files
│   ├── config.h        # Header files
│   ├── error_handler.h # Header files
│   ├── garbage_collector.h # Header files
│   └── logger.h        # Header files
├── lib/                # Static libraries (empty in your case)
├── logs/               # Build/runtime logs
├── obj/                # Object files
│   ├── config_loader.o # Object file (from src/config_loader.c)
│   └── main.o          # Object file (from src/main.c)
├── package.json        # NodeJS package (not relevant for static build)
├── package-lock.json   # NodeJS lockfile (not relevant for static build)
├── README.md           # Project documentation
├── src/                # Source code files
│   ├── config_loader.c  # C file for config loader
│   ├── main.c           # Main entry point
│   └── hello.c          # Another source file
├── tests/              # Test files
│   ├── test_config_loader.c # Test file for config_loader
│   ├── test_main.c

				# Test file for main
│   └── test_hello.c       # Test file for hello
└── tmp/                # Temporary files (intermediate)
```

By following this process, you ensure that your project is properly built as a static executable with all necessary intermediate files and configurations organized into the appropriate folders.

```Makefile
# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: 	AGPL-3.0-or-later

# Variables
ENV_FILE = .env
INI = config.ini
CONF = config.conf
BUILD_DIR = ../build
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

# Always run the targets to update .mk files
.PHONY: all set-env set-conf combine-configs $(ENV_VARS_MK) $(CONF_VARS_MK) $(SOURCES_MK) $(TESTS_MK) $(GCC_SPECS_MK)

# Default target
all: combine-configs $(SOURCES_MK) $(TESTS_MK) $(GCC_SPECS_MK)

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

# Use these flags in the build
CFLAGS += $(CFLAGS_SPEC) -I$(INCLUDE_DIR) -Wall -Wextra -std=c99 -g
LDFLAGS += $(LDFLAGS_SPEC) -static

# Load sources dynamically
SOURCES = $(patsubst %, $(SRC_DIR)/%, $(shell cat $(SOURCES_FILE)))
TEST_SOURCES = $(patsubst %, $(TESTS_DIR)/%, $(shell cat $(TEST_SOURCES_FILE)))
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
TEST_OBJECTS = $(patsubst $(TESTS_DIR)/%.c, $(OBJ_DIR)/%.o, $(TEST_SOURCES))
DEPENDENCIES = $(patsubst $(OBJ_DIR)/%.o, $(DEPS_DIR)/%.d, $(OBJECTS) $(TEST_OBJECTS))

# Compiler
CC = gcc

# Build main application
all: $(BIN_DIR)/$(APP_NAME)

$(BIN_DIR)/$(APP_NAME): $(OBJECTS)
	mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ -o $@

# Build individual object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR) $(DEPS_DIR)
	$(CC) $(CFLAGS) -MMD -MF $(patsubst $(OBJ_DIR)/%.o, $(DEPS_DIR)/%.d, $@) -c $< -o $@

# Build and run tests
test: $(TEST_OBJECTS)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $(BIN_DIR)/test_runner
	@echo "Running tests..."
	$(BIN_DIR)/test_runner

# Build individual test object files
$(OBJ_DIR)/%.o: $(TESTS_DIR)/%.c
	mkdir -p $(OBJ_DIR) $(DEPS_DIR)
	$(CC) $(CFLAGS) -MMD -MF $(patsubst $(OBJ_DIR)/%.o, $(DEPS_DIR)/%.d, $@) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR) $(BIN_DIR) $(DEPS_DIR) $(DEBUG_DIR) $(TMP_DIR)

# Include dependencies
-include $(DEPENDENCIES)
```
