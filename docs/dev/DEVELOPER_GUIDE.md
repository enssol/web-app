# Development Tools

## Core Build Tools

### musl-gcc
- Primary C compiler with musl libc support
- Used for production builds with static linking
- Ensures POSIX compliance and portability

### GNU Make
- Build automation system 
- Handles compilation and linking
- Manages build targets and dependencies

### CUnit
- Unit testing framework for C
- Supports test suites and assertions
- Integrated with build process

### Valgrind
- Memory analysis and debugging
- Detects memory leaks and errors
- Validates memory management

## Development Environment

### GDB (GNU Debugger)
- Interactive debugger
- Supports breakpoints and inspection
- Memory and stack analysis

### Git
- Version control system
- Branch management
- Collaboration support

### Shellcheck
- Shell script validator
- Ensures script portability
- Identifies common errors

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

### OpenSSL
- SSL/TLS implementation
- Certificate management and key generation
- Cryptographic operations and secure communications
- Optimized for embedded systems

### checksec
- Binary security analysis
- Stack protection verification
- Security feature validation

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
