# Development Tools

## Core Build Tools

### Build-base
- Essential build tools
- Includes GCC, Make, Musl and others
- Supports compilation and linking

### Busybox
- Single binary with multiple utilities
- Provides core Unix utilities
- Simplifies system management

### Binutils
- Binary utilities for object files
- Supports object file manipulation
- Simplifies binary management

## Development Environment

### GDB (GNU Debugger)
- Interactive debugger
- Supports breakpoints and inspection
- Memory and stack analysis

### Git
- Version control system
- Branch management
- Collaboration support

### Github-CLI
- Command line interface for Github
- Supports repository management
- Simplifies pull requests and issues

### Gcompat
- Compatibility library for GNU extensions
- Provides GNU extensions for musl libc
- Ensures compatibility with GNU tools

### Dropbear
- Lightweight SSH client and server
- Secure remote access
- Simplifies file transfer

### GPG
- Encryption and signing tool
- Secure data management
- Supports key management

## Code Quality

### Splint
- Static code analyzer
- Enforces C90/ANSI C standards 
- Checks for coding violations

### Cppcheck
- Static analysis tool
- Memory leak detection
- Style checking

### enca
- Character encoding validator
- Ensures ISO-8859-1 compliance
- Checks file encodings

### Valgrind
- Memory analysis and debugging
- Detects memory leaks and errors
- Validates memory management

### Shellcheck
- Shell script validator
- Ensures script portability
- Identifies common errors

## Documentation

### Doxygen
- Code documentation generator
- Produces HTML/man documentation
- Extracts documentation from source

### Groff
- Manual page formatter
- Generates man pages
- Processes documentation

## Testing

### CUnit
- Unit testing framework for C
- Supports test suites and assertions
- Integrated with build process

### POSIX Test Suite (PTS)
- POSIX compliance testing
- API compatibility validation
- System call verification

### gcov
- Code coverage analysis
- Test coverage reporting
- Identifies untested code

### gprof
- Performance profiler
- Function timing analysis
- Call graph generation

## Security

### BearSSL
- lightweight SSL/TLS implementation
- Certificate management and key generation
- Cryptographic operations and secure communications
- Optimized for embedded systems

### checksec
- Binary security analysis
- Stack protection verification
- Security feature validation

## Tools

### Recutils
- Text-based database management
- Supports structured data storage
- Simplifies data management

## Build Requirements

All tools must be configured for:
- ISO-8859-1 encoding
- POSIX compliance
- C90 standard support

## Environment Setup

Required environment variables:
```bash
LANG=en_AU.ISO8859-1
LC_ALL=en_AU.ISO8859-1
CHARSET=ISO-8859-1
```
