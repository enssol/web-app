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
        build.h             # Build environment
        config.h            # Main config header
        args.h              # Build arguments
        machine.h           # Machine settings
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
        http_request.h       # Request parsing header
        http_response.h      # Response building header
    util/                    # Utility headers
        string_utils.h       # String manipulation header
        memory_utils.h       # Memory management header
    standards.h              # C90 compliance header
    compat/                  # POSIX compatibility headers
        posix_compat.h       # POSIX compatibility declarations
        struct_defs.h        # POSIX structure definitions
        ctype_compat.h       # Character type compatibility
        time_compat.h        # Time functions compatibility
        signal_compat.h      # Signal handling compatibility
        sys_compat.h         # System call compatibility
        pthread_compat.h     # Threading compatibility layer
        io_compat.h          # I/O operations compatibility
        legacy_compat.h      # Legacy system compatibility
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
     man8/                    # System administration
        webappd.8            # Web application daemon
public/                      # Public web assets
    index.html              # Main entry point
    assets/                 # Core assets
        images/             # All image assets
            ui/            # Interface elements
                logo.gif       # Site logo
                btn-basic.gif  # Basic button
                btn-action.gif # Action button
                icon-home.gif  # Home navigation
                bg-header.gif  # Header background
                separator.gif  # Visual separator
                arrow-next.gif # Navigation arrow
                arrow-prev.gif # Navigation arrow
                bullet.gif     # List bullet
                checkbox.gif   # Form checkbox
                radio.gif      # Form radio button
            content/          # Content images
                about.jpg     # About page photo
                welcome.jpg   # Welcome banner
                product.jpg   # Product image
            icons/           # Icon files
                favicon.ico  # Browser favicon required
    views/                  # HTML templates
        auth/              # Auth templates
            login.html     # Login form
            signup.html    # Registration
        errors/           # Error pages
            404.html      # Not found
            500.html      # Server error
        forms/            # Form pages
            contact.html  # Contact form
            search.html   # Search form
        pages/            # Static pages
            about.html    # About page
            faq.html      # FAQ page
        common/           # Shared elements
            header.html   # Page header
            footer.html   # Page footer
            base.html     # Base template
        layout/           # Layout templates
            default.html  # Default layout
            print.html    # Print layout
        includes/         # Reusable parts
            nav.html     # Navigation menu
            messages.html # System messages
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
        run-tests.sh         # Test execution
private/                     # Private files and data
    ssl/                     # SSL certificates and keys
        certs/               # SSL certificates
            server.crt       # SSL certificate
            server.key       # SSL private key
src/                         # C source code files
    core/                    # Core server functionality
        server_core.c        # Main server implementation
        config_core.c        # Configuration handling
    http/                    # HTTP protocol handling
        http_request.c       # Request parsing
        http_response.c      # Response building
    util/                    # Utility functions
        string_utils.c       # String manipulation
        memory_utils.c       # Memory management
    compat/                  # POSIX compatibility functions
        posix_compat.c       # POSIX-specific functions
        system_compat.c      # Compatibility functions
    main.c                   # Main entry point for the application
test/                        # Unit tests directory
    unit/                    # Unit test files
        server_core_test.c   # Server core functionality tests
        http_request_test.c  # HTTP request handling tests
        string_utils_test.c  # String utility tests
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
