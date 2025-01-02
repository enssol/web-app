# **Copilot Instructions for EnvEng Web Application Development**

These instructions ensure Copilot's suggestions align with our project's technical standards and development environment.

---

## **General Guidelines**
- Adhere to **ISO/IEC 9899:1990 (C90 Standard)**, ** POSIX.1 (ISO/IEC 9945-1:1996 - IEEE Standard 1003.1)**, and **X/Open-500** compliance in all code suggestions.
- Ensure code is **portable**, **cross-platform**, and **cross-architecture**:
    - Prioritize compatibility with `musl libc`, and `gcc` (for Alpine)
    - Avoid features requiring `glibc` or non-portable platform-specific APIs.
- Write code optimized for lightweight environments (e.g. `musl`).
- Data oriented design is preferred over object oriented design.
- I dont want improvements, please only review and check they integrate well (consolidate) and cover the basic use-case functionality with simplicity without conflicts and duplication (cleanup code).
- Never overcomplicate the code generated.
- Ensure all files use **ISO-8859-1** encoding:
    - Set encoding to ISO-8859-1 (Latin-1) for all source files.
    - Avoid UTF-8 or other Unicode encodings.
    - Use only ASCII-compatible characters where possible.
    - For web files:
        - HTML: Use `content="text/html; charset=ISO-8859-1"` in meta tags.
        - CSS: Ensure all files are saved in ISO-8859-1 encoding.
        - JavaScript: Use only ISO-8859-1 compatible characters.
    - For C source files:
        - Use only ISO-8859-1 compatible characters in strings and comments.
        - Avoid Unicode escape sequences.
    - For build and configuration files:
        - Makefiles: Use ISO-8859-1 encoding.
        - Shell scripts: Use ISO-8859-1 encoding with `LANG=C` locale.
        - Configuration files: Save in ISO-8859-1 format.
    - Keep generated code as simple and straightforward as possible.
    - Avoid complex control structures and nested loops.
- For web development, following a mobile-first approach is preferred.
    - But still ensure pages can be responsive on desktops.
- For html web development, like `.html` pages, use `HTML4.01` and and only native `HTML4.01-API`.
    - Use `HTML 4.01 Strict` or `XHTML 1.0` Strict to enforce structural rigor.
- For CSS, use `CSS2.1` and only native `CSS2.1-API`.
    - no libraries or frameworks.
    - all inline and no external css files.
- For JavaScript, use `ES3` and only native `WEB-API`.
    - Strict Mode: Always use `"use strict";` at the beginning of files or functions to enforce stricter parsing and error handling.
    - No external libraries or frameworks.
    - No modern JavaScript features (e.g., ES6+).
    - all inline and no external js files.
- Provide code snippets that can be easily integrated into existing codebases.
- Ensure generated code is **safe**, **efficient**, and **maintainable**.
- Follow the project's **coding standards** and **style guidelines**.
- Only provide the necessary code to fulfill the user's request that is to perform the most basic operations.
- Avoid generating code that is overly verbose or redundant.
- Provide clear and concise comments to explain the purpose of the code.
- Use **consistent naming conventions** for variables, functions, and files.
- Avoid generating code that is overly complex or difficult to understand.
- Ensure that the generated code is **well-documented** and **self-explanatory**.
- Keep generated code **minimal**, **modular** and **reusable**.
- Avoid generating code that is overly specific to a particular use case.
- Ensure that the generated code is **well-structured** and **easy to read**.
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
- Avoid direct type punning to comply with strict aliasing rules.
- Never redefine a variable in the same scope.
- Avoid generating code that are conflicting types for the same variable.
- Avoid previous declarations of variables.
- C90 standard requiring declarations at start of blocks
- ISO C90 forbids mixed declarations and code
- Code must compile with gcc using: `-static -O3 std=c90 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=500 -Wall -ansi -Wextra -pedantic -Werror -Wshadow -Wconversion -Wstrict-prototypes -Wmissing-prototypes -Warray-bounds -Wformat=2 -Wformat-security -Wformat-overflow=2 -Wformat-truncation=2 -Wvla -Wbad-function-cast -Wstrict-aliasing=2 -Wnull-dereference -Wdouble-promotion -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wcast-align -Wcast-qual -Wredundant-decls -Wundef -Wmissing-include-dirs -Winit-self -Wswitch-enum -Wmissing-declarations -Wsign-conversion -fstack-protector-strong -fstack-check -fPIE -fstack-protector-all -fdata-sections -ffunction-sections -fno-common -fstrict-aliasing -fno-strict-overflow -fanalyzer -fno-omit-frame-pointer -finput-charset=iso-8859-1 -fexec-charset=iso-8859-1 -fwide-exec-charset=iso-8859-1`
- Code Safety Requirements:
    - No VLAs (enforced by -Wvla)
    - No array bounds violations (enforced by -Warray-bounds)
    - No stack buffer overflows (enforced by -fstack-protector-strong -fstack-check)
    - No undefined behavior
    - No memory leaks
    - No floating-point issues
- Do not use 'for' loop initial declarations
- Do not create implicit declarations of functions
- Memory Safety:

```c
/* Always initialize pointers */
int *ptr = NULL;

/* Check malloc results */
ptr = malloc(size);
if (ptr == NULL) {
    /* Handle error */
    return -1;
}
```
- Array Safety:

```c
/* Define array size as constant */
#define ARRAY_SIZE 100

/* Use size_t for array indices */
size_t i;
int array[ARRAY_SIZE];

/* Array bounds checking */
for (i = 0; i < ARRAY_SIZE; i++) {
    array[i] = 0;
}
```

- Function Prototypes:

```c
/* Complete prototype with parameter names */
int function_name(int parameter_one, char *parameter_two);

/* Implementation matches exactly */
int
function_name(int parameter_one, char *parameter_two)
{
    /* Function body */
    return 0;
}
```

- Variable Declaration:

```c
void function(void)
{
    /* All declarations at start */
    int i;
    char *ptr = NULL;
    const char *str = "example";

    /* Code follows */
    i = 0;
}
```

- String Handling:

```c
/* Safe string operations */
char buffer[MAX_SIZE];
size_t len;

if (snprintf(buffer, sizeof(buffer), "%s", input) >= sizeof(buffer)) {
    /* Handle truncation */
    return -1;
}
```

- Format String Safety

```c
/* Use format string prototypes */
int print_value(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

/* Check format string usage */
printf("%s", str);  /* correct */
printf(str);        /* wrong - format string vulnerability */
```

- Thread Safety

```c
/* Thread-local storage when needed */
static __thread int thread_local_var;

/* Proper mutex usage */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_lock(&mutex);
/* Critical section */
pthread_mutex_unlock(&mutex);
```

- Error Handling

```c
/* Check system calls */
if (write(fd, buf, len) < 0) {
    /* Handle error with errno */
    return -1;
}

/* Check library functions */
if (pthread_mutex_lock(&mutex) != 0) {
    /* Handle error */
    return -1;
}
```

- Type Safety

```c
/* Use explicit types */
uint32_t unsigned_val;
int32_t signed_val;

/* Type conversion with checks */
if (val > INT_MAX || val < INT_MIN) {
    /* Handle overflow */
    return -1;
}
```

---

## **Code Structure**
- Use only ANSI C headers
- Include POSIX headers with proper guards
- Order: system headers, then local headers
- Use `snake_case` for file names, e.g., `data_manager.c`, `user_auth.h`.

## **FOrmatting Style**
- K&R/GNU style bracing
- Full prototype declarations
- No inline functions (C90 restriction)
- Return type on separate line

## **Variable Declarations**
- All declarations at block start
- One variable per declaration
- No VLAs (C90 restriction)
- No mixed declarations/code

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

## Optimised Directory Structure
```plaintext
/			# . (root directory)
??? bin/                # Essential user binaries
?   ??? sh              # POSIX-compliant shell
?   ??? ls              # List directory contents
?   ??? cp              # Copy files
?   ??? mv              # Move/rename files
?   ??? rm              # Remove files
?   ??? mkdir           # Create directories
?   ??? ...             # Other essential commands
??? sbin/               # Essential system binaries
?   ??? init            # Init system
?   ??? fsck            # Filesystem check
?   ??? mount           # Mount filesystems
?   ??? umount          # Unmount filesystems
?   ??? ...             # Other system utilities
??? lib/                # Shared libraries for essential binaries
?   ??? libc.so         # musl libc
?   ??? libpthread.so   # POSIX threads
?   ??? libm.so         # Math library
?   ??? ...             # Other essential shared libraries
??? etc/                # System configuration files
?   ??? passwd          # User account information
?   ??? group           # Group account information
?   ??? fstab           # Filesystem table
?   ??? hosts           # Hostname resolution
?   ??? ...             # Other configuration files
??? dev/                # Device files
?   ??? null            # Null device
?   ??? zero            # Zero device
?   ??? tty             # Terminal device
?   ??? random          # Random number generator
?   ??? sda             # First disk device
?   ??? ...             # Other device nodes
??? tmp/                # Temporary files
??? var/                # Variable data
?   ??? log/            # Log files
?   ?   ??? syslog      # System log
?   ?   ??? auth.log    # Authentication log
?   ?   ??? ...         # Other log files
?   ??? spool/          # Spool directories
?   ??? run/            # Runtime data
??? usr/                # Secondary hierarchy for user programs
?   ??? bin/            # Non-essential user binaries
?   ??? lib/            # Shared libraries for `/usr/bin`
?   ??? include/        # Header files
?   ??? share/          # Architecture-independent files
?   ?   ??? man/        # Manual pages
?   ?   ??? locale/     # Localization files
?   ??? src/            # Source code (optional)
??? home/               # User home directories
?   ??? user1/          # Example user
?   ??? user2/          # Another user
??? opt/                # Optional software packages
??? mnt/                # Mount points for temporary filesystems
?   ??? usb/            # USB devices
?   ??? cdrom/          # CD-ROM
?   ??? ...             # Other mount points
??? proc/               # Virtual filesystem for processes
??? sys/                # Virtual filesystem for system information
??? boot/               # Bootloader files (if applicable)
    ??? kernel          # Kernel image
    ??? initrd          # Initial RAM disk
    ??? ...             # Other boot files
```

## **1. Essential Components of a POSIX-Compliant Filesystem**
### **Filesystem Hierarchy**
Your filesystem must support the essential directories and their roles as outlined by POSIX and the Filesystem Hierarchy Standard (FHS). Key directories include:

#### **Root Directory (`/`)**
The root directory is the starting point of the filesystem and contains essential subdirectories:
- `/bin`: Essential user binaries (e.g., `ls`, `cp`, `sh`).
- `/sbin`: Essential system binaries (e.g., `init`, `fsck`).
- `/lib`: Shared libraries needed by binaries in `/bin` and `/sbin`.
- `/etc`: System configuration files.
- `/dev`: Device files (e.g., `tty`, `null`, `random`).
- `/tmp`: Temporary files (cleared on reboot).
- `/var`: Variable data (e.g., logs, spools, runtime data).
- `/usr`: Secondary hierarchy for user applications and libraries.
  - `/usr/bin`: Non-essential user binaries.
  - `/usr/lib`: Libraries for `/usr/bin`.
  - `/usr/include`: Header files for development.
  - `/usr/share`: Architecture-independent files (e.g., man pages, locale files).

#### **Optional Directories**
- `/home`: User home directories.
- `/opt`: Optional application software packages.
- `/mnt` or `/media`: Mount points for external filesystems.
- `/proc`: Virtual filesystem for process and system information.
- `/sys`: Virtual filesystem for system hardware and kernel information.

---

## **2. Key Filesystem Components**
To build a fully functional POSIX filesystem, you need to consider the following:

### **a. Kernel Interface**
- Implement system calls for POSIX-compliant operations like file handling (`open`, `read`, `write`, `close`), process control (`fork`, `exec`, `wait`), and inter-process communication (IPC).
- Ensure compatibility with the Virtual File System (VFS) layer if using a Linux kernel.

### **b. Filesystem Implementation**
- Support for at least one filesystem type (e.g., ext4, XFS, or a custom implementation).
- Implement file metadata handling, including permissions, ownership, timestamps, and symbolic/hard links.

### **c. Device Files**
- Provide `/dev` with character and block device nodes for hardware interaction.
- Use `mknod` to create device files with proper major and minor numbers.

### **d. Shared Libraries**
- Provide essential libraries in `/lib` and `/usr/lib`, including:
  - **musl libc**: A lightweight, POSIX-compliant standard C library.
  - **libm**: Math library.
  - **libpthread**: POSIX threads library.

---

## **3. Tools and Utilities**
### **a. Essential Utilities**
Include the POSIX-mandated utilities, such as:
- File management: `ls`, `cp`, `mv`, `rm`, `mkdir`.
- Shell utilities: `sh`, `echo`, `test`, `kill`.
- Text processing: `cat`, `grep`, `sed`, `awk`.
- System management: `ps`, `top`, `df`, `du`, `mount`, `umount`.

### **b. Development Tools**
- A compliant C compiler (e.g., `musl-gcc` or a custom ANSI C compiler).
- Tools for building and debugging, such as `make` and `gdb`.

### **c. Init System**
- Implement a minimal init system (e.g., `init`) to handle system startup and service management.

---

## **4. POSIX Compliance Checklist**
To ensure POSIX compliance:
- **File and Directory Structure**: Verify the existence of mandatory directories.
- **System Calls**: Implement all required POSIX system calls.
- **Threads and Concurrency**: Provide support for POSIX threads (pthreads).
- **Signals and IPC**: Implement signal handling and IPC mechanisms (pipes, message queues, semaphores, shared memory).
- **Filesystem Features**: Support file permissions, ownership, and links.

---

## **5. Standards Compliance**
### **a. C89/C90 Compliance**
- Use `musl libc` or a custom ANSI C library implementation.
- Ensure strict adherence to C89/C90 for the entire codebase.

### **b. X/Open Portability**
- Implement the X/Open Portability Guide (XPG) features, including extended file attributes, locale support, and terminal interfaces.

---

## **6. Build and Deployment**
### **a. Static Build**
- Ensure all binaries are statically linked to avoid runtime dependencies.

### **b. Cross-Platform Compatibility**
- Use tools like `musl-gcc` to create cross-platform binaries.
- Test on multiple architectures (e.g., x86, ARM).

### **c. Build Tools**
- Use a portable build system (e.g., `make`) to compile the filesystem and utilities.

---

## **7. Documentation and Testing**
### **a. Documentation**
- Provide detailed documentation for the filesystem structure and utilities in Markdown.
- Include man pages for all commands and libraries.

### **b. Testing**
- Write unit tests for all libraries and utilities.
- Use test suites like `POSIX Test Suite (PTS)` to validate compliance.


### **Error Handling**
- Use errno for error reporting
- Check all system call returns
- No exceptions (C90)
- No goto (except error cleanup)
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
- **Compiler**: Target **x86_64-linux-musl (gcc)** with the following capabilities:
    - Use `-ansic` and `-std=c90` for ISO C compliance.
    - Define `_POSIX_C_SOURCE=1` and `_XOPEN_SOURCE=500` for POSIX and X/Open features supported by musl.
    - Avoid advanced GNU and GCC-specific extensions unsupported by musl.
- **Libraries**:
    - Base code on **musl libc** for portability and minimalism.
    - Ensure compatibility with musl's threading and math libraries.
- **Containerized Development**:
    - Target **AMD64** architecture with static linking for compatibility across container instances.
    - Optimize for minimal dependencies, leveraging musl?s built-in features where possible.

## **Compiler Flags Reference**
### Production Build (musl-gcc because it runs on Trisquel)
```c
CFLAGS = -std=c90 -D_POSIX_C_SOURCE=1 -D_XOPEN_SOURCE=500 -Wall -Wextra -pedantic -static
LDFLAGS = -static -pthread -Wl,-O1 -Wl,--gc-sections -lssl -lcrypto
```

```c
### **Development Build (gcc as it ise alpine)**
CFLAGS = -std=c90 -D_POSIX_C_SOURCE=1 -D_XOPEN_SOURCE=500 -Wall -Wextra -pedantic -O0 -ggdb3
LDFLAGS = -static -pthread -lssl -lcrypto
```
/lib/ld-musl-x86_64.so.1: This is the dynamic linker (or loader) for musl libc, which is responsible for loading shared libraries and starting the program.

/lib/libc.musl-x86_64.so.1: This is the musl C library itself, which provides the standard C library functions (e.g., malloc, printf, etc.).

## Explicit code block examples:
## **Code Structure Examples**

### Header File Template (.h)
```c
/* filepath: include/module_name.h */
#ifndef MODULE_NAME_H
#define MODULE_NAME_H

/* POSIX headers */
#include <sys/types.h>
#include <pthread.h>

/* Constants */
#define MODULE_MAX_SIZE 1024

/* Data structures */
struct module_data {
    int id;
    char name[MODULE_MAX_SIZE];
};src/mime.c: In function 'mimeAddType':
src/mime.c:88:5: error: ISO C90 forbids mixed declarations and code [-Werror=declaration-after-statement]
   88 |     size_t i;
      |     ^~~~~~
/* Function prototypes */
int moduleFunction(const struct module_data *data);

#endif /* MODULE_NAME_H */
```

### Source File Template (.c)
```c
/* filepath: src/module_name.c */
#include "module_name.h"
#include <stdlib.h>
#include <string.h>

int moduleFunction(const struct module_data *data)
{
    if (data == NULL)
        return -1;
    return 0;
}
```

## Example Compliant COde
```c
/* filepath: example.h */
#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <sys/types.h>
#include <unistd.h>

/* Constants */
#define MAX_BUF 1024

/* Types */
struct data_block {
    int id;
    char buf[MAX_BUF];
};

/* Function prototypes */
int
process_data(struct data_block *block);

#endif /* EXAMPLE_H */
```
```c
/* filepath: example.c */
#include "example.h"
#include <errno.h>
#include <stdlib.h>

int
process_data(struct data_block *block)
{
    int result;
    char *temp;

    if (block == NULL) {
        errno = EINVAL;
        return -1;
    }

    temp = malloc(MAX_BUF);
    if (temp == NULL) {
        return -1;
    }

    /* Process data */
    result = do_work(temp, block->buf);

    free(temp);
    return result;
}
```

## Build target clarity:
```markdown
## **Build Targets**
- `make ENV=dev` - Development build with debug symbols
- `make ENV=prod` - Production build with optimizations
- `make install` - Install based on ENV
- `make clean` - Clean build artifacts
```
---

## **Testing and Validation**
- Write unit tests using CUNit framework for all functions using the following naming conventions:
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
- CUnit framework for testing
- Use CI/CD pipelines to automate testing and code quality checks.
- Regularly review and refactor code for maintainability and performance.

## Copilot Usage Guidelines
- Focus on providing code snippets that can be copied into existing files rather than rewriting entire files.
- Ensure that the generated snippets adhere to the project's coding standards and practices as outlined in this document.
- Provide context-specific suggestions that integrate seamlessly with the existing codebase.

Here?s a comprehensive list of the **POSIX-compliant** and **ISO C89 (ANSI C)** headers and libraries provided by **musl libc**. These libraries and headers are essential for building a project that adheres to your requirements for compliance, portability, and lightweight environments.

---

## **ISO C89 Standard Headers**
These headers are defined by the **ISO/IEC 9899:1990 (C89)** standard and are fully supported by musl libc.

### **Standard Library Headers**
| Header         | Description                                                                 |
|----------------|-----------------------------------------------------------------------------|
| `<assert.h>`   | Diagnostics: `assert` macro for runtime error checking.                    |
| `<ctype.h>`    | Character classification and conversion functions.                        |
| `<errno.h>`    | Error codes used by library functions.                                     |
| `<float.h>`    | Defines limits for floating-point types.                                   |
| `<limits.h>`   | Defines limits for integral types.                                         |
| `<locale.h>`   | Localization functions and macros.                                         |
| `<math.h>`     | Mathematical functions.                                                   |
| `<setjmp.h>`   | Non-local jumps (e.g., `setjmp` and `longjmp`).                            |
| `<signal.h>`   | Signal handling.                                                          |
| `<stdarg.h>`   | Variable argument list handling (`va_list`, `va_start`, etc.).            |
| `<stddef.h>`   | Common definitions (e.g., `size_t`, `NULL`, `offsetof`).                  |
| `<stdio.h>`    | Input/output functions (e.g., `printf`, `fopen`, `fread`).                |
| `<stdlib.h>`   | General utilities (e.g., memory management, random numbers).              |
| `<string.h>`   | String handling and memory manipulation functions.                        |
| `<time.h>`     | Time and date functions.                                                  |
| `<wchar.h>`    | Wide-character functions.                                                 |
| `<wctype.h>`   | Wide-character classification and conversion functions.                   |

---

## **POSIX Standard Headers**
Defined by **POSIX.1 (IEEE Std 1003.1)**, these headers extend the functionality of the ISO C standard and are fully supported by musl libc.

### **System Interfaces**
| Header           | Description                                                                 |
|------------------|-----------------------------------------------------------------------------|
| `<aio.h>`        | Asynchronous I/O.                                                         |
| `<dirent.h>`     | Directory entry manipulation (`opendir`, `readdir`, etc.).                |
| `<fcntl.h>`      | File control options (`open`, `fcntl`).                                   |
| `<grp.h>`        | Group database access (`getgrgid`, `getgrnam`).                           |
| `<pwd.h>`        | User database access (`getpwuid`, `getpwnam`).                            |
| `<sys/stat.h>`   | File status (`stat`, `fstat`, etc.).                                       |
| `<sys/types.h>`  | Data types used in system calls (`pid_t`, `off_t`, etc.).                 |
| `<sys/wait.h>`   | Process termination (`wait`, `waitpid`).                                  |
| `<termios.h>`    | Terminal I/O interfaces.                                                  |
| `<unistd.h>`     | POSIX API for system services (`fork`, `exec`, `read`, `write`, etc.).    |

### **Signals and Threads**
| Header           | Description                                                                 |
|------------------|-----------------------------------------------------------------------------|
| `<pthread.h>`    | POSIX threads (Pthreads) interface.                                        |
| `<signal.h>`     | Signal handling and manipulation.                                          |
| `<sys/signal.h>` | Signal-related constants and macros (alias of `<signal.h>`).               |

### **Networking and IPC**
| Header           | Description                                                                 |
|------------------|-----------------------------------------------------------------------------|
| `<arpa/inet.h>`  | Internet operations (`inet_ntoa`, `inet_addr`).                            |
| `<netdb.h>`      | Network database operations (`gethostbyname`, `getaddrinfo`).              |
| `<netinet/in.h>` | IPv4 and IPv6 definitions.                                                 |
| `<sys/socket.h>` | Socket programming interface.                                              |
| `<sys/un.h>`     | UNIX domain sockets.                                                      |
| `<sys/ipc.h>`    | Interprocess communication (IPC) definitions.                              |
| `<sys/msg.h>`    | Message queue operations.                                                 |
| `<sys/sem.h>`    | Semaphore operations.                                                     |
| `<sys/shm.h>`    | Shared memory operations.                                                 |

### **Advanced File I/O**
| Header           | Description                                                                 |
|------------------|-----------------------------------------------------------------------------|
| `<sys/mman.h>`   | Memory management (`mmap`, `munmap`).                                      |
| `<sys/statvfs.h>`| Filesystem statistics (`statvfs`, `fstatvfs`).                             |
| `<utime.h>`      | File access and modification times.                                        |

---

## **Additional POSIX and X/Open 500 Headers**
These headers are part of the **X/Open 500** standard, which extends POSIX functionality.

| Header           | Description                                                                 |
|------------------|-----------------------------------------------------------------------------|
| `<regex.h>`      | Regular expression handling.                                               |
| `<fnmatch.h>`    | Filename matching (`fnmatch`).                                             |
| `<tar.h>`        | Tar file format definitions.                                               |
| `<fts.h>`        | File traversal utility functions.                                          |
| `<libgen.h>`     | Path and filename manipulation (`basename`, `dirname`).                   |
| `<utmp.h>`       | User accounting database.                                                 |
| `<wordexp.h>`    | Word expansion (e.g., globbing).                                           |

---

## **Math and Floating-Point Extensions**
| Header           | Description                                                                 |
|------------------|-----------------------------------------------------------------------------|
| `<fenv.h>`       | Floating-point environment control.                                        |
| `<tgmath.h>`     | Type-generic macros for mathematical functions.                           |

---

## **musl-Specific Features**
While musl libc is designed to strictly follow standards, it also includes enhancements for better performance and compatibility:
- **Static linking support**: Musl is optimized for creating fully static binaries.
- **Thread safety**: Full support for POSIX threads (Pthreads).
- **Lightweight implementations**: Many functions are rewritten to be smaller and faster compared to glibc.

---

## **Additional Web Development Standards**

### **HTML 4.01 Guidelines**
- Use `<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">`.
- Include ISO-8859-1 meta tag: `<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">`.
- Forms:
    ```html
    <form method="POST" action="...">
    <form method="GET" action="...">
    ```
- Tables for layout (no CSS flexbox/grid).
- No HTML5 elements or attributes.

### **CSS 2.1 Guidelines**
- Use only CSS 2.1 properties and selectors.
- Avoid modern features:
    - No `@font-face`
    - No flexbox/grid
    - No `calc()`
    - No transforms/animations
- Use browser-safe fonts.
- Tables for complex layouts.

### **ES3 JavaScript Guidelines**
```javascript
/* Use var, not let/const */
var x = 5;

/* Use function declarations */
function myFunction(param) {
        var result = 0;
        return result;
}

/* No arrow functions or modern features */
```

### **Project Structure**
```plaintext
www/
??? views/               # HTML templates
??? static/             # Static assets
?   ??? js/            # Minified JavaScript
?   ??? css/           # Minified CSS  
?   ??? images/        # Image assets
??? routes/            # Route handlers
```

### **File Encoding**
- All web files must use ISO-8859-1 encoding.
- Avoid UTF-8 or Unicode characters.
- Use HTML entities for special characters.

### **Configuration Files**
- `web.config`: Server configuration
- `robots.txt`: Search engine directives
- `manifest.json`: Web app manifest
- `sitemap.xml`: Site structure

### **Security Requirements**
- Validate all form input.
- Use proper HTML escaping.
- Follow secure cookie practices.
- Implement proper access controls.

### **Testing Standards**
- Unit test all JavaScript functions.
- Validate HTML/CSS against W3C standards.
- Test in legacy browsers.
- Verify ISO-8859-1 compatibility.

### **File Tree**
```plaintext
.                            # Project root
.editorconfig                # Editor configuration file
.gitattributes               # Git attributes file
.gitignore                   # Git ignore file
.gitmodules                  # Git submodules file
LICENSE                      # License must stay in root
README.md                    # Quick start guide stays in root
CHANGELOG.md                 # Version history stays in root
Makefile                     # Build file stays in root
VERSION                      # Version info stays in root
web-app.code-workspace       # VS Code workspace file
.github/                     # GitHub-specific configuration
    copilot-instructions.md  # Instructions for GitHub Copilot
    ISSUE_TEMPLATE/          # Issue templates
        feature_request.md   # Feature request template
        bug_report.md        # Bug report template
    workflows/               # GitHub Actions workflows
        build.yml            # Build automation workflow
    PULL_REQUEST_TEMPLATE.md # Pull request template
    dependabot.yml           # Dependabot configuration
    FUNDING.yml              # Sponsorship information
    CODEOWNERS               # Code owners for review
.vscode/                     # VS Code-specific configuration
    extensions.json          # Recommended extensions
    launch.json              # Debug configurations
    tasks.json               # Task configurations
    settings.json            # Editor settings
.devcontainer/               # Development container configuration
    devcontainer.json        # Container settings
    Containerfile            # Container setup
archive/                     # Archive root directory
    releases/                # Version-controlled archives
        app-1.2.3.tar.gz     # Major.Minor.Patch version
        app-1.2.4.tar.gz     # Patch update version
        app-2.0.0.tar.gz     # Major version update
    by-date/                 # Date-based archives
        app-20240314.tar.gz  # Daily archive (YYYYMMDD)
        app-202403.tar.gz    # Monthly archive (YYYYMM)
        app-2024Q1.tar.gz    # Quarterly archive
    deprecated/              # Deprecated code archive
        feature-v1-deprecated.c # Deprecated features
        module-old-202403.c  # Date-tagged old code
backup/                      # Backup directory
    daily/                   # Daily backups
        app-20240314.tar.gz  # Full application backup
        db-20240314.sql.gz   # Database backup
    weekly/                  # Weekly backups
        app-2024W11.tar.gz   # Week 11 backup
    monthly/                 # Monthly backups
        app-202403.tar.gz    # March 2024 backup
    config/                  # Configuration backups
        web.conf.bak         # Simple backup
        web.conf.1           # Versioned backup
        web.conf.old         # Previous version
    pre-update/              # Pre-update backups
        app-1.2.3-pre.tar.gz # Pre-update app backup
        config-1.2.3-pre.tar.gz # Pre-update config
bin/                         # Executable files and scripts
    enssol                   # Main application binary
build/                       # Build output directories
    debug/                   # Debug build files
        deps/                # Dependency files (e.g., .d files)
            placeholder.d
        obj/                 # Object files for debug build
            placeholder.o
            placeholder.s
    release/                 # Release build files
        deps/                # Dependency files for release
            placeholder.d
        obj/                 # Object files for release build
            placeholder.o
            placeholder.s
    dist/                    # Distribution files (static libraries, etc.)
        placeholder.a
checksums/                   # Directory for checksum files
    release.sha256           # SHA-256 checksum for release files
    release.md5              # MD5 checksum for release files
    libs.sha256              # SHA-256 checksum for libraries
config/                      # Configuration files and settings
    app/                     # Application configuration
        app.env              # Environment variables
        app.conf            # Core app settings (INI-style)
        user.conf           # User preferences (INI-style)
    auth/                    # Authentication configuration
        passwd              # User accounts (Unix standard) 
        shadow              # Password hashes (Unix standard)
    web/                     # Web server configuration
        web.conf            # Main web server config
        robots.txt          # Search engine directives
        web.xml             # Web app manifest (XML format)
        sitemap.xml         # Site structure for SEO
    security/               # Security settings
        acl.conf            # Access Control Lists
        auth.conf           # Authentication rules
        ssl.conf            # SSL/TLS configuration 
        headers.conf        # Security headers
        csp.conf            # Content Security Policy
    build/                  # Build configurations
        host.h              # Host-specific config
        config.h            # Main config header
        machine.h           # Machine settings
        gcc.specs           # GCC spec file for build configuration
    mkconfig/               # Config generation
        config.h.in         # Header template
        mkconfig.sh         # Generation script
        config-test.h       # Test configuration
    deps.txt                # Project dependencies
    .well-known/            # Standard URIs
        acme-challenge/     # ACME verification
            token           # Challenge token
            response        # Challenge response
    security.txt            # Security info
    weblinks.txt            # App links (plain text)
charset/                     # Character encoding configs
    iso-8859-1.map           # Maps ISO-8859-1 characters to their binary values
docs/                        # Project documentation root
    api/                     # API documentation directory
        API.md               # Complete API reference and documentation
    user/                    # End-user documentation directory
        USER_GUIDE.md        # Complete guide for end users
        INSTALL.md           # Installation instructions
        FAQ.md               # Frequently asked questions
        SUPPORT.md           # Support information and contact details
    dev/                     # Developer documentation directory
        DEVELOPER_GUIDE.md   # Guide for developers
        CONTRIBUTING.md      # Guidelines for contributing
        HACKING              # Technical details (plain text)
        EXAMPLES.md          # Code examples and usage samples
    project/                 # Project management documentation
        AUTHORS.md           # List of project authors and contributors
        MAINTAINERS          # Current project maintainers (plain text)
        ROADMAP.md           # Future development plans
        TODO.md              # Pending tasks and features
        NEWS                 # Project news and release notes (plain text)
    security/                # Security documentation
        SECURITY.md          # Security policies and procedures
        CODE_OF_CONDUCT.md   # Project code of conduct
    technical/               # Technical documentation (plain text)
        project-index.txt    # Index of project documentation
        project-structure.txt # Details of project architecture
        specfile.spec        # Project specifications
        DOCUMENTATION.md     # General technical documentation
include/                     # Header files for C code
    core/                    # Core server functionality headers
        server_core.h        # Server implementation header
        config_core.h        # Configuration handling header
    http/                    # HTTP protocol headers
        http_request_header_parser.h  # Parses HTTP request headers
        http_request_body_parser.h    # Parses HTTP request body
        http_response_header_builder.h # Constructs HTTP response headers
        http_response_body_builder.h  # Constructs HTTP response body
        http_route_matcher.h          # Matches HTTP requests to routes
    compat/                  # POSIX compatibility headers
        posix_compat.h       # POSIX compatibility declarations
        system_compat.h      # Compatibility functions
        ctype_compat.h       # Compatibility for character functions
        io_compat.h          # Compatibility for I/O functions
        legacy_compat.h      # Legacy compatibility functions
        pthread_compat.h     # Compatibility for POSIX threads
        signal_compat.h      # Compatibility for signal handling
        struct_compat.h      # Compatibility for data structures
        sys_compat.h         # Compatibility for system calls
        time_compat.h        # Compatibility for time functions
    main.h                   # Main entry point for the application
    constant.h               # Global constants header
    struct.h                 # Structure definitions header
    standards.h              # Standard headers inclusion
    common.h                 # Common functions and macros
    data_operations.h        # Implements functions to manipulate data structures
    data_manager.h           # Manages collections of data
    error_handling/          # Error handling headers
        error_logger.h           # Logs error messages to file or console
        error_reporter.h         # Reports errors to monitoring systems
        error_code_mapper.h      # Maps error codes to human-readable messages
        error_handler.h          # Provides centralized error handling logic
        error_validator.h        # Validates input data to prevent common errors
    validation/             # Validation headers
        input_validator.h        # Validates user input for correctness
        file_signature_verifier.h # Verifies digital signatures on files
        data_integrity_verifier.h # Checks data consistency and integrity
    memory_management/       # Memory management headers
        pointer_allocator.h      # Manages dynamic memory allocation for pointers
        buffer_manager.h         # Provides buffer creation, resizing, and deletion
        pointer_safety_checker.h # Checks for null and invalid pointers
    threading/              # Thread management headers
        thread_manager.h         # Manages thread creation, synchronization, and cleanup
        mutex_locker.h           # Provides functions to lock/unlock POSIX mutexes
        condition_variable.h     # Implements POSIX condition variable support
        thread_safe_queue.h      # Implements thread-safe queue operations
    io/                     # Input/Output headers
        io_file_reader.h         # Reads data from files efficiently
        io_file_writer.h         # Writes data to files efficiently
        io_socket_handler.h      # Handles socket-level I/O operations
    cache/                  # Cache management headers
        cache_initializer.h      # Initializes cache data structures
        cache_updater.h          # Updates cache entries based on new data
        cache_eviction.h         # Implements cache eviction policies (e.g., LRU)
    garbage_collection/     # Garbage collection headers
        garbage_collector.h      # Manages cleanup of unused memory regions
        object_ref_counter.h     # Tracks object references for manual garbage collection
    system/                 # System initialization and cleanup headers
        system_initializer.h     # Performs system-wide initialization tasks
        module_cleanup.h         # Cleans up resources during module shutdown
    documentation/          # Documentation generation headers
        doc_generator.h          # Generates documentation from source code comments
        man_page_builder.h       # Builds UNIX-style manual pages from templates
    scripting/              # Script execution and validation headers
        script_executor.h        # Executes custom scripts securely
        script_validator.h       # Validates the syntax of user-provided scripts
    permissions/            # Permission management headers
        permission_checker.h     # Verifies if a user has the required permissions
        access_control_list.h    # Manages access control lists for resources
    authentication/         # User authentication headers
        user_authenticator.h     # Authenticates users based on credentials
        auth_token_generator.h   # Generates secure authorization tokens
        auth_token_validator.h   # Validates the integrity and authenticity of authorization tokens
        auth_policy_enforcer.h   # Enforces authorization policies
        auth_access_control.h    # Manages access control based on authorization tokens and policies
        auth_token_revoker.h     # Handles token revocation and invalidation
        auth_policy_loader.h     # Loads and parses authorization policy configurations
        auth_audit_logger.h      # Logs authorization events for auditing purposes
        auth_session_manager.h   # Manages user sessions related to authorization
    encryption/             # Data encryption and decryption headers
        aes_encryptor.h          # Encrypts data using AES
        aes_decryptor.h          # Decrypts data encrypted with AES
        rsa_key_manager.h        # Manages RSA keys for encryption and decryption
    encoding/               # Data encoding and decoding headers
        iso_8859_1_encoder.h     # Encodes data into ISO-8859-1 format
        iso_8859_1_decoder.h     # Decodes ISO-8859-1 encoded data
    recutils/               # Recutils file operations headers
        rec_file_reader.h        # Contains functions for reading records from Recutils files
        rec_file_writer.h        # Contains functions for writing data to Recutils files
        rec_record_parser.h      # Functions for parsing individual records
        rec_record_validator.h   # Functions for validating the integrity of records
        rec_metadata_extractor.h # Functions for extracting metadata from Recutils files
        rec_metadata_updater.h   # Functions for updating metadata within Recutils files
        rec_filter_parser.h      # Functions for parsing filter expressions to search records
        rec_searcher.h           # Functions for searching and filtering records based on specific criteria
        rec_importer.h           # Functions for importing records from external sources into Recutils files
        rec_exporter.h           # Functions for exporting records from Recutils files to other formats
        rec_data_transformer.h   # Functions for transforming data within Recutils files (e.g., converting data types, formatting)
        rec_config_loader.h      # Functions for loading configuration settings related to Recutils file operations
        rec_file_initializer.h   # Functions for initializing Recutils files with required structure and metadata
        rec_error_logger.h       # Functions for logging errors related to Recutils operations
        rec_error_handler.h      # Functions for handling errors and exceptions during Recutils file processing
        rec_backup_manager.h     # Functions for backing up Recutils files
        rec_restore_manager.h    # Functions for restoring Recutils files from backups
        rec_compressor.h         # Functions for compressing Recutils files for storage
        rec_decompressor.h       # Functions for decompressing Recutils files for access
    archive/                # Archive management headers
        archive_version_fetcher.h # Retrieves archive version metadata
        archive_version_format_validator.h # Validates version format
        archive_version_comparator.h # Compares archive versions
        archive_date_parser.h    # Parses date strings into structured formats
        archive_date_format_validator.h # Ensures date format validity
        archive_date_comparator.h # Compares archive dates
        archive_deprecated_marker.h # Marks deprecated archives
        archive_deprecation_checker.h # Checks if an archive is deprecated
    backup/                 # Backup management headers
        backup_metadata_creator.h # Creates metadata for backups
        backup_metadata_validator.h # Validates backup metadata
        backup_metadata_updater.h # Updates backup metadata
        backup_file_packer.h     # Packs files into a backup
        backup_file_unpacker.h   # Unpacks files from a backup
        backup_file_compressor.h # Compresses backup files
        backup_file_decompressor.h # Decompresses backup files
        backup_integrity_checker.h # Verifies backup integrity
        backup_integrity_reporter.h # Reports integrity status
    checksum/               # Checksum calculation and verification headers
        checksum_sha256_calculator.h # Computes SHA-256 checksum
        checksum_sha256_verifier.h # Verifies SHA-256 checksum
        checksum_md5_calculator.h # Computes MD5 checksum
        checksum_md5_verifier.h  # Verifies MD5 checksum
        checksum_file_reader.h   # Reads file for checksum calculation
        checksum_file_writer.h   # Writes checksum to file
    configuration/          # Configuration management headers
        config_key_reader.h      # Reads configuration key
        config_key_writer.h      # Writes configuration key
        config_file_reader.h     # Reads configuration file
        config_file_writer.h     # Writes to configuration file
        config_settings_loader.h # Loads configuration settings
        config_settings_saver.h  # Saves configuration settings
    logging/                # Logging headers
        log_writer.h             # Writes logs to a specified log file or console
        log_reader.h             # Reads logs from a file or buffer
        log_formatter.h          # Formats log messages for consistent output
        log_rotator.h            # Rotates log files based on size or time
        error_log_writer.h       # Writes error messages to the log
        error_log_reader.h       # Reads error logs for analysis
        error_log_formatter.h    # Formats error messages for logging
    monitoring/             # System monitoring headers
        monitor_service.h        # Monitors system health and resource usage
        alert_manager.h          # Sends alerts based on system monitoring thresholds
        event_notifier.h         # Notifies the system or users of significant events
    network/                # Network management headers
        socket_creator.h         # Creates and manages network sockets
        socket_listener.h        # Listens for incoming network connections
        socket_connector.h       # Connects to remote servers via sockets
        socket_communicator.h    # Sends and receives data over sockets
        http_client.h            # Implements HTTP client functionality for sending requests
        http_server.h            # Implements HTTP server functionality for handling requests
        http_router.h            # Routes incoming HTTP requests to the appropriate handlers
        http_request_handler.h   # Handles incoming HTTP requests
        http_response_builder.h  # Builds HTTP responses for the server
        ssl_compat.h             # Provides compatibility for SSL/TLS protocols
        firewall_manager.h       # Manages network firewall rules
        network_monitor.h        # Monitors network traffic for security and performance
    database/               # Database management headers
        db_connection.h          # Manages database connections
        db_query_executor.h      # Executes SQL queries against a database
        db_result_parser.h       # Parses database query results
        db_insert.h              # Handles database record insertion
        db_update.h              # Handles database record updates
        db_delete.h              # Handles database record deletion
        db_transaction.h         # Manages database transactions
        db_migration.h           # Handles database schema and data migrations
        db_backup_manager.h      # Manages database backups
        db_restore_manager.h     # Restores database from backups
        db_encryption.h          # Encrypts sensitive data in the database
        db_access_control.h      # Manages access control for database operations
    file_system/            # File system operations headers
        file_reader.h            # Reads files from the file system
        file_writer.h            # Writes data to files on the file system
        file_deleter.h           # Deletes files from the file system
        file_renamer.h           # Renames files on the file system
        file_permission_checker.h # Checks file permissions
        file_permission_setter.h # Modifies file permissions
        file_owner_setter.h      # Changes the owner of a file
        dir_creator.h            # Creates directories in the file system
        dir_remover.h            # Removes directories from the file system
        dir_list_reader.h        # Reads the contents of a directory
        file_compressor.h        # Compresses files into archives
        file_decompressor.h      # Decompresses files from archives
        file_encryption.h        # Encrypts files before storing or transmitting
        file_decryption.h        # Decrypts files after retrieval
    system_info/            # System information headers
        system_info_reader.h     # Retrieves system information such as CPU, memory, etc.
        system_health_checker.h  # Monitors system health and resource usage
        system_shutdown_manager.h # Handles system shutdown or restart procedures
    process_management/     # Process management headers
        process_manager.h        # Manages processes and their execution
        process_monitor.h        # Monitors the status of running processes
        process_killer.h         # Terminates processes based on conditions
    sys_config/             # System configuration headers
        sys_config_loader.h      # Loads system configuration files
        sys_config_updater.h     # Updates system configuration settings
        sys_config_validator.h   # Validates system configuration files for correctness
    util/                       # Utility headers
        string_utils.h           # String manipulation functions
        date_utils.h             # Date and time manipulation functions
        file_utils.h             # File system utility functions
        memory_utils.h           # Memory management functions
        network_utils.h          # Network utility functions
        system_utils.h           # System utility functions
        crypto_utils.h           # Cryptographic utility functions
        data_utils.h             # Data manipulation functions
        validation_utils.h       # Data validation functions
        logging_utils.h          # Logging utility functions
        monitoring_utils.h       # System monitoring utility functions
        security_utils.h         # Security utility functions
        config_utils.h           # Configuration utility functions
        backup_utils.h           # Backup utility functions
        checksum_utils.h         # Checksum utility functions
        archive_utils.h          # Archive utility functions
        process_utils.h          # Process management utility functions
        sys_config_utils.h       # System configuration utility functions
        util.h                   # General utility functions
lib/                         # Libraries for the project
    pkgconfig/               # Package configuration files
        libexample.pc        # Example package config file
    static/                  # Static libraries
        placeholder.a        # Example static library
man/                         # Unix manual pages
    man1/                    # User commands
        webapp.1             # Web application user manual
    man3/                    # Library functions
        libwebapp.3          # Web application library functions
    man5/                    # File formats
        webapp.conf.5        # Configuration file format
public/                      # Public web root
    index.html              # Main entry point
    assets/                 # Core assets
        images/             # Image assets by type
            interface/      # Interface elements
                buttons/    # Button elements
                    standard/
                        std-normal.gif
                        std-hover.gif
                        std-active.gif
                        std-disabled.gif
                    interactive/
                        int-normal.gif
                        int-hover.gif
                        int-active.gif
                        int-disabled.gif
                symbols/    # Symbol elements
                    navigation/
                        nav-home.gif
                        nav-about.gif
                        nav-products.gif
                        nav-contact.gif
                    indicators/
                        ind-success.gif
                        ind-error.gif
                        ind-warning.gif
                        ind-info.gif
                    operations/
                        op-edit.gif
                        op-delete.gif
                        op-save.gif
                        op-cancel.gif
                structural/     # Structural images
                    headerarea/
                        hdr-default.gif
                        hdr-alternate.gif
                        hdr-print.gif
                    mainarea/
                        main-default.gif
                        main-alternate.gif
                    footerarea/
                        ftr-default.gif
                        ftr-alternate.gif
                formcontrols/  # Form controls
                    checkboxes/
                        chk-empty.gif
                        chk-marked.gif
                        chk-inactive.gif
                    radiobuttons/
                        rad-empty.gif
                        rad-marked.gif
                        rad-inactive.gif
                    dropdowns/
                        dd-arrow.gif
                        dd-inactive.gif
                ornaments/      # Visual elements
                    dividers/
                        div-horizontal.gif
                        div-vertical.gif
                    markers/
                        mrk-primary.gif
                        mrk-secondary.gif
                    directional/
                        dir-next.gif
                        dir-prev.gif
                        dir-up.gif
                        dir-down.gif
            media/              # Content images
                companyinfo/
                    team-photo.jpg
                    office-view.jpg
                    mission-graphic.jpg
                catalog/
                    item1/
                        item1-preview.jpg
                        item1-large.jpg
                        item1-specs.jpg
                    item2/
                        item2-preview.jpg
                        item2-large.jpg
                        item2-specs.jpg
                promotional/
                    homepage/
                        promo-desktop.jpg
                        promo-mobile.jpg
                    seasonal/
                        season-spring.jpg
                        season-summer.jpg
                        season-autumn.jpg
                        season-winter.jpg
                identity/       # Brand assets
                    brandmarks/
                        logo-primary.gif
                        logo-print.gif
                        logo-compact.gif
                    siteicons/
                        icon-default.ico
                        icon-small.ico
                        icon-large.ico
    views/                  # All HTML views/pages
        pages/              # Dynamic pages
            w6946.html         # W6946 project page
            scjv.html          # SCJV project page  
            ms1180.html        # MS1180 project page
            profile.html       # User profile page
            dashboard.html     # Main dashboard
            audit.html         # Audit log viewer
            crud_profile.html  # User profile management
            crud_scjv.html     # SCJV records management
            crud_ms1180.html   # MS1180 records management 
            crud_w6946.html    # W6946 records management
        authentication/  # Authentication
            signin/
                signin-form.html
                signin-success.html
                signin-error.html
                password-reset.html
            signup/
                signup-form.html
                signup-success.html
                signup-verify.html
            signout/
                signout-success.html
                signout-error.html
        exceptions/      # Error states
            badrequest/
                badrequest-main.html
                badrequest-info.html
            notfound/
                notfound-main.html
                notfound-search.html
            servererror/
                servererror-main.html
                servererror-retry.html
        inputs/          # Form templates
            inquiries/
                contact-basic.html
                contact-support.html
                contact-quote.html
            searching/
                search-basic.html
                search-advanced.html
                search-results.html
        sections/        # Content pages
            aboutus/
                aboutus-main.html
                aboutus-team.html
                aboutus-location.html
            merchandise/
                merch-list.html
                merch-detail.html
                merch-compare.html
            offerings/
                offer-list.html
                offer-detail.html
                offer-pricing.html
        shared/          # Shared components
            pageheader/
                header-minimal.html
                header-full.html
                header-print.html
            pagefooter/
                footer-minimal.html
                footer-full.html
                footer-print.html
            wayfinding/
                nav-primary.html          # Primary navigation component
                nav-secondary.html        # Secondary navigation component
                nav-breadcrumb.html       # Breadcrumb navigation
            api/                          # Native Web API implementations
                dom/                      # DOM API components [DOM API]
                    dom-utils.html        # Core DOM manipulation utilities
                    element-creator.html  # Element creation functions
                    element-finder.html   # Element query utilities
                window/                   # Window API components [Window Object]
                    alert-manager.html    # Alert/confirm/prompt handlers
                    timer-manager.html    # setTimeout/setInterval utilities
                navigation/               # Navigation API components [Navigator/Location/History]
                    history-handler.html  # History manipulation
                    location-handler.html # URL/location utilities
                    navigator-detect.html # Browser/OS detection
                events/                   # Event API components [Event Handling]
                    event-handler.html    # Event binding utilities
                    ie-compat.html        # IE compatibility layer
                cssom/                   # CSSOM API components [CSSOM]
                    style-manager.html    # Style manipulation
                    class-manager.html    # Class handling
            utilities/                    # Utility components
                validation/              # Input validation [DOM]
                    form-validator.html   # Form validation logic
                    type-checker.html     # Data type validation
                compatibility/           # Browser compatibility
                    browser-detect.html   # Feature detection
                    ie-fallback.html      # IE fallback handlers
            patterns/                     # Reusable patterns
                forms/                    # Form patterns [DOM]
                    validated-input.html  # Input validation
                    validated-form.html   # Form validation
                interface/               # UI patterns [DOM/Window]
                    modal-dialog.html     # Modal dialogs
                    loading-indicator.html # Loading states
        structures/      # Layout templates
            foundation/
                layout-default.html
                layout-minimal.html
                layout-print.html
            columns/
                col-single.html
                col-double.html
                col-triple.html
        elements/        # Reusable components
            notifications/
                notify-info.html
                notify-success.html
                notify-warning.html
                notify-error.html
            actiontriggers/
                btn-standard.html
                btn-special.html
                btn-hyperlink.html
            inputfields/
                field-text.html
                field-select.html
                field-check.html
                field-radio.html
            itemization/
                list-bullet.html
                list-numbered.html
                list-definition.html
            datatables/
                tbl-simple.html
                tbl-data.html
                tbl-pricing.html
    content/            # Static data
        menuitems/      # Navigation data
            menu-main.txt
            menu-footer.txt
            menu-sitemap.txt
        notifications/  # System messages
            msg-error.txt
            msg-success.txt
            msg-info.txt
        items/          # Content data
            item-products.txt
            item-services.txt
            item-faq.txt
        descriptors/    # Metadata
            meta-pages.txt
            meta-images.txt
            meta-files.txt
        config/             # New configuration directory
            forms/
                field-definitions.txt     # Field types and attributes
                validation-rules.txt      # Input validation rules
                form-states.txt           # Form state definitions
            ui/
                state-mappings.txt        # UI state configurations  
                component-states.txt      # Component state definitions
                layout-configs.txt        # Layout configurations
            routing/
                route-mappings.txt        # URL to view mappings
                redirect-rules.txt        # Redirection rules
            errors/
                error-mappings.txt        # Error code to message maps
                validation-messages.txt    # Field validation messages
            compat/
                browser-checks.txt        # Browser feature tests
                fallback-rules.txt        # Fallback behavior rules
scripts/                     # Shell scripts for automation
    build/                   # Build automation scripts
        configure            # Project configuration script
        build.sh             # Main build script
        make-dist.sh         # Create distribution package
        make-debug.sh        # Create debug build
        make-release.sh      # Create release build
        install.sh           # Installation script
        clean.sh             # Cleanup script
        test.sh              # Test runner script
    config/                  # Configuration scripts
        mkconfig.sh          # Script to generate config headers
        mkdirs.sh            # Script to create installation directories
    install/                 # Installation scripts
        install-deps.sh      # Install dependencies
        install-tools.sh     # Install dev tools
        post-install.sh      # Post-installation tasks
    utils/                   # Utility scripts
        gen-docs.sh          # Generate documentation
        check-style.sh       # Code style checker
        sync-test.sh         # Sync test data
        run-tests.sh         # Test execution
    include/                 # Include directory scripts
        update-headers.sh    # Update header files
    src/                     # Source directory scripts
        compile-core.sh      # Compile core server functionality
        compile-http.sh      # Compile HTTP protocol handling
        compile-utils.sh     # Compile utility functions
        compile-compat.sh    # Compile POSIX compatibility functions
        compile-main.sh      # Compile main entry point
        compile-constants.sh # Compile global constants
        compile-structs.sh   # Compile structure definitions
        compile-data.sh      # Compile data operations
        compile-error.sh     # Compile error handling
        compile-validation.sh # Compile validation functions
        compile-memory.sh    # Compile memory management
        compile-threading.sh # Compile thread management
        compile-io.sh        # Compile I/O operations
        compile-cache.sh     # Compile cache management
        compile-garbage.sh   # Compile garbage collection
        compile-system.sh    # Compile system initialization and cleanup
        compile-docs.sh      # Compile documentation generation
        compile-scripting.sh # Compile script execution and validation
        compile-permissions.sh # Compile permission management
        compile-auth.sh      # Compile user authentication
        compile-encryption.sh # Compile data encryption and decryption
        compile-encoding.sh  # Compile data encoding and decoding
        compile-recutils.sh  # Compile Recutils file operations
        compile-archive.sh   # Compile archive management
        compile-backup.sh    # Compile backup management
        compile-checksum.sh  # Compile checksum calculation and verification
        compile-config.sh    # Compile configuration management
        compile-logging.sh   # Compile logging
        compile-monitoring.sh # Compile system monitoring
        compile-network.sh   # Compile network management
        compile-database.sh  # Compile database management
        compile-filesystem.sh # Compile file system operations
        compile-systeminfo.sh # Compile system information
        compile-process.sh   # Compile process management
        compile-sysconfig.sh # Compile system configuration
    public/                  # Public directory scripts
        update-assets.sh     # Update public assets
        update-views.sh      # Update HTML templates
        update-data.sh       # Update data files
    var/                     # Variable data directory scripts
        update-records.sh    # Update system records
        update-logs.sh       # Update logging files
        update-runtime.sh    # Update runtime data files
        update-state.sh      # Update application state files
        update-queue.sh      # Update message queues
        update-db.sh         # Update database files
        update-exports.sh    # Update data exports
private/                     # Private files and data
    ssl/                     # SSL certificates and keys using BearSSL
        certs/               # SSL certificates
            server.crt       # SSL certificate
            server.key       # SSL private key
src/                         # C source code files
    core/                    # Core server functionality
        server_core.c        # Main server implementation
        config_core.c        # Configuration handling
    http/                    # HTTP protocol handling
        http_request_header_parser.c  # Parses HTTP request headers
        http_request_body_parser.c    # Parses HTTP request body
        http_response_header_builder.c # Constructs HTTP response headers
        http_response_body_builder.c  # Constructs HTTP response body
        http_route_matcher.c          # Matches HTTP requests to routes
    compat/                  # POSIX compatibility functions
        posix_compat.c       # POSIX-specific functions
        system_compat.c      # Compatibility functions
        ctype_compat.c       # Compatibility for character functions
        io_compat.c          # Compatibility for I/O functions
        legacy_compat.c      # Legacy compatibility functions
        pthread_compat.c     # Compatibility for POSIX threads
        signal_compat.c      # Compatibility for signal handling
        struct_compat.c      # Compatibility for data structures
        sys_compat.c         # Compatibility for system calls
        time_compat.c        # Compatibility for time functions
    main.c                   # Main entry point for the application
    common.c                 # Common functions and macros
    error_handling/          # Error handling
        error_logger.c           # Logs error messages to file or console
        error_reporter.c         # Reports errors to monitoring systems
        error_code_mapper.c      # Maps error codes to human-readable messages
        error_handler.c          # Provides centralized error handling logic
        error_validator.c        # Validates input data to prevent common errors
    validation/             # Validation functions
        input_validator.c        # Validates user input for correctness
        file_signature_verifier.c # Verifies digital signatures on files
        data_integrity_verifier.c # Checks data consistency and integrity
    memory_management/       # Memory management
        pointer_allocator.c      # Manages dynamic memory allocation for pointers
        buffer_manager.c         # Provides buffer creation, resizing, and deletion
        pointer_safety_checker.c # Checks for null and invalid pointers
    threading/              # Thread management
        thread_manager.c         # Manages thread creation, synchronization, and cleanup
        mutex_locker.c           # Provides functions to lock/unlock POSIX mutexes
        condition_variable.c     # Implements POSIX condition variable support
        thread_safe_queue.c      # Implements thread-safe queue operations
    io/                     # Input/Output operations
        io_file_reader.c         # Reads data from files efficiently
        io_file_writer.c         # Writes data to files efficiently
        io_socket_handler.c      # Handles socket-level I/O operations
    cache/                  # Cache management
        cache_initializer.c      # Initializes cache data structures
        cache_updater.c          # Updates cache entries based on new data
        cache_eviction.c         # Implements cache eviction policies (e.g., LRU)
    garbage_collection/     # Garbage collection
        garbage_collector.c      # Manages cleanup of unused memory regions
        object_ref_counter.c     # Tracks object references for manual garbage collection
    system/                 # System initialization and cleanup
        system_initializer.c     # Performs system-wide initialization tasks
        module_cleanup.c         # Cleans up resources during module shutdown
    documentation/          # Documentation generation
        doc_generator.c          # Generates documentation from source code comments
        man_page_builder.c       # Builds UNIX-style manual pages from templates
    scripting/              # Script execution and validation
        script_executor.c        # Executes custom scripts securely
        script_validator.c       # Validates the syntax of user-provided scripts
    permissions/            # Permission management
        permission_checker.c     # Verifies if a user has the required permissions
        access_control_list.c    # Manages access control lists for resources
    authentication/         # User authentication
        user_authenticator.c     # Authenticates users based on credentials
        auth_token_generator.c   # Generates secure authorization tokens
        auth_token_validator.c   # Validates the integrity and authenticity of authorization tokens
        auth_policy_enforcer.c   # Enforces authorization policies
        auth_access_control.c    # Manages access control based on authorization tokens and policies
        auth_token_revoker.c     # Handles token revocation and invalidation
        auth_policy_loader.c     # Loads and parses authorization policy configurations
        auth_audit_logger.c      # Logs authorization events for auditing purposes
        auth_session_manager.c   # Manages user sessions related to authorization
    encryption/             # Data encryption and decryption
        aes_encryptor.c          # Encrypts data using AES
        aes_decryptor.c          # Decrypts data encrypted with AES
        rsa_key_manager.c        # Manages RSA keys for encryption and decryption
    encoding/               # Data encoding and decoding
        iso_8859_1_encoder.c     # Encodes data into ISO-8859-1 format
        iso_8859_1_decoder.c     # Decodes ISO-8859-1 encoded data
    recutils/               # Recutils file operations
        rec_file_reader.c        # Contains functions for reading records from Recutils files
        rec_file_writer.c        # Contains functions for writing data to Recutils files
        rec_record_parser.c      # Functions for parsing individual records
        rec_record_validator.c   # Functions for validating the integrity of records
        rec_metadata_extractor.c # Functions for extracting metadata from Recutils files
        rec_metadata_updater.c   # Functions for updating metadata within Recutils files
        rec_filter_parser.c      # Functions for parsing filter expressions to search records
        rec_searcher.c           # Functions for searching and filtering records based on specific criteria
        rec_importer.c           # Functions for importing records from external sources into Recutils files
        rec_exporter.c           # Functions for exporting records from Recutils files to other formats
        rec_data_transformer.c   # Functions for transforming data within Recutils files (e.g., converting data types, formatting)
        rec_config_loader.c      # Functions for loading configuration settings related to Recutils file operations
        rec_file_initializer.c   # Functions for initializing Recutils files with required structure and metadata
        rec_error_logger.c       # Functions for logging errors related to Recutils operations
        rec_error_handler.c      # Functions for handling errors and exceptions during Recutils file processing
        rec_backup_manager.c     # Functions for backing up Recutils files
        rec_restore_manager.c    # Functions for restoring Recutils files from backups
        rec_compressor.c         # Functions for compressing Recutils files for storage
        rec_decompressor.c       # Functions for decompressing Recutils files for access
    archive/                # Archive management
        archive_version_fetcher.c # Retrieves archive version metadata
        archive_version_format_validator.c # Validates version format
        archive_version_comparator.c # Compares archive versions
        archive_date_parser.c    # Parses date strings into structured formats
        archive_date_format_validator.c # Ensures date format validity
        archive_date_comparator.c # Compares archive dates
        archive_deprecated_marker.c # Marks deprecated archives
        archive_deprecation_checker.c # Checks if an archive is deprecated
    backup/                 # Backup management
        backup_metadata_creator.c # Creates metadata for backups
        backup_metadata_validator.c # Validates backup metadata
        backup_metadata_updater.c # Updates backup metadata
        backup_file_packer.c     # Packs files into a backup
        backup_file_unpacker.c   # Unpacks files from a backup
        backup_file_compressor.c # Compresses backup files
        backup_file_decompressor.c # Decompresses backup files
        backup_integrity_checker.c # Verifies backup integrity
        backup_integrity_reporter.c # Reports integrity status
    checksum/               # Checksum calculation and verification
        checksum_sha256_calculator.c # Computes SHA-256 checksum
        checksum_sha256_verifier.c # Verifies SHA-256 checksum
        checksum_md5_calculator.c # Computes MD5 checksum
        checksum_md5_verifier.c  # Verifies MD5 checksum
        checksum_file_reader.c   # Reads file for checksum calculation
        checksum_file_writer.c   # Writes checksum to file
    configuration/          # Configuration management
        config_key_reader.c      # Reads configuration key
        config_key_writer.c      # Writes configuration key
        config_file_reader.c     # Reads configuration file
        config_file_writer.c     # Writes to configuration file
        config_settings_loader.c # Loads configuration settings
        config_settings_saver.c  # Saves configuration settings
    logging/                # Logging
        log_writer.c             # Writes logs to a specified log file or console
        log_reader.c             # Reads logs from a file or buffer
        log_formatter.c          # Formats log messages for consistent output
        log_rotator.c            # Rotates log files based on size or time
        error_log_writer.c       # Writes error messages to the log
        error_log_reader.c       # Reads error logs for analysis
        error_log_formatter.c    # Formats error messages for logging
    monitoring/             # System monitoring
        monitor_service.c        # Monitors system health and resource usage
        alert_manager.c          # Sends alerts based on system monitoring thresholds
        event_notifier.c         # Notifies the system or users of significant events
    network/                # Network management
        socket_creator.c         # Creates and manages network sockets
        socket_listener.c        # Listens for incoming network connections
        socket_connector.c       # Connects to remote servers via sockets
        socket_communicator.c    # Sends and receives data over sockets
        http_client.c            # Implements HTTP client functionality for sending requests
        http_server.c            # Implements HTTP server functionality for handling requests
        http_router.c            # Routes incoming HTTP requests to the appropriate handlers
        http_request_handler.c   # Handles incoming HTTP requests
        http_response_builder.c  # Builds HTTP responses for the server
        ssl_compat.c             # Provides compatibility for SSL/TLS protocols
        firewall_manager.c       # Manages network firewall rules
        network_monitor.c        # Monitors network traffic for security and performance
    database/               # Database management
        db_connection.c          # Manages database connections
        db_query_executor.c      # Executes SQL queries against a database
        db_result_parser.c       # Parses database query results
        db_insert.c              # Handles database record insertion
        db_update.c              # Handles database record updates
        db_delete.c              # Handles database record deletion
        db_transaction.c         # Manages database transactions
        db_migration.c           # Handles database schema and data migrations
        db_backup_manager.c      # Manages database backups
        db_restore_manager.c     # Restores database from backups
        db_encryption.c          # Encrypts sensitive data in the database
        db_access_control.c      # Manages access control for database operations
    file_system/            # File system operations
        file_reader.c            # Reads files from the file system
        file_writer.c            # Writes data to files on the file system
        file_deleter.c           # Deletes files from the file system
        file_renamer.c           # Renames files on the file system
        file_permission_checker.c # Checks file permissions
        file_permission_setter.c # Modifies file permissions
        file_owner_setter.c      # Changes the owner of a file
        dir_creator.c            # Creates directories in the file system
        dir_remover.c            # Removes directories from the file system
        dir_list_reader.c        # Reads the contents of a directory
        file_compressor.c        # Compresses files into archives
        file_decompressor.c      # Decompresses files from archives
        file_encryption.c        # Encrypts files before storing or transmitting
        file_decryption.c        # Decrypts files after retrieval
    system_info/            # System information
        system_info_reader.c     # Retrieves system information such as CPU, memory, etc.
        system_health_checker.c  # Monitors system health and resource usage
        system_shutdown_manager.c # Handles system shutdown or restart procedures
    process_management/     # Process management
        process_manager.c        # Manages processes and their execution
        process_monitor.c        # Monitors the status of running processes
        process_killer.c         # Terminates processes based on conditions
    sys_config/             # System configuration
        sys_config_loader.c      # Loads system configuration files
        sys_config_updater.c     # Updates system configuration settings
        sys_config_validator.c   # Validates system configuration files for correctness
    util/                       # Utility functions
        string_utils.c           # String manipulation functions
        date_utils.c             # Date and time manipulation functions
        file_utils.c             # File system utility functions
        memory_utils.c           # Memory management functions
        network_utils.c          # Network utility functions
        system_utils.c           # System utility functions
        crypto_utils.c           # Cryptographic utility functions
        data_utils.c             # Data manipulation functions
        validation_utils.c       # Data validation functions
        logging_utils.c          # Logging utility functions
        monitoring_utils.c       # System monitoring utility functions
        security_utils.c         # Security utility functions
        config_utils.c           # Configuration utility functions
        backup_utils.c           # Backup utility functions
        checksum_utils.c         # Checksum utility functions
        archive_utils.c          # Archive utility functions
        process_utils.c          # Process management utility functions
        sys_config_utils.c       # System configuration utility functions
        util.c                   # General utility functions
test/                        # Unit tests directory
    unit/                    # Unit test files
        core/                    # Core server functionality tests
            server_core_test.c   # Server core functionality tests
            config_core_test.c   # Configuration handling tests
        http/                    # HTTP protocol tests
            http_request_header_parser_test.c  # Parses HTTP request headers tests
            http_request_body_parser_test.c    # Parses HTTP request body tests
            http_response_header_builder_test.c # Constructs HTTP response headers tests
            http_response_body_builder_test.c  # Constructs HTTP response body tests
            http_route_matcher_test.c          # Matches HTTP requests to routes tests
        util/                    # Utility function tests
            string_utils_test.c  # String manipulation tests
            memory_utils_test.c  # Memory management tests
        compat/                  # POSIX compatibility function tests
            posix_compat_test.c  # POSIX-specific function tests
            system_compat_test.c # Compatibility function tests
        main_test.c              # Main entry point tests
        constant_test.c          # Global constants tests
        struct_test.c            # Structure definitions tests
        data_operations_test.c   # Implements functions to manipulate data structures tests
        data_manager_test.c      # Manages collections of data tests
        error_handling/          # Error handling tests
            error_logger_test.c      # Logs error messages to file or console tests
            error_reporter_test.c    # Reports errors to monitoring systems tests
            error_code_mapper_test.c # Maps error codes to human-readable messages tests
            error_handler_test.c     # Provides centralized error handling logic tests
            error_validator_test.c   # Validates input data to prevent common errors tests
        validation/             # Validation function tests
            input_validator_test.c        # Validates user input for correctness tests
            file_signature_verifier_test.c # Verifies digital signatures on files tests
            data_integrity_verifier_test.c # Checks data consistency and integrity tests
        memory_management/       # Memory management tests
            pointer_allocator_test.c      # Manages dynamic memory allocation for pointers tests
            buffer_manager_test.c         # Provides buffer creation, resizing, and deletion tests
            pointer_safety_checker_test.c # Checks for null and invalid pointers tests
        threading/              # Thread management tests
            thread_manager_test.c         # Manages thread creation, synchronization, and cleanup tests
            mutex_locker_test.c           # Provides functions to lock/unlock POSIX mutexes tests
            condition_variable_test.c     # Implements POSIX condition variable support tests
            thread_safe_queue_test.c      # Implements thread-safe queue operations tests
        io/                     # Input/Output operation tests
            io_file_reader_test.c         # Reads data from files efficiently tests
            io_file_writer_test.c         # Writes data to files efficiently tests
            io_socket_handler_test.c      # Handles socket-level I/O operations tests
        cache/                  # Cache management tests
            cache_initializer_test.c      # Initializes cache data structures tests
            cache_updater_test.c          # Updates cache entries based on new data tests
            cache_eviction_test.c         # Implements cache eviction policies (e.g., LRU) tests
        garbage_collection/     # Garbage collection tests
            garbage_collector_test.c      # Manages cleanup of unused memory regions tests
            object_ref_counter_test.c     # Tracks object references for manual garbage collection tests
        system/                 # System initialization and cleanup tests
            system_initializer_test.c     # Performs system-wide initialization tasks tests
            module_cleanup_test.c         # Cleans up resources during module shutdown tests
        documentation/          # Documentation generation tests
            doc_generator_test.c          # Generates documentation from source code comments tests
            man_page_builder_test.c       # Builds UNIX-style manual pages from templates tests
        scripting/              # Script execution and validation tests
            script_executor_test.c        # Executes custom scripts securely tests
            script_validator_test.c       # Validates the syntax of user-provided scripts tests
        permissions/            # Permission management tests
            permission_checker_test.c     # Verifies if a user has the required permissions tests
            access_control_list_test.c    # Manages access control lists for resources tests
        authentication/         # User authentication tests
            user_authenticator_test.c     # Authenticates users based on credentials tests
            auth_token_generator_test.c   # Generates secure authorization tokens tests
            auth_token_validator_test.c   # Validates the integrity and authenticity of authorization tokens tests
            auth_policy_enforcer_test.c   # Enforces authorization policies tests
            auth_access_control_test.c    # Manages access control based on authorization tokens and policies tests
            auth_token_revoker_test.c     # Handles token revocation and invalidation tests
            auth_policy_loader_test.c     # Loads and parses authorization policy configurations tests
            auth_audit_logger_test.c      # Logs authorization events for auditing purposes tests
            auth_session_manager_test.c   # Manages user sessions related to authorization tests
        encryption/             # Data encryption and decryption tests
            aes_encryptor_test.c          # Encrypts data using AES tests
            aes_decryptor_test.c          # Decrypts data encrypted with AES tests
            rsa_key_manager_test.c        # Manages RSA keys for encryption and decryption tests
        encoding/               # Data encoding and decoding tests
            iso_8859_1_encoder_test.c     # Encodes data into ISO-8859-1 format tests
            iso_8859_1_decoder_test.c     # Decodes ISO-8859-1 encoded data tests
        recutils/               # Recutils file operations tests
            rec_file_reader_test.c        # Contains functions for reading records from Recutils files tests
            rec_file_writer_test.c        # Contains functions for writing data to Recutils files tests
            rec_record_parser_test.c      # Functions for parsing individual records tests
            rec_record_validator_test.c   # Functions for validating the integrity of records tests
            rec_metadata_extractor_test.c # Functions for extracting metadata from Recutils files tests
            rec_metadata_updater_test.c   # Functions for updating metadata within Recutils files tests
            rec_filter_parser_test.c      # Functions for parsing filter expressions to search records tests
            rec_searcher_test.c           # Functions for searching and filtering records based on specific criteria tests
            rec_importer_test.c           # Functions for importing records from external sources into Recutils files tests
            rec_exporter_test.c           # Functions for exporting records from Recutils files to other formats tests
            rec_data_transformer_test.c   # Functions for transforming data within Recutils files tests (e.g., converting data types, formatting) tests
            rec_config_loader_test.c      # Functions for loading configuration settings related to Recutils file operations tests
            rec_file_initializer_test.c   # Functions for initializing Recutils files with required structure and metadata tests
            rec_error_logger_test.c       # Functions for logging errors related to Recutils operations tests
            rec_error_handler_test.c      # Functions for handling errors and exceptions during Recutils file processing tests
            rec_backup_manager_test.c     # Functions for backing up Recutils files tests
            rec_restore_manager_test.c    # Functions for restoring Recutils files from backups tests
            rec_compressor_test.c         # Functions for compressing Recutils files for storage tests
            rec_decompressor_test.c       # Functions for decompressing Recutils files for access tests
        archive/                # Archive management tests
            archive_version_fetcher_test.c # Retrieves archive version metadata tests
            archive_version_format_validator_test.c # Validates version format tests
            archive_version_comparator_test.c # Compares archive versions tests
            archive_date_parser_test.c    # Parses date strings into structured formats tests
            archive_date_format_validator_test.c # Ensures date format validity tests
            archive_date_comparator_test.c # Compares archive dates tests
            archive_deprecated_marker_test.c # Marks deprecated archives tests
            archive_deprecation_checker_test.c # Checks if an archive is deprecated tests
        backup/                 # Backup management tests
            backup_metadata_creator_test.c # Creates metadata for backups tests
            backup_metadata_validator_test.c # Validates backup metadata tests
            backup_metadata_updater_test.c # Updates backup metadata tests
            backup_file_packer_test.c     # Packs files into a backup tests
            backup_file_unpacker_test.c   # Unpacks files from a backup tests
            backup_file_compressor_test.c # Compresses backup files tests
            backup_file_decompressor_test.c # Decompresses backup files tests
            backup_integrity_checker_test.c # Verifies backup integrity tests
            backup_integrity_reporter_test.c # Reports integrity status tests
        checksum/               # Checksum calculation and verification tests
            checksum_sha256_calculator_test.c # Computes SHA-256 checksum tests
            checksum_sha256_verifier_test.c # Verifies SHA-256 checksum tests
            checksum_md5_calculator_test.c # Computes MD5 checksum tests
            checksum_md5_verifier_test.c  # Verifies MD5 checksum tests
            checksum_file_reader_test.c   # Reads file for checksum calculation tests
            checksum_file_writer_test.c   # Writes checksum to file tests
        configuration/          # Configuration management tests
            config_key_reader_test.c      # Reads configuration key tests
            config_key_writer_test.c      # Writes configuration key tests
            config_file_reader_test.c     # Reads configuration file tests
            config_file_writer_test.c     # Writes to configuration file tests
            config_settings_loader_test.c # Loads configuration settings tests
            config_settings_saver_test.c  # Saves configuration settings tests
        logging/                # Logging tests
            log_writer_test.c             # Writes logs to a specified log file or console tests
            log_reader_test.c             # Reads logs from a file or buffer tests
            log_formatter_test.c          # Formats log messages for consistent output tests
            log_rotator_test.c            # Rotates log files based on size or time tests
            error_log_writer_test.c       # Writes error messages to the log tests
            error_log_reader_test.c       # Reads error logs for analysis tests
            error_log_formatter_test.c    # Formats error messages for logging tests
        monitoring/             # System monitoring tests
            monitor_service_test.c        # Monitors system health and resource usage tests
            alert_manager_test.c          # Sends alerts based on system monitoring thresholds tests
            event_notifier_test.c         # Notifies the system or users of significant events tests
        network/                # Network management tests
            socket_creator_test.c         # Creates and manages network sockets tests
            socket_listener_test.c        # Listens for incoming network connections tests
            socket_connector_test.c       # Connects to remote servers via sockets tests
            socket_communicator_test.c    # Sends and receives data over sockets tests
            http_client_test.c            # Implements HTTP client functionality for sending requests tests
            http_server_test.c            # Implements HTTP server functionality for handling requests tests
            http_router_test.c            # Routes incoming HTTP requests to the appropriate handlers tests
            http_request_handler_test.c   # Handles incoming HTTP requests tests
            http_response_builder_test.c  # Builds HTTP responses for the server tests
            ssl_compat_test.c             # Provides compatibility for SSL/TLS protocols tests
            firewall_manager_test.c       # Manages network firewall rules tests
            network_monitor_test.c        # Monitors network traffic for security and performance tests
        database/               # Database management tests
            db_connection_test.c          # Manages database connections tests
            db_query_executor_test.c      # Executes SQL queries against a database tests
            db_result_parser_test.c       # Parses database query results tests
            db_insert_test.c              # Handles database record insertion tests
            db_update_test.c              # Handles database record updates tests
            db_delete_test.c              # Handles database record deletion tests
            db_transaction_test.c         # Manages database transactions tests
            db_migration_test.c           # Handles database schema and data migrations tests
            db_backup_manager_test.c      # Manages database backups tests
            db_restore_manager_test.c     # Restores database from backups tests
            db_encryption_test.c          # Encrypts sensitive data in the database tests
            db_access_control_test.c      # Manages access control for database operations tests
        file_system/            # File system operations tests
            file_reader_test.c            # Reads files from the file system tests
            file_writer_test.c            # Writes data to files on the file system tests
            file_deleter_test.c           # Deletes files from the file system tests
            file_renamer_test.c           # Renames files on the file system tests
            file_permission_checker_test.c # Checks file permissions tests
            file_permission_setter_test.c # Modifies file permissions tests
            file_owner_setter_test.c      # Changes the owner of a file tests
            dir_creator_test.c            # Creates directories in the file system tests
            dir_remover_test.c            # Removes directories from the file system tests
            dir_list_reader_test.c        # Reads the contents of a directory tests
            file_compressor_test.c        # Compresses files into archives tests
            file_decompressor_test.c      # Decompresses files from archives tests
            file_encryption_test.c        # Encrypts files before storing or transmitting tests
            file_decryption_test.c        # Decrypts files after retrieval tests
        system_info/            # System information tests
            system_info_reader_test.c     # Retrieves system information such as CPU, memory, etc. tests
            system_health_checker_test.c  # Monitors system health and resource usage tests
            system_shutdown_manager_test.c # Handles system shutdown or restart procedures tests
        process_management/     # Process management tests
            process_manager_test.c        # Manages processes and their execution tests
            process_monitor_test.c        # Monitors the status of running processes tests
            process_killer_test.c         # Terminates processes based on conditions tests
        sys_config/             # System configuration tests
            sys_config_loader_test.c      # Loads system configuration files tests
            sys_config_updater_test.c     # Updates system configuration settings tests
            sys_config_validator_test.c   # Validates system configuration files for correctness tests
    integration/             # Integration tests
        api_test.c           # API integration tests
        system_test.c        # System integration tests
    performance/             # Performance test suite
        load_test.c          # Load testing
        stress_test.c        # Stress testing
    security/                # Security test suite
        penetration_test.c   # Penetration tests
        xss_test.c           # XSS vulnerability tests
        sql_injection_test.c # SQL injection tests
    mocks/                   # Test doubles
        http_request_mock.c      # HTTP request mocks
        util_stub.c              # Utility stubs
        include/                     # Test headers
        fixtures/                # Test fixture data
            data.conf            # Test data in INI format
            config.conf          # Test config in INI format
        test_helpers.h           # Test helper functions
tmp/                            # Temporary files
    build/                      # Build-related temporary files
        tmp-build-*.log      # Build process logs
        tmp-make-*.out       # Make process output
pid/                     # Process ID files
    app-*.pid            # Running application PIDs
    worker-*.pid         # Worker process PIDs
cache/                   # Cache directory
    tmp-cache-*.dat      # Temporary cache data
    tmp-index-*.idx      # Cache indices
var/                         # Variable data directory
    records/                 # System records
        obligation_number.txt # Obligation number
        group-1234.rec       # Group record
        user-1234.rec        # User record
        trans-w6946.rec      # Transaction record
        trans-scjv.rec       # Transaction record
        trans-ms1180.rec     # Transaction record
        schema-v1.desc       # Schema descriptor
        test-data.rec        # Test data
    log/                     # Logging root directory
        web/                 # Web server logs
            access-YYYY-MM-DD.log # Daily access logs
            error-YYYY-MM-DD.log  # Daily error logs
            events-YYYY-MM-DD.log # Combined daily events
        app/                 # Application logs
            app_core.log     # Core application logs
            app_debug.log    # Debug information
            app_audit.log    # Application audit trail
        security/            # Security logs
            auth-YYYY-MM-DD.log # Daily authentication logs
            audit-YYYY-MM-DD.log # Daily security audit
            incident-YYYY-MM-DD.log # Security incidents
        system/              # System logs
            daemon.log       # Daemon activities
            kernel.log       # Kernel messages
            cron.log         # Scheduled tasks
    data/                    # Application data directory
        runtime/             # Runtime data files
            cache/           # Cache files
                *.cache      # Temporary cached content
                index.dat    # Cache index file
            sessions/        # Session data
                sess_*       # Session files by ID
                sessions.idx # Session index
        state/               # Application state
            counters/        # Counter files
                req_count.dat # Request counter
                hits.dat     # Hit counter
            flags/           # Flag files
                maint.flag   # Maintenance mode flag
                debug.flag   # Debug mode flag
            locks/           # Lock files
                app.lock     # Application lock
                proc.lock    # Process lock
        queue/               # Message queues
            incoming/        # Incoming messages
                msg-*.in     # Incoming message files
                in.idx       # Incoming index
            outgoing/        # Outgoing messages
                msg-*.out    # Outgoing message files
                out.idx      # Outgoing index
    db/                      # Database files
        app.sqlite           # Main database
        app.sqlite-journal   # Journal file  
        app.sqlite-wal       # Write-ahead log
        app.sqlite-shm       # Shared memory
    exports/                 # Data exports
        data-20240314.sql    # SQL dump with date
        data-20240314.csv    # CSV export with date
```

## **Native Web APIs Compatibility Guidelines**

## Part 1: Compatible Native Browser Web APIs with HTML 4.1, CSS 2.1, and ECMAScript 3

Given the constraints of **HTML 4.1**, **CSS 2.1**, and **ECMAScript 3**, the following native browser Web APIs are compatible:

1. **DOM (Document Object Model) API**
   - Allows manipulation of the structure and content of HTML documents.
   - ECMAScript 3 supports DOM manipulation using methods like `getElementById`, `createElement`, etc.

2. **Window Object**
   - Includes methods like `alert()`, `confirm()`, `prompt()`, `setTimeout()`, and `setInterval()`.
   - These functions are supported in browsers that implement ECMAScript 3.

3. **Navigator Object**
   - Provides browser and operating system information using properties like `navigator.userAgent` and `navigator.platform`.

4. **Location Object**
   - Provides methods and properties for manipulating the URL of the current document (e.g., `window.location.href`).

5. **History Object**
   - Includes methods like `history.back()`, `history.forward()`, and `history.pushState()` (although `pushState` is less compatible with older browsers).

6. **Event Handling**
   - Basic event handling using `addEventListener` (or `attachEvent` for older Internet Explorer versions).

7. **Timers (setTimeout, setInterval)**
   - Part of ECMAScript 3, allowing scheduling of functions.

8. **JavaScript Alerts**
   - Functions like `alert()`, `confirm()`, and `prompt()` are widely supported in ECMAScript 3.

9. **CSS Object Model (CSSOM)**
   - Allows basic manipulation of CSS properties through JavaScript. Advanced features may not be compatible with older browsers.

## Part 2: Compatibility with a Custom Backend Web Server in Strict POSIX ANSI-C89 and ISO8859-1 Encoding

When considering compatibility with a backend custom web server written in **strict POSIX ANSI-C89** and **ISO8859-1 encoding**, the following points are relevant:

1. **DOM API**
   - **Compatibility**: The DOM API operates client-side and does not directly interact with the server. The server can serve HTML documents that use the DOM API, but it doesn't interact with it.
   - **Server-side Compatibility**: The server must ensure the HTML content it serves is in ISO8859-1 encoding (`Content-Type: text/html; charset=ISO-8859-1`).

2. **Window Object**
   - **Compatibility**: This is a browser-side API and doesn?t interact with the server directly. It will work in browsers supporting ECMAScript 3.
   - **Server-side Compatibility**: The server doesn?t need to handle this API, as it is used only in the browser.

3. **Navigator Object**
   - **Compatibility**: The `navigator` object is client-side and doesn?t affect server-side code. It provides browser-specific information to client-side JavaScript.
   - **Server-side Compatibility**: The server handles requests from different browsers but does not need to interact with the `navigator` object. The server should correctly handle HTTP headers and user agent information.

4. **Location Object**
   - **Compatibility**: The `location` object is a client-side API and doesn?t interact with the server directly.
   - **Server-side Compatibility**: The server doesn?t need to interact with the `location` object. It simply serves content based on the URL requested.

5. **History Object**
   - **Compatibility**: This object is used for browser history manipulation and does not affect the server.
   - **Server-side Compatibility**: The server serves pages based on client requests but does not need to interact with the history object.

6. **Event Handling**
   - **Compatibility**: Event handling is client-side, and while some older browsers use `attachEvent` instead of `addEventListener`, both can work with ECMAScript 3.
   - **Server-side Compatibility**: The server doesn?t interact with event handling but can serve JavaScript files that include event listeners.

7. **Timers (setTimeout, setInterval)**
   - **Compatibility**: These are client-side APIs, executed by the browser?s JavaScript engine.
   - **Server-side Compatibility**: The server doesn?t need to handle timers, as they are executed client-side.

8. **JavaScript Alerts**
   - **Compatibility**: `alert()`, `confirm()`, and `prompt()` are supported by browsers implementing ECMAScript 3.
   - **Server-side Compatibility**: These are client-side features and do not interact with the server.

9. **CSS Object Model (CSSOM)**
   - **Compatibility**: Basic CSSOM manipulation is supported in ECMAScript 3, but advanced features may not be compatible with older browsers.
   - **Server-side Compatibility**: The server serves HTML and CSS content but does not need to interact with the CSSOM. The server must ensure the CSS is served correctly, considering ISO8859-1 encoding.

## Conclusion

In summary, the **DOM API**, **Window Object**, **Navigator Object**, **Location Object**, **History Object**, **Event Handling**, **Timers**, **JavaScript Alerts**, and **CSSOM** are all compatible with browsers supporting **HTML 4.1**, **CSS 2.1**, and **ECMAScript 3**. These APIs are client-side and will work with a backend server written in **strict POSIX ANSI-C89**, as long as the server serves content in **ISO8859-1 encoding** and handles basic HTTP requests and responses. The server does not need to interact with these client-side APIs directly, but it must ensure proper encoding and serve compatible HTML, CSS, and JavaScript content.
````
