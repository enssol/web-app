# General Documentation

## Overview
This document provides general information and guidelines for the EnvEng Web Application Development project.

## Project Goals
- Develop a POSIX-compliant web application
- Ensure compatibility with musl libc and gcc
- Adhere to ISO/IEC 9899:1990 (C90 Standard)
- Use ISO-8859-1 encoding for all files

## Development Guidelines
- Follow the project's coding standards and style guidelines
- Ensure code is portable, cross-platform, and cross-architecture
- Write code optimized for lightweight environments
- Use data-oriented design over object-oriented design

## Build and Deployment
- Use GCC with musl libc for compilation
- Ensure all binaries are statically linked
- Use the provided build flags for compliance and optimization
- Deploy static binaries with ISO-8859-1 encoding

## Testing and Validation
- Write unit tests using CUnit framework
- Validate memory safety with Valgrind
- Ensure POSIX compliance with POSIX Test Suite (PTS)
- Perform security testing with BearSSL and checksec

## Documentation
- Use Doxygen for code documentation
- Generate man pages with Groff
- Maintain up-to-date documentation for all components

## Contact
For any questions or issues, please refer to the [Support Information](../user/SUPPORT.md).
