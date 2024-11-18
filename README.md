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

