# Web Server Project Documentation

## Overview
A lightweight, POSIX-compliant HTTP server implemented in ANSI C (C90) with support for basic HTTP requests. This project adheres to strict compliance with POSIX.1 and X/Open-500 standards.

## Project Structure
```plaintext
/web-app/
├── include/           # Header files
│   └── server.h      # Server interface declarations
├── src/              # Source files
│   ├── main.c        # Entry point
│   └── server.c      # Server implementation
├── test/             # Test files
│   └── test_server.c # Server unit tests
├── www/              # Web content
│   └── index.html    # Default webpage
└── Makefile         # Build system
```

## Build Requirements
- POSIX-compliant system
- C compiler (gcc/musl-gcc)
- GNU Make
- CUnit (for testing)
- OpenSSL development libraries

## Building the Project
### Development Build
```bash
make ENV=dev
```

### Production Build
```bash
make ENV=prod
```

### Running Tests
```bash
make test
```

## Key Features
- POSIX-compliant HTTP/1.0 server
- Thread-safe implementation
- Static file serving
- Signal handling (SIGINT, SIGTERM)
- Configurable port binding
- Comprehensive error logging via syslog

## Configuration
- Default HTTP port: 80 (production), 49152 (testing)
- Buffer size: 1024 bytes
- Max connection queue: SOMAXCONN

## API Reference
### Server Interface
```c
int server_init(void);              /* Initialize server */
int server_run(int server_fd);      /* Handle connections */
void server_cleanup(int server_fd); /* Cleanup resources */
int server_get_port(int server_fd); /* Get bound port */
```

## Testing
Unit tests are implemented using CUnit framework, covering:
- Server initialization
- Port binding
- Connection handling
- Resource cleanup

## License
Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
SPDX-License-Identifier: AGPL-3.0-or-later

## Security Considerations
- Uses OpenSSL for cryptographic operations
- Implements proper socket cleanup
- Validates all system call returns
- Bounds checking on buffer operations
- Limited file system access

## Dependencies
- musl libc
- OpenSSL
- POSIX threads
- CUnit (test only)
