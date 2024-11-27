# Copilot Instructions for EnvEng Web Application Development

These instructions help align Copilot's suggestions with our project's requirements.

## General Guidelines
- Adhere to ISO/IEC 9899:2024 (C Standard), POSIX.1-2024, and X/Open 800 compliance in all code suggestions.
- Ensure all code is portable, cross-platform, and cross-architecture. Avoid platform-specific features unless encapsulated for easy replacement.

## Code Structure
### File Organization
- Maintain the specified directory structure.
- Use snake_case for file names, e.g., `data_manager.c`, `user_auth.h`.

### Naming Conventions
- **Variables**: Use `snake_case` (e.g., `user_id`).
- **Functions**: Use `camelCase` (e.g., `processData`).
- **Constants**: Use `UPPER_CASE` (e.g., `MAX_BUFFER_SIZE`).

## Coding Practices
### Data Handling
- Keep data definitions separate from logic. Define data structures in header files and implement logic in C files.
- Use immutable data structures whenever applicable.

### Error Handling
- Always check return values of system calls and library functions.

### Memory Management
- Avoid memory leaks. Always pair memory allocation (`malloc`) with deallocation (`free`).

### Performance
- Optimize for CPU cache efficiency with data structures (e.g., prefer arrays over linked lists for sequential access).

### Thread Safety
- Ensure thread safety using POSIX threading primitives (e.g., `pthread_mutex_t`).

## Style and Formatting
### Indentation
- Use spaces (not tabs) for indentation, with an indent size of 4.

### Brace Style
- Use the Allman brace style.

### Quotes
- Prefer double quotes for strings in C and header files.

### Additional Formatting Rules
- Align consecutive assignments and declarations.
- Limit lines to 80 characters for readability.

## Documentation
- Document all files and functions using block comments.
- Use Markdown for external documentation and place it in the `docs/` directory.

## Testing
- Write unit tests for all functions and features.
- Follow the naming convention `test_<module_name>.c` for test files.
- Use a unit testing framework like `Unity` or `CMock`.

## Tools and Practices
- Use Git for version control, and ensure commit messages follow the format: `[Type] Short description`.
- Use CI/CD pipelines to automate testing and code quality checks.
- Regularly review and refactor code for maintainability and performance.

## Copilot Usage Guidelines
- Focus on providing code snippets that can be copied into existing files rather than rewriting entire files.
- Ensure that the generated snippets adhere to the project's coding standards and practices as outlined in this document.
- Provide context-specific suggestions that integrate seamlessly with the existing codebase.

### File Organization

- Maintain the following directory structure:

```
.
├── ABOUT-NLS
├── AUTHORS
├── COPYING
├── ChangeLog
├── INSTALL
├── LICENSE
├── Makefile.am
├── NEWS
├── README.html
├── README.md
├── bin
│   └── Makefile.am
├── build
│   ├── Makefile.am
├── clib.json
├── common-rules.am
├── configure.ac
├── deps
│   ├── Makefile.am
│   └── generate_deps.sh
├── dist
│   └── Makefile.am
├── docs
│   └── Makefile.am
├── etc
│   ├── Makefile.am
│   ├── config.conf
│   ├── config.ini
│   ├── gcc.spec
│   ├── header_sources.txt
│   ├── process_files.gawk
│   ├── scan_list.txt
│   ├── sources.txt
│   └── test_sources.txt
├── gtk-doc.make
├── include
│   ├── Makefile.am
│   ├── config.h
│   ├── config_loader.h
│   ├── env_loader.h
│   ├── error_handler.h
│   ├── garbage_collector.h
│   ├── hello.h
│   ├── logger.h
│   └── validator.h
├── lib
│   ├── Makefile.am
│   └── unity.h
├── logs
│   └── Makefile.am
├── m4
│   └── Makefile.am
├── objects
│   └── Makefile.am
├── package-lock.json
├── package.json
├── po
│   ├── ChangeLog
│   ├── LINGUAS
│   ├── Makefile.am
│   └── POTFILES.in
├── src
│   ├── Makefile.am
│   ├── config_loader.c
│   ├── env_loader.c
│   ├── error_handler.c
│   ├── garbage_collector.c
│   ├── hello.c
│   ├── lexer.l
│   ├── logger.c
│   ├── main.c
│   ├── parser.y
│   └── validator.c
├── tests
│   ├── Makefile.am
│   ├── test_config_loader.c
│   ├── test_env_loader.c
│   ├── test_error_handler.c
│   ├── test_garbage_collector.c
│   ├── test_hello.c
│   ├── test_logger.c
│   ├── test_main.c
│   └── test_validator.c
├── tmp
│   └── Makefile.am
├── web-app.code-workspace
└── xml
    └── gtkdocentities.ent
```
