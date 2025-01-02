$ git status -v
On branch 96-2-improve-record-creation-and-editing-functionality
Your branch is up to date with 'origin/96-2-improve-record-creation-and-editing-functionality'.

Changes to be committed:
  (use "git restore --staged <file>..." to unstage)
        new file:   .devcontainer/issue.md
        modified:   .github/copilot-instructions.md
        new file:   .shellcheckrc
        modified:   .vscode/settings.json
        deleted:    config/build/args.h
        deleted:    config/build/build.h
        modified:   config/build/config.h
        new file:   config/build/gcc.specs
        modified:   config/build/host.h
        modified:   config/build/machine.h
        modified:   config/deps.txt
        new file:   scripts/build/check-deps.sh
        new file:   scripts/build/setup-env.sh
        modified:   web-app.code-workspace

diff --git a/.devcontainer/issue.md b/.devcontainer/issue.md
new file mode 100644
index 0000000..84565d5
--- /dev/null
+++ b/.devcontainer/issue.md
@@ -0,0 +1,80 @@
+<!-- filepath: /home/appuser/web-app/.devcontainer/issue.md -->
+# Build System Setup and Documentation
+
+## Description
+Set up complete build system infrastructure and documentation for the web application project. This includes configuration scripts, build tools, and comprehensive documentation.
+
+## Build System Tasks
+
+### Configuration Scripts
+- [X] Verify/create configuration files:
+  - [X] scripts/build/check-deps.sh
+  - [X] build/setup-env.sh
+  - [X] config/build/host.h
+  - [X] config/build/config.h
+  - [X] config/build/machine.h
+  - [X] config/build/gcc.specs
+
+### Core Build Scripts
+- [ ] Implement main build scripts:
+  - [ ] configure (generates config.h from config.h.in with posix configure script, that sets build options)
+  - [ ] build.sh (main build orchestration)
+  - [ ] make-debug.sh (debug build with symbols)
+  - [ ] make-release.sh (optimized release build)
+  - [ ] make-dist.sh (distribution packaging)
+  - [ ] install.sh (installation script)
+  - [ ] clean.sh (cleanup script)
+  - [ ] test.sh (test runner)
+  - [ ] mkconfig.sh (config generation)
+
+### Build Verification
+- [ ] Implement checksum verification:
+  - [ ] release.sha256
+  - [ ] libs.sha256
+
+### CI/IDE Integration
+- [ ] Configure build automation:
+  - [ ] .github/workflows/build.yml
+  - [ ] .vscode/tasks.json
+
+## Documentation Tasks
+
+### Developer Documentation
+- [ ] Create/update build documentation:
+  - [ ] /docs/dev/BUILD.md
+  - [ ] /docs/dev/INSTALL.md
+  - [ ] /docs/dev/TOOLCHAIN.md
+  - [ ] /docs/dev/DEBUG.md
+  - [ ] /docs/dev/RELEASE.md
+  - [ ] /docs/dev/TESTING.md
+  - [ ] CONTRIBUTING.md
+
+### Man Pages
+- [ ] Create man pages:
+  - [ ] /man/man1/build.1
+
+### Quick Start Guide
+- [ ] Update README.md with:
+  - [ ] Build quick start
+  - [ ] Tool requirements
+  - [ ] Documentation links
+
+## Additional Notes
+- All scripts must be POSIX-compliant
+- Build system must work with musl-gcc
+- Documentation should follow project standards
+- Test coverage must be comprehensive
+
+## Related Issues
+- None
+
+## Dependencies
+- musl-gcc toolchain
+- GNU Make
+- CUnit testing framework
+- BearSSL
+- Valgrind
+
+/label build-system
+/label documentation
+/milestone v1.0
diff --git a/.github/copilot-instructions.md b/.github/copilot-instructions.md
index 897c684..e9ba017 100644
--- a/.github/copilot-instructions.md
+++ b/.github/copilot-instructions.md
@@ -903,9 +903,7 @@ ### **File Tree**
         csp.conf            # Content Security Policy
     build/                  # Build configurations
         host.h              # Host-specific config
-        build.h             # Build environment
         config.h            # Main config header
-        args.h              # Build arguments
         machine.h           # Machine settings
         gcc.specs           # GCC spec file for build configuration
     mkconfig/               # Config generation
diff --git a/.shellcheckrc b/.shellcheckrc
new file mode 100644
index 0000000..0d14c58
--- /dev/null
+++ b/.shellcheckrc
@@ -0,0 +1,53 @@
+# Set shell to POSIX sh for maximum portability
+shell=sh
+
+# Enable core POSIX checks
+enable=all
+
+# Disable non-POSIX features and extensions
+disable=SC2039  # POSIX-only shell features
+disable=SC2310  # Non-POSIX test commands
+disable=SC2311  # Non-POSIX test operators
+
+# Source handling configuration
+source-path=SCRIPTDIR
+external-sources=true
+
+# Set error handling level
+error-level=error
+severity=error
+
+# Critical safety checks
+enable=check-unassigned-uppercase
+enable=quote-safe-variables
+enable=require-variable-braces
+enable=check-set-e-suppressed
+enable=check-external-commands
+enable=require-function-declaration
+enable=check-unquoted-tilde
+enable=check-command-substitution-quoting
+enable=check-return-code
+enable=check-set-options
+enable=check-command-path
+
+# Variable and parameter handling
+enable=check-parameter-expansion
+enable=check-environment-variables
+enable=require-variable-declaration
+enable=check-word-splitting
+
+# File and directory operations
+enable=check-symlink-race
+enable=check-tempfile-race
+enable=check-cd-errors
+enable=check-pathname-expansion
+
+# Process and signal handling
+enable=check-signal-handler
+enable=check-trap-handler
+enable=check-job-control
+
+# Input/Output handling
+enable=check-redirect-printf
+enable=check-fd-leaked
+enable=check-fd-closed
diff --git a/.vscode/settings.json b/.vscode/settings.json
index e69de29..6edfa18 100644
--- a/.vscode/settings.json
+++ b/.vscode/settings.json
@@ -0,0 +1,25 @@
+{
+    "shellcheck.enable": true,
+    "shellcheck.enableQuickFix": true,
+    "shellcheck.run": "onType",
+    "shellcheck.executablePath": "/usr/bin/shellcheck",
+    "[shellcheckrc]": {
+      "files.encoding": "iso88591",
+      "files.autoGuessEncoding": false,
+      "files.trimTrailingWhitespace": true
+    },
+    "[shellcheck-output]": {
+      "editor.wordWrap": "on",
+      "editor.trimAutoWhitespace": false,
+      "editor.defaultFormatter": "timonwong.shellcheck",
+      "files.encoding": "iso88591",
+      "files.autoGuessEncoding": false,
+      "files.trimTrailingWhitespace": false
+    },
+    "[shellscript]": {
+      "files.eol": "\n",
+      "editor.defaultColorDecorators": "auto",
+    },
+    "shellcheck.disableVersionCheck": true,
+    "shellcheck.useWorkspaceRootAsCwd": false
+}
diff --git a/config/build/args.h b/config/build/args.h
deleted file mode 100644
index e69de29..0000000
diff --git a/config/build/build.h b/config/build/build.h
deleted file mode 100644
index e69de29..0000000
diff --git a/config/build/config.h b/config/build/config.h
index e69de29..609a0f2 100644
--- a/config/build/config.h
+++ b/config/build/config.h
@@ -0,0 +1,65 @@
+#ifndef CONFIG_H
+#define CONFIG_H
+
+/* Language standard and POSIX compliance flags */
+#define LANG_FLAGS "-std=c90 -ansi -pedantic" \
+                  " -D_POSIX_C_SOURCE=200809L" \
+                  " -D_XOPEN_SOURCE=500" \
+                  " -D_FILE_OFFSET_BITS=64"
+
+/* Warning and error detection flags */
+#define WARN_FLAGS "-Wall -Wextra -Werror" \
+                  " -Wshadow -Wconversion" \
+                  " -Wstrict-prototypes -Wmissing-prototypes" \
+                  " -Warray-bounds -Wformat=2" \
+                  " -Wformat-security -Wformat-overflow=2" \
+                  " -Wformat-truncation=2 -Wvla" \
+                  " -Wstrict-aliasing=2 -Wnull-dereference" \
+                  " -Wpointer-arith -Wwrite-strings" \
+                  " -Wcast-align -Wcast-qual" \
+                  " -Wredundant-decls -Wundef" \
+                  " -Wmissing-declarations -Wsign-conversion"
+
+/* Security hardening flags */
+#define SEC_FLAGS "-fstack-protector-strong" \
+                 " -fstack-check -fPIE" \
+                 " -fstack-protector-all" \
+                 " -fdata-sections -ffunction-sections" \
+                 " -fno-common -fstrict-aliasing" \
+                 " -finput-charset=iso-8859-1" \
+                 " -fexec-charset=iso-8859-1" \
+                 " -fwide-exec-charset=iso-8859-1"
+
+/* Optimization flags */
+#define OPT_FLAGS "-O3" \
+                 " -fdata-sections" \
+                 " -ffunction-sections" \
+                 " -fno-strict-overflow" \
+                 " -flto=auto"
+
+/* CPU-Specific Optimization Flags */
+#define ARCH_FLAGS "-march=native -mtune=native -msse4.2 -mavx2 -mfma -mavx512f -mpclmul -maes -msha -mprefer-vector-width=512"
+
+/* Cache Optimization Flags */
+#define CACHE_FLAGS "-falign-functions=64 -falign-jumps=32 -falign-loops=32 -falign-labels=32"
+
+/* Network Performance Flags */
+#define NET_FLAGS "-fno-schedule-insns -fno-schedule-insns2 -fomit-frame-pointer -fno-stack-check"
+
+/* Build configuration structure */
+struct build_config {
+    const char *prod_cflags;   /* Production compiler flags */
+    const char *prod_ldflags;  /* Production linker flags */
+    const char *test_cflags;   /* Test build compiler flags */
+    const char *test_ldflags;  /* Test build linker flags */
+};
+
+/* Default build configuration */
+#define BUILD_CONFIG_DEFAULT { \
+    .prod_cflags = LANG_FLAGS " " WARN_FLAGS " " SEC_FLAGS " " OPT_FLAGS " " ARCH_FLAGS " " CACHE_FLAGS " " NET_FLAGS " -static", \
+    .prod_ldflags = "-static -Wl,--gc-sections -Wl,-O3", \
+    .test_cflags = LANG_FLAGS " " WARN_FLAGS " " SEC_FLAGS " -O0 -g3", \
+    .test_ldflags = "-Wl,--warn-common -Wl,--no-undefined" \
+}
+
+#endif /* CONFIG_H */
diff --git a/config/build/gcc.specs b/config/build/gcc.specs
new file mode 100644
index 0000000..508be8c
--- /dev/null
+++ b/config/build/gcc.specs
@@ -0,0 +1,45 @@
+# Core Language and Standards Flags
+LANG_FLAGS = -std=c90 -ansi -pedantic -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=500 -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FORTIFY_SOURCE=2 -DVERSION=\"$(VERSION)\"
+
+# Warning Flags
+WARN_FLAGS = -Wall -Wextra -Werror -Wshadow -Wconversion -Wstrict-prototypes -Wmissing-prototypes -Warray-bounds -Wformat=2 -Wformat-security -Wformat-overflow=2 -Wformat-truncation=2 -Wvla -Wbad-function-cast -Wstrict-aliasing=3 -Wnull-dereference -Wdouble-promotion -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wcast-align=strict -Wcast-qual -Wredundant-decls -Wundef -Wmissing-include-dirs -Winit-self -Wswitch-enum -Wmissing-declarations -Wsign-conversion -Wmissing-noreturn -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wunsafe-loop-optimizations -Wstack-usage=8192 -Wpadded -Waggregate-return -Wattributes -Wbuiltin-macro-redefined -Wdeprecated -Wdiv-by-zero -Wendif-labels -Wexpansion-to-defined -Wformat-contains-nul -Wformat-extra-args -Wformat-zero-length -Wmultichar -Wnested-externs -Wno-format-nonliteral -Wno-unused-result -Woverflow -Wpointer-to-int-cast -Wpragmas -Wredundant-decls -Wvariadic-macros -Wvla-larger-than=4096 -Wwrite-strings -Wstack-protector
+
+# Security Flags
+SEC_FLAGS = -fstack-protector-strong -fstack-clash-protection -fstack-check -fPIE -fstack-protector-all -fno-delete-null-pointer-checks -ftrivial-auto-var-init=zero -fno-allow-store-data-races -fwrapv -fno-strict-volatile-bitfields -finput-charset=iso-8859-1 -fexec-charset=iso-8859-1 -fwide-exec-charset=iso-8859-1 -fstack-protector
+
+# CPU-Specific Optimization Flags
+ARCH_FLAGS = -march=native -mtune=native -msse4.2 -mavx2 -mfma -mavx512f -mpclmul -maes -msha -mprefer-vector-width=512
+
+# Cache Optimization Flags
+CACHE_FLAGS = -falign-functions=64 -falign-jumps=32 -falign-loops=32 -falign-labels=32
+
+# Network Performance Flags
+NET_FLAGS = -fno-schedule-insns -fno-schedule-insns2 -fomit-frame-pointer -fno-stack-check
+
+# Enhanced Optimization Flags
+OPT_FLAGS = -O3 -fdata-sections -ffunction-sections -fno-common -fstrict-aliasing -fno-strict-overflow -fno-plt -fPIC -fno-semantic-interposition -flto=auto -fno-fat-lto-objects -fdevirtualize-at-ltrans -fipa-pta -floop-nest-optimize -floop-parallelize-all -ftree-parallelize-loops=4 -ftree-vectorize -funroll-loops -ffast-math -fmerge-all-constants -fmodulo-sched -fmodulo-sched-allow-regmoves -fgcse-sm -fgcse-las -fipa-cp-clone -fira-loop-pressure -fivopts -flifetime-dse=2 -flive-range-shrinkage -fmath-errno -fpeephole2 -freciprocal-math -fsched-pressure -fsched-spec-load -fsched2-use-superblocks -ftree-loop-distribution -ftree-loop-im -ftree-loop-ivcanon -ftree-vectorize -fuse-linker-plugin -fwhole-program -falign-functions=32 -falign-jumps=32 -falign-loops=32 -falign-labels=32 -fcaller-saves -fcrossjumping -fcse-follow-jumps -fcse-skip-blocks -fdelete-null-pointer-checks -fdevirtualize -fexpensive-optimizations -ffinite-loops -fgcse-after-reload -fhoist-adjacent-loads -findirect-inlining -finline-functions -finline-small-functions -fipa-bit-cp -fipa-icf -fipa-icf-functions -fipa-icf-variables -fipa-profile -fipa-pure-const -fipa-ra -fipa-reference -fipa-reference-addressable -fipa-sra -fira-hoist-pressure -fisolate-erroneous-paths-dereference -flra-remat -foptimize-sibling-calls -fpartial-inlining -fpeel-loops -fpredictive-commoning -free -frename-registers -freorder-blocks -freorder-blocks-algorithm=stc -freorder-functions -frerun-cse-after-loop -fschedule-fusion -freschedule-modulo-scheduled-loops -fsplit-ivs-in-unroller -fssa-phiopt -fstore-merging -fstrict-aliasing -fthread-jumps -ftree-bit-ccp -ftree-builtin-call-dce -ftree-copy-prop -ftree-dce -ftree-dominator-opts -ftree-dse -ftree-forwprop -ftree-fre -ftree-loop-if-convert -ftree-loop-im -ftree-loop-ivcanon -ftree-loop-optimize -ftree-parallelize-loops=4 -ftree-pre -ftree-pta -ftree-reassoc -ftree-sink -ftree-slsr -ftree-sra -ftree-switch-conversion -ftree-tail-merge -ftree-ter -ftree-vrp -funit-at-a-time -funswitch-loops -fvariable-expansion-in-unroller -fvect-cost-model=dynamic -fversion-loops-for-strides $(ARCH_FLAGS)
+
+# Combined Production Flags
+PROD_CFLAGS = $(LANG_FLAGS) $(WARN_FLAGS) $(SEC_FLAGS) $(OPT_FLAGS) -static
+
+# Enhanced Linker Flags for Performance
+PROD_LDFLAGS = -static -Wl,--gc-sections,-O3,--sort-common -Wl,--build-id=none,-z,relro,-z,now,-z,noexecstack -Wl,--sort-section=alignment,--strip-all,--warn-common -Wl,--no-undefined,--no-allow-shlib-undefined -Wl,--no-copy-dt-needed-entries,--as-needed -Wl,-z,separate-code,-z,text,-z,defs -Wl,-z,relro,-z,global -Wl,-z,noexecstack,-z,nocombreloc -Wl,--eh-frame-hdr,--exclude-libs,ALL -Wl,--sort-section=alignment -Wl,--relax -Wl,-O3 -lpthread -lcrypt -lrt -ldl
+
+# Test & Debug Configuration Flags
+TEST_BASIC = -std=c90 -ansi -pedantic -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=500 -Wall -Wextra -Werror -O0
+
+# Debug Symbol Flags for GDB
+TEST_DEBUG = -g3 -ggdb3 -gdwarf-4 -fno-eliminate-unused-debug-types -fno-omit-frame-pointer -fno-inline
+
+# Coverage Flags
+TEST_COVERAGE = -fprofile-arcs -ftest-coverage -fprofile-generate -fvpt -funroll-loops
+
+# Refined Analysis Flags
+TEST_ANALYSIS = -fanalyzer -fno-omit-frame-pointer -fstack-check -fstack-protector-all -ftrapv
+
+# Combined Test Flags
+TEST_CFLAGS = $(TEST_BASIC) $(TEST_DEBUG) $(TEST_COVERAGE) $(filter-out -O3,$(TEST_ANALYSIS))
+
+# Test Libraries and Linker Flags
+TEST_LIBS = -lgcov -lcunit -lcrypt -lpthread -lrt
+TEST_LDFLAGS = -Wl,--warn-common -Wl,--no-undefined -Wl,--gc-sections -Wl,--build-id -Wl,--eh-frame-hdr $(TEST_LIBS)
diff --git a/config/build/host.h b/config/build/host.h
index e69de29..f4b85c1 100644
--- a/config/build/host.h
+++ b/config/build/host.h
@@ -0,0 +1,16 @@
+#ifndef HOST_H
+#define HOST_H
+
+/* Check if musl libc is being used */
+#define LIBC_CHECK "$(shell ldd --version 2>&1 | grep -i musl)"
+
+/* Default compiler */
+#define CC "gcc"
+
+/* Use musl-gcc if musl libc is detected */
+#ifdef LIBC_CHECK
+#undef CC
+#define CC "musl-gcc"
+#endif
+
+#endif /* HOST_H */
diff --git a/config/build/machine.h b/config/build/machine.h
index e69de29..891152a 100644
--- a/config/build/machine.h
+++ b/config/build/machine.h
@@ -0,0 +1,22 @@
+#ifndef MACHINE_H
+#define MACHINE_H
+
+#define ARCH_FLAGS "-march=native -mtune=native -msse4.2 -mavx2 -mfma -mavx512f -mpclmul -maes -msha -mprefer-vector-width=512"
+#define CACHE_FLAGS "-falign-functions=64 -falign-jumps=32 -falign-loops=32 -falign-labels=32"
+#define NET_FLAGS "-fno-schedule-insns -fno-schedule-insns2 -fomit-frame-pointer -fno-stack-check"
+
+#endif /* MACHINE_H */#ifndef MACHINE_H
+#define MACHINE_H
+
+/* CPU-specific optimization flags */
+#define ARCH_FLAGS "-march=native -mtune=native -msse4.2 -mavx2 -mfma -mavx512f -mpclmul -maes -msha -mprefer-vector-width=512"
+
+/* Cache optimization flags */
+#define CACHE_FLAGS "-falign-functions=64 -falign-jumps=32 -falign-loops=32 -falign-labels=32"
+
+/* Network performance flags */
+#define NET_FLAGS "-fno-schedule-insns -fno-schedule-insns2 -fomit-frame-pointer -fno-stack-check"
+
+#endif /* MACHINE_H */#define ARCH_FLAGS "-march=native -mtune=native -msse4.2 -mavx2 -mfma -mavx512f -mpclmul -maes -msha -mprefer-vector-width=512"
+#define CACHE_FLAGS "-falign-functions=64 -falign-jumps=32 -falign-loops=32 -falign-labels=32"
+#define NET_FLAGS "-fno-schedule-insns -fno-schedule-insns2 -fomit-frame-pointer -fno-stack-check"
diff --git a/config/deps.txt b/config/deps.txt
index 8490ed5..e0575af 100644
--- a/config/deps.txt
+++ b/config/deps.txt
@@ -1,63 +1,33 @@
 # Core Build Tools (build-base contents)
-binutils           # GNU assembler, linker and binary utilities
-gcc                # GNU C Compiler (already provides cc)
-make               # GNU Make build tool
-libc-dev          # C library headers and static libraries
-musl-dev          # musl libc development files
-fortify-headers   # Fortify source headers
-patch             # Apply patches to source files
-
-# Build System Tools
-autoconf           # Generate configure scripts
-automake           # Generate Makefiles
-libtool            # Shared library tools
-pkgconfig          # Manage library compile/link flags
-
-# Parser/Lexer Tools
-flex               # Fast Lexical Analyzer
-bison              # YACC-compatible Parser Generator
+binutils >= 2.40.0    # GNU assembler, linker and binary utilities
+gcc >= 12.2.1         # GNU C Compiler (already provides cc)
+make >= 4.4.1         # GNU Make build tool
 
 # Version Control
-git                # Distributed version control
+git >= 2.40.0         # Distributed version control
 
 # Core Libraries
-musl               # C standard library implementation (Alpine's libc)
-recutils           # Text-based database utilities
+recutils >= 1.9       # Text-based database utilities
 
 # Development Tools
-gdb                # GNU Debugger
-git                # Version control
-valgrind           # Memory analysis
-shellcheck         # Shell script validator
-dropbear           # SSH client and server
-gpg                # GnuPG
-
-# Testing Tools
-cunit              # Unit testing framework
-gcov               # Code coverage
-gprof              # Performance profiler
-posixtestsuite     # POSIX compliance testing
+gdb >= 13.1          # GNU Debugger
+valgrind >= 3.21.0   # Memory analysis
+shellcheck >= 0.9.0   # Shell script validator
 
 # Code Analysis
-splint             # Static code analyzer
-cppcheck           # Static analysis tool
-checksec           # Binary security checker
+cppcheck >= 2.10.0    # Static analysis tool
 
 # Documentation
-doxygen            # Code documentation generator
-groff              # Manual page processor
+doxygen >= 1.9.6      # Code documentation generator
+groff >= 1.22.4       # Manual page processor
 
 # Character Encoding
-enca               # Character encoding validator
+enca >= 1.19          # Character encoding validator
 
 # Security
-bearssl            # SSL/TLS and crypto library
+bearssl >= 0.6        # SSL/TLS and crypto library
 
 # Environment Requirements
 LANG=en_AU.ISO8859-1
 LC_ALL=en_AU.ISO8859-1
 CHARSET=ISO-8859-1
-
-# Compiler Flags
-
-# Build Configuration
diff --git a/scripts/build/check-deps.sh b/scripts/build/check-deps.sh
new file mode 100755
index 0000000..57dafab
--- /dev/null
+++ b/scripts/build/check-deps.sh
@@ -0,0 +1,365 @@
+#!/bin/sh
+
+#######################################
+# Check development dependencies
+#
+# Usage: ./check-deps.sh [--help]
+#
+# Options:
+#   --help    Display help message
+#######################################
+
+# Strict mode
+set -e
+
+#######################################
+# Constants
+#######################################
+
+# Environment setup
+LANG=en_AU.ISO8859-1
+LC_ALL=en_AU.ISO8859-1
+CHARSET=ISO-8859-1
+export LANG LC_ALL CHARSET
+
+# Exit codes
+EXIT_SUCCESS=0
+EXIT_FAILURE=1
+
+# Configuration file path
+DEPS_FILE="config/deps.txt"
+
+# Required minimum versions
+MIN_BINUTILS="2.40.0"
+MIN_GCC="12.2.1"
+MIN_GIT="2.40.0"
+MIN_VALGRIND="3.21.0"
+MIN_SHELLCHECK="0.9.0"
+MIN_CPPCHECK="2.10.0"
+MIN_ENCA="1.19"
+MIN_DOXYGEN="1.9.6"
+MIN_GROFF="1.22.4"
+MIN_BEARSSL="0.6"
+MIN_MAKE="4.4.1"
+MIN_GDB="13.1"
+MIN_RECUTILS="1.9"
+
+# Known tools
+KNOWN_TOOLS="gcc git valgrind shellcheck cppcheck enca doxygen groff bearssl make gdb recsel rec2csv recdel recfix recfmt recinf recins recset ar as ld nm objcopy objdump ranlib strip"
+
+#######################################
+# Functions
+#######################################
+
+# Print error message to stderr
+log_error() {
+    printf "ERROR: %s\n" "${1}" >&2
+}
+
+# Print info message to stdout
+log_info() {
+    printf "INFO: %s\n" "${1}"
+}
+
+# Add global variables for environment requirements
+REQUIRED_LANG="en_AU.ISO8859-1"
+REQUIRED_LC_ALL="en_AU.ISO8859-1"
+REQUIRED_CHARSET="ISO-8859-1"
+
+# Update parse_deps_file function
+parse_deps_file() {
+    if [ ! -f "${DEPS_FILE}" ]; then
+        log_error "Dependencies file not found: ${DEPS_FILE}"
+        return "${EXIT_FAILURE}"
+    fi
+
+    while IFS= read -r line; do
+        # Skip comments and empty lines
+        if [ -z "${line}" ] || [ "${line#\#}" != "${line}" ]; then
+            continue
+        fi
+
+        case "${line}" in
+            "LANG="*)
+                REQUIRED_LANG="${line#LANG=}"
+                ;;
+            "LC_ALL="*)
+                REQUIRED_LC_ALL="${line#LC_ALL=}"
+                ;;
+            "CHARSET="*)
+                REQUIRED_CHARSET="${line#CHARSET=}"
+                ;;
+            *"binutils >= "*)     MIN_BINUTILS=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
+            *"gcc >= "*)          MIN_GCC=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
+            *"git >= "*)          MIN_GIT=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
+            *"valgrind >= "*)     MIN_VALGRIND=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
+            *"shellcheck >= "*)   MIN_SHELLCHECK=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
+            *"cppcheck >= "*)     MIN_CPPCHECK=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
+            *"enca >= "*)         MIN_ENCA=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
+            *"doxygen >= "*)      MIN_DOXYGEN=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
+            *"groff >= "*)        MIN_GROFF=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
+            *"bearssl >= "*)      MIN_BEARSSL=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
+            *"make >= "*)         MIN_MAKE=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
+            *"gdb >= "*)          MIN_GDB=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
+            *"recutils >= "*)     MIN_RECUTILS=$(echo "${line}" | awk -F '>= ' '{print $2}' | awk '{print $1}') ;;
+            *)
+                # Only log error for non-environment variables
+                if ! echo "${line}" | grep -q "^[A-Z_]*="; then
+                    tool=$(echo "${line}" | awk '{print $1}')
+                    if echo "${KNOWN_TOOLS}" | grep -qw "${tool}"; then
+                        log_error "Unknown dependency: ${line}"
+                        return "${EXIT_FAILURE}"
+                    fi
+                fi
+                ;;
+        esac
+    done < "${DEPS_FILE}"
+
+    return "${EXIT_SUCCESS}"
+}
+
+# Add version parsing functions
+get_valgrind_version() {
+    valgrind --version | awk '{print $2}'
+}
+
+get_shellcheck_version() {
+    shellcheck --version | awk '/version:/ {print $2}'
+}
+
+get_cppcheck_version() {
+    cppcheck --version | awk '{print $2}'
+}
+
+get_enca_version() {
+    enca --version | awk '/Version/ {print $2}'
+}
+
+get_doxygen_version() {
+    doxygen --version
+}
+
+get_groff_version() {
+    groff --version | awk 'NR==1 {print $4}'
+}
+
+get_make_version() {
+    make --version | awk 'NR==1 {print $3}'
+}
+
+get_gdb_version() {
+    gdb --version | awk 'NR==1 {print $4}'
+}
+
+get_recutils_version() {
+    recsel --version | awk 'NR==1 {print $4}'
+}
+
+check_versions() {
+    # Check binutils version
+    if ! command -v ar >/dev/null 2>&1 || \
+       ! command -v as >/dev/null 2>&1 || \
+       ! command -v ld >/dev/null 2>&1; then
+        log_error "binutils >= ${MIN_BINUTILS} is required"
+        return "${EXIT_FAILURE}"
+    fi
+
+    # Check gcc version
+    if ! command -v gcc >/dev/null 2>&1; then
+        log_error "gcc >= ${MIN_GCC} is required"
+        return "${EXIT_FAILURE}"
+    fi
+    gcc_version=$(gcc -dumpversion)
+    if ! version_greater_equal "${gcc_version}" "${MIN_GCC}"; then
+        log_error "gcc ${gcc_version} is too old, >= ${MIN_GCC} is required"
+        return "${EXIT_FAILURE}"
+    fi
+
+    # Check git version
+    if ! command -v git >/dev/null 2>&1; then
+        log_error "git >= ${MIN_GIT} is required"
+        return "${EXIT_FAILURE}"
+    fi
+    git_version=$(git --version | awk '{print $3}')
+    if ! version_greater_equal "${git_version}" "${MIN_GIT}"; then
+        log_error "git ${git_version} is too old, >= ${MIN_GIT} is required"
+        return "${EXIT_FAILURE}"
+    fi
+
+    # Check versions of other tools with optional flag for bearssl
+    BEARSSL_REQUIRED=false # Can be set to true if SSL is needed
+
+    for tool in valgrind shellcheck cppcheck enca doxygen groff make gdb recsel; do
+        if ! command -v "${tool}" >/dev/null 2>&1; then
+            case "${tool}" in
+                valgrind)    version="${MIN_VALGRIND}" ;;
+                shellcheck)  version="${MIN_SHELLCHECK}" ;;
+                cppcheck)    version="${MIN_CPPCHECK}" ;;
+                enca)       version="${MIN_ENCA}" ;;
+                doxygen)    version="${MIN_DOXYGEN}" ;;
+                groff)      version="${MIN_GROFF}" ;;
+                make)       version="${MIN_MAKE}" ;;
+                gdb)        version="${MIN_GDB}" ;;
+                recsel)     version="${MIN_RECUTILS}" ;;
+                *)          log_error "Unknown tool: ${tool}"; return "${EXIT_FAILURE}" ;;
+            esac
+            log_error "${tool} >= ${version} is required"
+            return "${EXIT_FAILURE}"
+        fi
+
+        # Get installed version and check
+        case "${tool}" in
+            valgrind)    current_version=$(get_valgrind_version) ;;
+            shellcheck)  current_version=$(get_shellcheck_version) ;;
+            cppcheck)    current_version=$(get_cppcheck_version) ;;
+            enca)       current_version=$(get_enca_version) ;;
+            doxygen)    current_version=$(get_doxygen_version) ;;
+            groff)      current_version=$(get_groff_version) ;;
+            make)       current_version=$(get_make_version) ;;
+            gdb)        current_version=$(get_gdb_version) ;;
+            recsel)     current_version=$(get_recutils_version) ;;
+            *)          log_error "Unknown tool: ${tool}"; return "${EXIT_FAILURE}" ;;
+        esac
+
+        # Compare versions
+        if ! version_greater_equal "${current_version}" "${version}"; then
+            log_error "${tool} ${current_version} is too old, >= ${version} is required"
+            return "${EXIT_FAILURE}"
+        fi
+    done
+
+# Add function to check BearSSL library path
+get_bearssl_version() {
+    # First try command-line tool
+    if command -v bearssl >/dev/null 2>&1; then
+        bearssl version | awk '{print $2}'
+        return 0
+    fi
+
+    # Fall back to checking library paths?
+    for lib in \
+        /lib/libbearssl.so.* \
+        /usr/lib/libbearssl.so.* \
+        /usr/local/lib/libbearssl.so.*; do
+        if [ -f "${lib}" ]; then
+            # Extract version from library name
+            echo "${lib##*.}"
+            return 0
+        fi
+    done
+
+    return 1
+}
+
+# Update BearSSL check in check_versions()
+if command -v bearssl >/dev/null 2>&1 || get_bearssl_version >/dev/null 2>&1; then
+    bearssl_version=$(get_bearssl_version)
+    if ! version_greater_equal "${bearssl_version}" "${MIN_BEARSSL}"; then
+        if [ "${BEARSSL_REQUIRED}" = "true" ]; then
+            log_error "BearSSL ${bearssl_version} is too old, >= ${MIN_BEARSSL} is required"
+            return "${EXIT_FAILURE}"
+        else
+            log_info "WARNING: BearSSL ${bearssl_version} is older than recommended version ${MIN_BEARSSL}"
+        fi
+    fi
+else
+    if [ "${BEARSSL_REQUIRED}" = "true" ]; then
+        log_error "BearSSL >= ${MIN_BEARSSL} is required for SSL/TLS support"
+        return "${EXIT_FAILURE}"
+    else
+        log_info "WARNING: BearSSL not found. SSL/TLS features will be disabled"
+    fi
+fi
+
+    return "${EXIT_SUCCESS}"
+}
+
+# Add this helper function for version comparison
+version_greater_equal() {
+    printf '%s\n%s\n' "$1" "$2" | sort -V
+}
+
+#######################################
+# Validate environment settings match deps.txt
+# Returns:
+#   0 if environment matches, 1 if not
+#######################################
+validate_deps_env() {
+    # Check LANG
+    if [ "${LANG}" != "${REQUIRED_LANG}" ]; then
+        log_error "LANG must be ${REQUIRED_LANG} (current: ${LANG})"
+        return "${EXIT_FAILURE}"
+    fi
+
+    # Check LC_ALL
+    if [ "${LC_ALL}" != "${REQUIRED_LC_ALL}" ]; then
+        log_error "LC_ALL must be ${REQUIRED_LC_ALL} (current: ${LC_ALL})"
+        return "${EXIT_FAILURE}"
+    fi
+
+    # Check CHARSET
+    if [ "${CHARSET}" != "${REQUIRED_CHARSET}" ]; then
+        log_error "CHARSET must be ${REQUIRED_CHARSET} (current: ${CHARSET})"
+        return "${EXIT_FAILURE}"
+    fi
+
+    return "${EXIT_SUCCESS}"
+}
+
+#######################################
+# Display help message
+#######################################
+show_help() {
+    cat << EOF
+Usage: ${0} [--help]
+
+Check development dependencies and environment settings against deps.txt requirements.
+
+Options:
+  --help    Display this help message and exit
+
+Environment:
+  LANG      Must be en_AU.ISO8859-1
+  LC_ALL    Must be en_AU.ISO8859-1
+  CHARSET   Must be ISO-8859-1
+EOF
+}
+
+#######################################
+# Main execution
+#######################################
+main() {
+    if [ "$1" = "--help" ]; then
+        show_help
+        exit "${EXIT_SUCCESS}"
+    fi
+
+    log_info "Checking installed development tools and versions..."
+
+    # Parse deps file
+    if ! parse_deps_file; then
+        exit "${EXIT_FAILURE}"
+    fi
+
+    # Validate environment settings
+    if ! validate_deps_env; then
+        log_error "Environment settings do not match deps.txt requirements"
+        exit "${EXIT_FAILURE}"
+    fi
+
+    # Check tool versions
+    if ! check_versions; then
+        exit "${EXIT_FAILURE}"
+    fi
+
+    log_info "All dependency checks passed successfully"
+    exit "${EXIT_SUCCESS}"
+}
+
+# Execute main directly (POSIX-compliant)
+case ${0##*/} in
+    check-deps.sh) main "$@" ;;
+    *) log_error "Script must be run as check-deps.sh"; exit "${EXIT_FAILURE}" ;;
+esac
+
+# vim: set ft=sh ts=4 sw=4 et:
diff --git a/scripts/build/setup-env.sh b/scripts/build/setup-env.sh
new file mode 100755
index 0000000..aa0e363
--- /dev/null
+++ b/scripts/build/setup-env.sh
@@ -0,0 +1,304 @@
+#!/bin/sh
+
+#######################################
+# Setup build environment for project
+#
+# Usage: ./setup-env.sh
+#
+# Environment variables:
+#   CC        - C compiler to use (defaults to gcc)
+#   CFLAGS    - C compiler flags
+#   LDFLAGS   - Linker flags
+#######################################
+
+# Strict mode
+set -e  # Exit on error
+set -u  # Exit on undefined variables
+
+#######################################
+# Constants
+#######################################
+
+# Version info
+VERSION_FILE="VERSION"
+if [ -f "${VERSION_FILE}" ]; then
+    VERSION=$(cat "${VERSION_FILE}")
+else
+    VERSION="0.0.2"
+fi
+
+# Parse version components
+MAJOR_VERSION=$(echo "${VERSION}" | cut -d. -f1)
+MINOR_VERSION=$(echo "${VERSION}" | cut -d. -f2)
+PATCH_VERSION=$(echo "${VERSION}" | cut -d. -f3)
+
+# Export version info
+export VERSION
+export MAJOR_VERSION
+export MINOR_VERSION
+export PATCH_VERSION
+
+# Base directories
+export SRCDIR="src"
+export TESTDIR="test"
+export INCLUDEDIR="include"
+export LOGDIR="var/log/scripts/build"
+export BUILDDIR="build"
+export BINDIR="bin"
+export LIBDIR="lib"
+export MANDIR="man"
+export DOCSDIR="docs"
+export CONFIGDIR="config"
+export SCRIPTDIR="scripts"
+export CACHEDIR="cache"
+export PIDDIR="pid"
+export TMPDIR="tmp"
+export VARDIR="var"
+export PRIVATEDIR="private"
+export PUBLICDIR="public"
+
+# Build directories
+export DEBUG_DIR="${BUILDDIR}/debug"
+export RELEASE_DIR="${BUILDDIR}/release"
+export DEBUG_DEPS_DIR="${DEBUG_DIR}/deps"
+export DEBUG_OBJ_DIR="${DEBUG_DIR}/obj"
+export RELEASE_DEPS_DIR="${RELEASE_DIR}/deps"
+export RELEASE_OBJ_DIR="${RELEASE_DIR}/obj"
+export DIST_DIR="${BUILDDIR}/dist"
+
+# Library directories
+export PKGCONFIG_DIR="${LIBDIR}/pkgconfig"
+export STATIC_LIB_DIR="${LIBDIR}/static"
+
+# Documentation directories
+export MAN1_DIR="${MANDIR}/man1"
+export MAN3_DIR="${MANDIR}/man3"
+export MAN5_DIR="${MANDIR}/man5"
+export MAN8_DIR="${MANDIR}/man8"
+
+# Data directories
+export CHARSET_DIR="charset"
+export CHECKSUMS_DIR="checksums"
+export ARCHIVE_DIR="archive"
+export BACKUP_DIR="backup"
+
+# Runtime directories
+export VAR_DB_DIR="${VARDIR}/db"
+export VAR_LOG_DIR="${VARDIR}/log"
+export VAR_DATA_DIR="${VARDIR}/data"
+export VAR_RECORDS_DIR="${VARDIR}/records"
+
+# SSL directories
+export SSL_DIR="${PRIVATEDIR}/ssl"
+export SSL_CERTS_DIR="${SSL_DIR}/certs"
+
+# Public web directories
+export PUBLIC_ASSETS_DIR="${PUBLICDIR}/assets"
+export PUBLIC_VIEWS_DIR="${PUBLICDIR}/views"
+
+# Compiler settings
+: "${CC:=gcc}"
+: "${CFLAGS:=-std=c90 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=500 -Wall -ansi -Wextra -pedantic}"
+: "${LDFLAGS:=-static -pthread}"
+
+export CC
+export CFLAGS
+export LDFLAGS
+
+#######################################
+# Functions
+#######################################
+
+# Print error message and exit
+error() {
+    printf "Error: %s\n" "$1" >&2
+    exit 1
+}
+
+# Check if a command exists
+check_cmd() {
+    command -v "$1" >/dev/null 2>&1 || error "Required command $1 not found"
+}
+
+# Create directory if it doesn't exist
+ensure_dir() {
+    [ -d "$1" ] || mkdir -p "$1" || error "Could not create directory $1"
+}
+
+# Setup build infrastructure
+setup_build_dirs() {
+    # Create all required build directories
+    ensure_dir "${BUILDDIR}"
+    ensure_dir "${DEBUG_DIR}"
+    ensure_dir "${DEBUG_DEPS_DIR}"
+    ensure_dir "${DEBUG_OBJ_DIR}"
+    ensure_dir "${RELEASE_DIR}"
+    ensure_dir "${RELEASE_DEPS_DIR}"
+    ensure_dir "${RELEASE_OBJ_DIR}"
+    ensure_dir "${DIST_DIR}"
+
+    # Create library directories
+    ensure_dir "${LIBDIR}"
+    ensure_dir "${PKGCONFIG_DIR}"
+    ensure_dir "${STATIC_LIB_DIR}"
+
+    # Create documentation directories
+    ensure_dir "${DOCSDIR}"
+    ensure_dir "${MAN1_DIR}"
+    ensure_dir "${MAN3_DIR}"
+    ensure_dir "${MAN5_DIR}"
+    ensure_dir "${MAN8_DIR}"
+
+    # Create data directories
+    ensure_dir "${CHARSET_DIR}"
+    ensure_dir "${CHECKSUMS_DIR}"
+    ensure_dir "${ARCHIVE_DIR}"
+    ensure_dir "${BACKUP_DIR}"
+
+    # Create runtime directories
+    ensure_dir "${VAR_DB_DIR}"
+    ensure_dir "${VAR_LOG_DIR}"
+    ensure_dir "${VAR_DATA_DIR}"
+    ensure_dir "${VAR_RECORDS_DIR}"
+
+    # Create SSL directories
+    ensure_dir "${SSL_DIR}"
+    ensure_dir "${SSL_CERTS_DIR}"
+
+    # Create public web directories
+    ensure_dir "${PUBLIC_ASSETS_DIR}"
+    ensure_dir "${PUBLIC_VIEWS_DIR}"
+
+    # Create temporary directories
+    ensure_dir "${TMPDIR}"
+    ensure_dir "${CACHEDIR}"
+    ensure_dir "${PIDDIR}"
+}
+
+# Validate environment
+validate_env() {
+    # Check required commands
+    check_cmd "${CC}"
+    check_cmd "make"
+
+    # Validate base directories
+    ensure_dir "${SRCDIR}"
+    ensure_dir "${TESTDIR}"
+    ensure_dir "${BINDIR}"
+    ensure_dir "${INCLUDEDIR}"
+    ensure_dir "${LOGDIR}"
+    ensure_dir "${CONFIGDIR}"
+    ensure_dir "${SCRIPTDIR}"
+
+    # Setup all other directories
+    setup_build_dirs
+
+    # Check compiler works
+    if ! "${CC}" -v >/dev/null 2>&1; then
+        error "Compiler ${CC} is not working"
+    fi
+}
+
+# Setup build environment
+setup_env() {
+    # Check for root user
+    user_id=$(id -u) || error "Failed to get user ID"
+    if [ "${user_id}" = "0" ]; then
+        error "This script should not be run as root"
+    fi
+
+    # Validate environment
+    validate_env
+
+    # Create build config
+    cat > "${BUILDDIR}/config.mk" << EOF
+# Generated by setup-env.sh
+VERSION := ${VERSION}
+MAJOR_VERSION := ${MAJOR_VERSION}
+MINOR_VERSION := ${MINOR_VERSION}
+PATCH_VERSION := ${PATCH_VERSION}
+
+CC := ${CC}
+CFLAGS := ${CFLAGS}
+LDFLAGS := ${LDFLAGS}
+
+# Base directories
+SRCDIR := ${SRCDIR}
+TESTDIR := ${TESTDIR}
+INCLUDEDIR := ${INCLUDEDIR}
+LOGDIR := ${LOGDIR}
+BUILDDIR := ${BUILDDIR}
+BINDIR := ${BINDIR}
+LIBDIR := ${LIBDIR}
+MANDIR := ${MANDIR}
+DOCSDIR := ${DOCSDIR}
+CONFIGDIR := ${CONFIGDIR}
+SCRIPTDIR := ${SCRIPTDIR}
+CACHEDIR := ${CACHEDIR}
+PIDDIR := ${PIDDIR}
+TMPDIR := ${TMPDIR}
+VARDIR := ${VARDIR}
+PRIVATEDIR := ${PRIVATEDIR}
+PUBLICDIR := ${PUBLICDIR}
+
+# Build directories
+DEBUG_DIR := ${DEBUG_DIR}
+DEBUG_DEPS_DIR := ${DEBUG_DEPS_DIR}
+DEBUG_OBJ_DIR := ${DEBUG_OBJ_DIR}
+RELEASE_DIR := ${RELEASE_DIR}
+RELEASE_DEPS_DIR := ${RELEASE_DEPS_DIR}
+RELEASE_OBJ_DIR := ${RELEASE_OBJ_DIR}
+DIST_DIR := ${DIST_DIR}
+
+# Library directories
+PKGCONFIG_DIR := ${PKGCONFIG_DIR}
+STATIC_LIB_DIR := ${STATIC_LIB_DIR}
+
+# Documentation directories
+MAN1_DIR := ${MAN1_DIR}
+MAN3_DIR := ${MAN3_DIR}
+MAN5_DIR := ${MAN5_DIR}
+MAN8_DIR := ${MAN8_DIR}
+
+# Data directories
+CHARSET_DIR := ${CHARSET_DIR}
+CHECKSUMS_DIR := ${CHECKSUMS_DIR}
+ARCHIVE_DIR := ${ARCHIVE_DIR}
+BACKUP_DIR := ${BACKUP_DIR}
+
+# Runtime directories
+VAR_DB_DIR := ${VAR_DB_DIR}
+VAR_LOG_DIR := ${VAR_LOG_DIR}
+VAR_DATA_DIR := ${VAR_DATA_DIR}
+VAR_RECORDS_DIR := ${VAR_RECORDS_DIR}
+
+# SSL directories
+SSL_DIR := ${SSL_DIR}
+SSL_CERTS_DIR := ${SSL_CERTS_DIR}
+
+# Public web directories
+PUBLIC_ASSETS_DIR := ${PUBLIC_ASSETS_DIR}
+PUBLIC_VIEWS_DIR := ${PUBLIC_VIEWS_DIR}
+EOF
+
+    # Get current date/time
+    current_time=$(date) || error "Failed to get current time"
+
+    # Log setup completion
+    mkdir -p "${LOGDIR}"
+    {
+        printf "Build environment setup completed at %s\n" "${current_time}"
+        printf "Using compiler: %s\n" "${CC}"
+        printf "Version: %s\n" "${VERSION}"
+    } >> "${LOGDIR}/setup.log"
+
+    printf "Build environment setup completed successfully\n"
+}
+
+#######################################
+# Main
+#######################################
+
+setup_env
+
+# vim: set ft=sh ts=4 sw=4 et:
diff --git a/web-app.code-workspace b/web-app.code-workspace
index 3711698..f55da44 100644
--- a/web-app.code-workspace
+++ b/web-app.code-workspace
@@ -336,7 +336,31 @@
     "githubPullRequests.quickDiff": true,
     "githubPullRequests.showPullRequestNumberInTree": true,
     "githubPullRequests.terminalLinksHandler": "vscode",
-    "chat.detectParticipant.enabled": false
+    "chat.detectParticipant.enabled": false,
+    "shellcheck.enable": true,
+    "shellcheck.enableQuickFix": true,
+    "shellcheck.run": "onType",
+    "shellcheck.executablePath": "/usr/bin/shellcheck",
+    "shellcheck.logLevel": "error",
+    "[shellcheckrc]": {
+      "files.encoding": "iso88591",
+      "files.autoGuessEncoding": false,
+      "files.trimTrailingWhitespace": true
+    },
+    "[shellcheck-output]": {
+      "editor.wordWrap": "on",
+      "editor.trimAutoWhitespace": false,
+      "editor.defaultFormatter": "timonwong.shellcheck",
+      "files.encoding": "iso88591",
+      "files.autoGuessEncoding": false,
+      "files.trimTrailingWhitespace": false
+    },
+    "[shellscript]": {
+      "files.eol": "\n",
+      "editor.defaultColorDecorators": "auto",
+    },
+    "shellcheck.disableVersionCheck": true,
+    "shellcheck.useWorkspaceRootAsCwd": false
   },
   "extensions": {
     "recommendations": [
.../appuser/web-app (96-2-improve-record-creation-and-editing-functionality+) $ 
