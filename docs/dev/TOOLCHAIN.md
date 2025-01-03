# Toolchain

## Compiler
- **GCC**: The GNU Compiler Collection is used for compiling the C code.
  - **Flags**: `-static -O3 -std=c90 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=500 -Wall -ansi -Wextra -pedantic -Werror -Wshadow -Wconversion -Wstrict-prototypes -Wmissing-prototypes -Warray-bounds -Wformat=2 -Wformat-security -Wformat-overflow=2 -Wformat-truncation=2 -Wvla -Wbad-function-cast -Wstrict-aliasing=2 -Wnull-dereference -Wdouble-promotion -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wcast-align -Wcast-qual -Wredundant-decls -Wundef -Wmissing-include-dirs -Winit-self -Wswitch-enum -Wmissing-declarations -Wsign-conversion -fstack-protector-strong -fstack-check -fPIE -fstack-protector-all -fdata-sections -ffunction-sections -fno-common -fstrict-aliasing -fno-strict-overflow -fanalyzer -fno-omit-frame-pointer -finput-charset=iso-8859-1 -fexec-charset=iso-8859-1 -fwide-exec-charset=iso-8859-1`

## Libraries
- **musl libc**: A lightweight, fast, and simple implementation of the standard library for Linux-based systems.

## Build System
- **Make**: Used for automating the build process.
  - **Targets**:
    - `make ENV=dev`: Development build with debug symbols.
    - `make ENV=prod`: Production build with optimizations.
    - `make install`: Install based on ENV.
    - `make clean`: Clean build artifacts.

## Debugging Tools
- **GDB**: The GNU Debugger is used for debugging the application.
- **Valgrind**: Used for memory analysis and debugging.

## Version Control
- **Git**: Used for version control.
  - **Branching Strategy**: Follow GitFlow branching model.
  - **Commit Messages**: Follow the format `[Type] Short description`.

## Documentation Tools
- **Doxygen**: Used for generating documentation from annotated source code.
- **Markdown**: Used for writing documentation files.

## Testing Framework
- **CUnit**: A unit testing framework for C.

## Continuous Integration
- **GitHub Actions**: Used for automating testing and code quality checks.
