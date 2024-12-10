# **Copilot Instructions for EnvEng Web Application Development**

These instructions ensure Copilot's suggestions align with our project's technical standards and development environment.

---

## **General Guidelines**
- Adhere to **ISO/IEC 9899:1990 (C90 Standard)**, ** POSIX.1 (ISO/IEC 9945-1:1996 - IEEE Standard 1003.1)**, and **X/Open** compliance in all code suggestions.
- Ensure code is **portable**, **cross-platform**, and **cross-architecture**:
    - Prioritize compatibility with `musl libc`, and `musl-clang`.
    - Avoid features requiring `glibc` or non-portable platform-specific APIs.
- Write code optimized for lightweight environments (e.g. `musl`).
- Data oriented design is preferred over object oriented design.
- Format C Code following GNU Coding Standards.
- Do not use C++ comments (`//`) in C code.
- Do not use undeclared identifier.
- Generate code that does not allow variable declarations in for loop initializers
- Always define variables at the beginning of the block.
- Always define functions before they are called.
- Always define reference variables with the `&` operator.
- Always define pointers with the `*` operator.
- Always define arrays with the `[]` operator.
- Never generate code that has undefined reference behaviour.
- Never redefine a variable in the same scope.
- Avoid generating code that are conflicting types for the same variable.
- Avoid previous declarations of variables.

---

## **Code Structure**
### **File Organization**
- Follow a structured directory layout:
    - **src/**: Source files (`.c`)
    - **include/**: Header files (`.h`)
    - **obj/**: Object files (`.o`)
    - **bin/**: Binary artifacts
    - **etc/**: Config test files
    - **docs/**: Documentation files
- Use `snake_case` for file names, e.g., `data_manager.c`, `user_auth.h`.

### **Naming Conventions**
- **Variables**: Use `snake_case` (e.g., `user_id`, `config_path`).
- **Functions**: Use `camelCase` (e.g., `processData`, `validateUser`).
- **Constants**: Use `UPPER_CASE` (e.g., `MAX_BUFFER_SIZE`, `DEFAULT_TIMEOUT`).

---

## **Coding Practices**
### **Data Handling**
- Keep data definitions separate from logic:
    - Define data structures in header files (`.h`).
    - Implement logic in source files (`.c`).
- Use immutable data structures whenever applicable for safety and clarity.

### **Error Handling**
- Always check return values of system calls and library functions, including:
    - Memory allocation (`malloc`, `calloc`).
    - File I/O (`fopen`, `fread`, `fwrite`).
    - Threading functions (`pthread_mutex_lock`, `pthread_create`).
- Use consistent error codes (`errno`) and logging to diagnose failures.

### **Memory Management**
- Prevent memory leaks by always pairing:
    - **Allocation** (`malloc`, `calloc`) with **Deallocation** (`free`).
- Avoid using dynamic memory where possible in performance-critical paths.
- Use stack allocation for predictable performance and reduced overhead.

### **Performance**
- Optimize for **CPU cache efficiency**:
    - Use arrays instead of linked lists for sequential data access.
    - Minimize pointer indirection and fragmentation.
- Enable compiler optimizations with appropriate flags (e.g., `-ffunction-sections`, `-fdata-sections`).
- Prefer static linking (`-static`) to ensure binaries are self-contained and lightweight.

### **Thread Safety**
- Use POSIX threading primitives:
    - Mutexes (`pthread_mutex_t`) for synchronized access to shared resources.
    - Condition variables (`pthread_cond_t`) for thread signaling.
- Avoid global variables or use them sparingly with proper synchronization.
- Ensure all functions are reentrant if they may be called in multi-threaded contexts.

---

## **Environment-Specific Considerations**
- **Compiler**: Target **musl-gcc (gcc)** with the following capabilities:
    - Use `-std=c90` for ISO C compliance.
    - Define `_POSIX_C_SOURCE` and `_XOPEN_SOURCE` for POSIX and X/Open features supported by musl.
    - Avoid advanced GNU and GCC-specific extensions unsupported by clang.
- **Libraries**:
    - Base code on **musl libc** for portability and minimalism.
    - Ensure compatibility with musl's threading and math libraries.
- **Containerized Development**:
    - Target **AMD64** architecture with static linking for compatibility across container instances.
    - Optimize for minimal dependencies, leveraging musl’s built-in features where possible.

---

## **Testing and Validation**
- Write unit tests for all functions using the following naming conventions:
    - `test_<module_name>.c` for test files.
    - `test_<function_name>` for individual test cases.
- Use memory analysis tools (e.g., `valgrind` or equivalent musl-compatible tools) to validate memory safety.
- Run all tests within the containerized environment to ensure consistency.

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
- Document all files and functions using C90 comments style.
- Use Doxygen for external documentation.

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

```plaintext
.
├── ACKNOWLEDGEMENT.md
├── app.log
├── AUTHORS
├── CHANGELOG
├── CMakeLists.txt
├── CODE_OF_CONDUCT.md
├── compile_commands.json
├── CONTRIBUTING.md
├── COPYING
├── docs
│   ├── ARCHITECTURE.md
│   ├── CONTACT.md
│   ├── CONTRIBUTORS.md
│   ├── CREDITS.md
│   ├── DEVELOPERS.md
│   ├── DISCLAIMER.md
│   ├── EXAMPLES.md
│   ├── FAQ.md
│   ├── GLOSSARY.md
│   ├── GOVERNANCE.md
│   ├── HISTORY.md
│   ├── MIGRATION.md
│   ├── PRIVACY.md
│   ├── REFERENCES.md
│   ├── RELEASE_NOTES.md
│   ├── REQUIREMENTS.md
│   ├── ROADMAP.md
│   ├── STYLEGUIDE.md
│   ├── TERMS.md
│   ├── TROUBLESHOOTING.md
│   ├── TUTORIAL.md
│   └── USAGE.md
├── Doxyfile
├── entries[config-
├── entry_count++]
├── etc
│   ├── config.ini
│   ├── letsencrypt
│   │   └── live
│   │       └── yourdomain.com
│   │           ├── cert.pem
│   │           └── privkey.pem
│   └── server
│       └── auth.passwd
├── HACKING
├── include
│   ├── atomic_ops.h
│   ├── compat.h
│   ├── compat_layer.h
│   ├── compat_types.h
│   ├── config_loader.h
│   ├── constants.h
│   ├── csv_utils.h
│   ├── data_structures.h
│   ├── env_loader.h
│   ├── error_codes.h
│   ├── error_handler.h
│   ├── external_includes.h
│   ├── garbage_collector.h
│   ├── http_parser.h
│   ├── http_response.h
│   ├── logger.h
│   ├── project.h
│   ├── records.h
│   ├── rec_utils.h
│   ├── server.h
│   ├── socket_module.h
│   ├── ssl_module.h
│   ├── static_file_handler.h
│   ├── test_framework.h
│   ├── utils.h
│   └── validator.h
├── INSTALL
├── LICENSE
├── logfile.log
├── MAINTAINERS
├── MONITORING_AND_EVALUATION
├── package.json
├── package-lock.json
├── README.md
├── SECURITY.md
├── src
│   ├── add_custom_uint64.s
│   ├── atomic_ops.c
│   ├── compat.c
│   ├── compat_layer.c
│   ├── compatStrtol64_asm.s
│   ├── config_loader.c
│   ├── constants.c
│   ├── csv_utils.c
│   ├── data_structures.c
│   ├── div_custom_uint64.s
│   ├── env_loader.c
│   ├── error_handler.c
│   ├── garbage_collector.c
│   ├── http_parser.c
│   ├── http_response.c
│   ├── logger.c
│   ├── main.c
│   ├── records.c
│   ├── rec_utils.c
│   ├── server.c
│   ├── sha256_asm.c
│   ├── socket_module.c
│   ├── ssl_module.c
│   ├── static_file_handler.c
│   ├── utils.c
│   └── validator.c
├── SUPPORT.md
├── TODO.md
├── var
│   └── www
│       ├── data
│       │   └── records.rec
│       └── html
│           └── index.html
└── web-app.code-workspace

13 directories, 105 files
```

### POSIX Headers only allowed to use
<aio.h>
<arpa/inet.h>
<assert.h>
<complex.h>
<cpio.h>
<ctype.h>
<dirent.h>
<dlfcn.h>
<errno.h>
<fcntl.h>
<fenv.h>
<float.h>
<fnmatch.h>
<ftw.h>
<glob.h>
<grp.h>
<iconv.h>
<inttypes.h>
<iso646.h>
<langinfo.h>
<libgen.h>
<limits.h>
<locale.h>
<math.h>
<monetary.h>
<mqueue.h>
<net/if.h>
<netdb.h>
<netinet/in.h>
<netinet/tcp.h>
<nl_types.h>
<poll.h>
<pthread.h>
<pwd.h>
<regex.h>
<sched.h>
<search.h>
<semaphore.h>
<setjmp.h>
<signal.h>
<stdarg.h>
<stdbool.h>
<stddef.h>
<stdint.h>
<stdio.h>
<stdlib.h>
<string.h>
<strings.h>
<sys/ipc.h>
<sys/mman.h>
<sys/msg.h>
<sys/resource.h>
<sys/select.h>
<sys/sem.h>
<sys/shm.h>
<sys/socket.h>
<sys/stat.h>
<sys/statvfs.h>
<sys/time.h>
<sys/times.h>
<sys/types.h>
<sys/uio.h>
<sys/un.h>
<sys/utsname.h>
<sys/wait.h>
<syslog.h>
<termios.h>
<tgmath.h>
<time.h>
<ulimit.h>
<unistd.h>
<utime.h>
<wchar.h>
<wctype.h>
<wordexp.h>
