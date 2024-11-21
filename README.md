# EnvEng-WebApp-Development

https://en.wikipedia.org/wiki/GNU_toolchain
Projects in the GNU toolchain are:

gcc: The GNU Compiler Collection, a compiler system supporting various programming languages, primarily C and C++.
glibc: The GNU C Library, providing the core libraries for the GNU system and GNU/Linux systems, offering essential APIs for C programming.
bison: A parser generator that is compatible with Yacc, used to convert a grammar description for an LALR context-free grammar into a C program to parse that grammar.
coreutils: A package of basic file, shell, and text manipulation utilities of the GNU operating system1.
autoconf: A tool for producing shell scripts that automatically configure software source code packages to adapt to various types of Unix-like systems.
automake: A tool for automatically generating Makefile.in files compliant with the GNU Coding Standards.
autotools: A suite of programming tools designed to assist in making source code packages portable to many Unix-like systems.
gdb: The GNU Debugger, a tool for debugging applications written in various programming languages.
emacs: An extensible, customizable, free text editor with built-in support for many programming languages and other features.
make: A build automation tool that automatically builds executable programs and libraries from source code by reading files called Makefiles.
binutils: A collection of binary tools, including ld (the GNU linker) and as (the GNU assembler).
pkgconf: A tool that helps manage compile and link flags for libraries, often used as a replacement for pkg-config.
libtool: A generic library support script that hides the complexity of using shared libraries behind a consistent, portable interface.
flex: A fast lexical analyzer generator, often used with Bison to generate scanners (programs that recognize lexical patterns in text).
gettext: A set of tools for internationalization and localization, allowing programs to be compiled with multi-language support.
m4: The GNU m4 package is a macro processing language available in the apt package manager. It is an implementation of the traditional UNIX macro processor, compatible with SVR4 but with some extensions. These extensions include handling more than nine positional parameters to macros and built-in functions for including files, running shell commands, and performing arithmetic.
autopoint is a tool from the GNU gettext package. It is used to set up the gettext infrastructure in a source package. Specifically, autopoint copies standard gettext infrastructure files into a source package, which is essential for internationalization (i18n) supporrt.

CFEngine – Configuration management software

Development of the EnvEng web application using 5S methodology for project management and Data-Oriented Programming (DOP) for development.

```md
project/
├── src/
│   ├── main.c
│   ├── utils.c
│   └── utils.h
├── build/
│   ├── main.i
│   ├── utils.i
│   ├── main.s
│   ├── utils.s
│   ├── main.asm
│   ├── utils.asm
│   ├── main.d
│   ├── utils.d
│   ├── main.gch
│   ├── utils.gch
│   ├── main.lst
│   ├── utils.lst
│   ├── main.map
│   ├── utils.map
│   ├── main.dbg
│   ├── utils.dbg
│   └── main.tmp
├── obj/
│   ├── main.o
│   ├── utils.o
│   └── utils.obj
├── lib/
│   ├── libmylib.a
│   └── libmylib.so
├── bin/
│   ├── myprogram.out
│   └── myprogram.exe
├── include/
│   └── mylib.h
├── docs/
│   ├── README.md
│   ├── manual.pdf
│   └── guide.txt
├── scripts/
│   ├── build.sh
│   └── clean.sh
├── config/
│   ├── build.cfg
│   └── settings.conf
├── logs/
│   └── build.log
└── Makefile
```

# Project Build Process

## 1. Preprocessing
   - File Inclusion: Replace #include directives with the contents of the specified header files.
     - **Tool**: `gcc -E`
     - **Input File Extension**: `.c`, `.h`
     - **Output File Extension**: `.i`
     - **Input Directory**: `src/`
     - **Output Directory**: `build/`
   - Macro Expansion: Replace macros defined with #define with their corresponding values.
     - **Tool**: `gcc -E`
     - **Input File Extension**: `.c`, `.h`
     - **Output File Extension**: `.i`
     - **Input Directory**: `src/`
     - **Output Directory**: `build/`
   - Conditional Compilation: Evaluate #if, #ifdef, #ifndef, #else, and #endif directives to include or exclude parts of the code.
     - **Tool**: `gcc -E`
     - **Input File Extension**: `.c`, `.h`
     - **Output File Extension**: `.i`
     - **Input Directory**: `src/`
     - **Output Directory**: `build/`
   - Line Control: Process #line directives to control line numbers and filenames in error messages and debugging information.
     - **Tool**: `gcc -E`
     - **Input File Extension**: `.c`, `.h`
     - **Output File Extension**: `.i`
     - **Input Directory**: `src/`
     - **Output Directory**: `build/`
   - Removing Comments: Strip out all comments from the source code.
     - **Tool**: `gcc -E`
     - **Input File Extension**: `.c`, `.h`
     - **Output File Extension**: `.i`
     - **Input Directory**: `src/`
     - **Output Directory**: `build/`

## 2. Compilation
   - Lexical Analysis: Convert the sequence of characters in the source code into a sequence of tokens.
     - **Tool**: `gcc -S`
     - **Input File Extension**: `.i`
     - **Output File Extension**: `.s`
     - **Input Directory**: `build/`
     - **Output Directory**: `build/`
   - Syntax Analysis: Parse the tokens to generate an Abstract Syntax Tree (AST) based on the grammar of the language.
     - **Tool**: `gcc -S`
     - **Input File Extension**: `.i`
     - **Output File Extension**: `.s`
     - **Input Directory**: `build/`
     - **Output Directory**: `build/`
   - Semantic Analysis: Check the AST for semantic errors (e.g., type checking, variable declaration).
     - **Tool**: `gcc -S`
     - **Input File Extension**: `.i`
     - **Output File Extension**: `.s`
     - **Input Directory**: `build/`
     - **Output Directory**: `build/`
   - Intermediate Code Generation: Translate the AST into an intermediate representation (IR), which is a lower-level code that is easier to optimize.
     - **Tool**: `gcc -S`
     - **Input File Extension**: `.i`
     - **Output File Extension**: `.s`
     - **Input Directory**: `build/`
     - **Output Directory**: `build/`
   - Optimization: Perform optimizations on the IR to improve performance (e.g., constant folding, dead code elimination).
     - **Tool**: `gcc -S`
     - **Input File Extension**: `.i`
     - **Output File Extension**: `.s`
     - **Input Directory**: `build/`
     - **Output Directory**: `build/`
   - Code Generation: Convert the optimized IR into assembly code.
     - **Tool**: `gcc -S`
     - **Input File Extension**: `.i`
     - **Output File Extension**: `.s`, `.asm`
     - **Input Directory**: `build/`
     - **Output Directory**: `build/`

## 3. Assembly
   - Assembly Translation: Translate the assembly code into machine code (binary instructions).
     - **Tool**: `as`
     - **Input File Extension**: `.s`, `.asm`
     - **Output File Extension**: `.o`
     - **Input Directory**: `build/`
     - **Output Directory**: `obj/`
   - Object File Creation: Package the machine code into an object file (.o), including metadata such as symbol tables and relocation information.
     - **Tool**: `as`
     - **Input File Extension**: `.s`, `.asm`
     - **Output File Extension**: `.o`
     - **Input Directory**: `build/`
     - **Output Directory**: `obj/`

## 4. Linking
   - Combining Object Files: Merge multiple object files into a single executable.
     - **Tool**: `ld`
     - **Input File Extension**: `.o`
     - **Output File Extension**: `.out`, `.exe`
     - **Input Directory**: `obj/`
     - **Output Directory**: `bin/`
   - Symbol Resolution: Resolve references to symbols (functions, variables) across object files.
     - **Tool**: `ld`
     - **Input File Extension**: `.o`
     - **Output File Extension**: `.out`, `.exe`
     - **Input Directory**: `obj/`
     - **Output Directory**: `bin/`
   - Library Linking: Link against static or dynamic libraries, resolving references to library functions.
     - **Tool**: `ld`
     - **Input File Extension**: `.o`, `.a`, `.so`
     - **Output File Extension**: `.out`, `.exe`
     - **Input Directory**: `obj/`, `lib/`
     - **Output Directory**: `bin/`
   - Relocation: Adjust addresses in the code and data sections to reflect their final positions in memory.
     - **Tool**: `ld`
     - **Input File Extension**: `.o`
     - **Output File Extension**: `.out`, `.exe`
     - **Input Directory**: `obj/`
     - **Output Directory**: `bin/`
   - Executable Creation: Generate the final executable file, including headers and metadata required for execution.
     - **Tool**: `ld`
     - **Input File Extension**: `.o`
     - **Output File Extension**: `.out`, `.exe`
     - **Input Directory**: `obj/`
     - **Output Directory**: `bin/`

## 5. Dependency Generation
   - Dependency Generation: Generate dependency files to track which files need to be recompiled when a source file changes.
     - **Tool**: `gcc -M`
     - **Input File Extension**: `.c`, `.h`
     - **Output File Extension**: `.d`
     - **Input Directory**: `src/`
     - **Output Directory**: `build/`

## 6. Optimization Tools and Techniques
   - Compiler Optimization Flags: Improve performance and efficiency of the generated code.
     - **Tool**: `gcc`
     - **Flags**: `-O0`, `-O1`, `-O2`, `-O3`, `-Os`, `-Ofast`, `-flto`
   - Profile-Guided Optimization (PGO): Use profiling data to optimize the program.
     - **Tool**: `gcc`
     - **Steps**: `-fprofile-generate`, `-fprofile-use`
   - Interprocedural Optimization (IPO): Perform optimizations across function and file boundaries.
     - **Tool**: `gcc`
     - **Flag**: `-flto`
   - Vectorization: Enable automatic vectorization of loops.
     - **Tool**: `gcc`
     - **Flags**: `-ftree-vectorize`, `-march=native`
   - Inlining: Enable function inlining.
     - **Tool**: `gcc`
     - **Flags**: `-finline-functions`, `-finline-limit=N`
   - Loop Optimization: Improve performance of loops.
     - **Tool**: `gcc`
     - **Flags**: `-funroll-loops`, `-floop-interchange`, `-floop-block`, `-floop-parallelize-all`
   - Dead Code Elimination: Remove code that does not affect the program's output.
     - **Tool**: `gcc`
     - **Flag**: `-fdce`
   - Constant Folding: Evaluate constant expressions at compile time.
     - **Tool**: `gcc`
     - **Flag**: `-ftree-ccp`

## 7. Other Common Tools
   - **Tool**: `make`
     - **Command**: `make`
     - **Description**: Automates the build process by reading `Makefile` instructions.
   - **Tool**: `gdb`
     - **Command**: `gdb`
     - **Description**: Debugs the executable, providing source-level debugging information.
   - **Tool**: `strip`
     - **Command**: `strip`
     - **Description**: Removes symbols from the executable to reduce its size.
   - **Tool**: `objdump`
     - **Command**: `objdump`
     - **Description**: Displays information about object files.
   - **Tool**: `nm`
     - **Command**: `nm`
     - **Description**: Lists symbols from object files.
   - **Tool**: `ranlib`
     - **Command**: `ranlib`
     - **Description**: Generates an index to the contents of an archive.

8. Other Common File Types
   - Precompiled Headers: Speed up compilation by precompiling frequently included headers.
     - **File Extension**: `.gch`
     - **Directory**: `build/`
   - Assembly Listings: Human-readable versions of the assembly code, often used for debugging and analysis.
     - **File Extension**: `.lst`
     - **Directory**: `build/`
   - Map Files: Provide detailed information about the memory layout of the program.
     - **File Extension**: `.map`
     - **Directory**: `build/`
   - Debug Information: Contain additional information used by debuggers to provide source-level debugging.
     - **File Extension**: `.dbg`
     - **Directory**: `build/`
   - Intermediate Object Files: Similar to `.o` files but may be used in different contexts or by different tools.
     - **File Extension**: `.obj`
     - **Directory**: `obj/`
   - Configuration Files: Store settings and parameters for the build process or the application.
     - **File Extension**: `.cfg`, `.conf`
     - **Directory**: `config/`
   - Log Files: Record the output of the build process or the execution of the application.
     - **File Extension**: `.log`
     - **Directory**: `logs/`
   - Temporary Files: Created during the build process, often used for intermediate steps.
     - **File Extension**: `.tmp`
     - **Directory**: `tmp/`
   - Documentation Files: Provide information about the project, such as README files, manuals, and guides.
     - **File Extension**: `.md`, `.txt`, `.pdf`
     - **Directory**: `docs/`
   - Script Files: Used to automate the build process or other tasks.
     - **File Extension**: `.sh`, `.bat`, `.ps1`
     - **Directory**: `scripts/`

## Summary
- Preprocessing:
  - File Inclusion
  - Macro Expansion
  - Conditional Compilation
  - Line Control
  - Removing Comments
- Compilation:
  - Lexical Analysis
  - Syntax Analysis
  - Semantic Analysis
  - Intermediate Code Generation
  - Optimization
  - Code Generation
- Assembly:
  - Assembly Translation
  - Object File Creation
- Linking:
  - Combining Object Files
  - Symbol Resolution
  - Library Linking
  - Relocation
  - Executable Creation
- Dependency Generation:
  - Dependency Generation
- Optimization Tools and Techniques:
  - Compiler Optimization Flags
  - Profile-Guided Optimization (PGO)
  - Interprocedural Optimization (IPO)
  - Vectorization
  - Inlining
  - Loop Optimization
  - Dead Code Elimination
  - Constant Folding
- Other Common Tools:
  - make
  - gdb
  - strip
  - objdump
  - nm
  - ranlib

Each of these micro stages contributes to transforming the high-level C source code into a runnable executable program.

Here is a set of **coding standards** derived from the provided information, aligned with the requirements for ISO/IEC 9899:2024, POSIX.1-2024, and X/Open 800 compliance, and incorporating the principles of 5S methodology and Data-Oriented Programming (DOP):  

---

### **Coding Standards for EnvEng Web Application Development**

#### **1. General Standards**
1.1 **Compliance**  
- All code must adhere to the ISO/IEC 9899:2024 (C Standard).  
- Code must be compliant with POSIX.1-2024 and X/Open 800 for portability and interoperability.  

1.2 **Portability**  
- Ensure the codebase is compatible across multiple platforms, architectures, and operating systems.  
- Avoid platform-specific features unless necessary, and encapsulate such dependencies to enable easy replacement.  

1.3 **Self-Containment**  
- All modules should be self-contained, with no reliance on external configurations or environment-specific details unless explicitly defined.  

---

#### **2. File and Repository Structure (Aligned with 5S: Set in Order)**  
2.1 **Directory Structure**  
- Use a logical and consistent directory structure, e.g.,:  
  ```
  src/         # Source code files  
  include/     # Header files  
  tests/       # Unit and integration tests  
  docs/        # Documentation  
  build/       # Build outputs  
  scripts/     # Utility scripts  
  ```

2.2 **File Naming**  
- Use descriptive, snake_case naming for files (e.g., `data_manager.c`, `user_auth.h`).  

2.3 **Version Control**  
- All source code must be tracked using a version control system (e.g., Git).  
- Commit messages should follow a standardized format:  
  ```
  [Type] Short Description (e.g., [Fix] Resolved memory leak in data processing)
  ```

---

#### **3. Coding Practices (Aligned with DOP and 5S Principles)**

3.1 **Clarity and Maintainability**  
- Separate data and logic by defining clear data structures in header files and encapsulating logic in C files.  
- Write modular and reusable functions.  

3.2 **Coding Style**  
- Follow a consistent style for readability:  
  - Indentation: 4 spaces (no tabs).  
  - Brace style: Place braces on a new line.  
    ```c
    if (condition) 
    {
        // Code block
    }
    ```
  - Naming:  
    - Variables: `snake_case` (e.g., `user_id`).  
    - Functions: `camelCase` (e.g., `processData`).  
    - Constants: `UPPER_CASE` (e.g., `MAX_BUFFER_SIZE`).  

3.3 **Data Handling**  
- Use immutable data structures whenever possible.  
- Avoid global variables; instead, use well-defined interfaces.  
- Centralize configuration and constants for reuse (e.g., `config.h`).  

3.4 **Error Handling**  
- Use POSIX-compliant error codes for standardization.  
- Always check the return value of system calls and library functions. Example:  
  ```c
  if ((fd = open(filename, O_RDONLY)) < 0) 
  {
      perror("Error opening file");
      exit(EXIT_FAILURE);
  }
  ```

3.5 **Memory Management**  
- Allocate and free memory explicitly; avoid memory leaks.  
- Use tools like `valgrind` to identify leaks during development.  

3.6 **Thread Safety**  
- Design code to be thread-safe using POSIX threading primitives (e.g., `pthread_mutex_t`).  

---

#### **4. Documentation (Aligned with 5S: Standardize)**  
4.1 **Code Comments**  
- Use block comments for file headers and function documentation:  
  ```c
  /**
   * Function: calculateSum
   * ----------------------
   * Computes the sum of two integers.
   *
   * x: First integer
   * y: Second integer
   *
   * returns: The sum of x and y
   */
  ```

4.2 **Standardized Templates**  
- Create templates for:  
  - Function definitions  
  - File headers  
  - Commit messages  

4.3 **External Documentation**  
- Maintain documentation for APIs, data structures, and workflows using Markdown in the `docs/` directory.  

---

#### **5. Testing Standards**  
5.1 **Test-Driven Development (TDD)**  
- Write tests for every feature and bug fix.  

5.2 **Unit Testing**  
- Use a framework like `Unity` or `CMock` for unit tests.  
- All tests must reside in the `tests/` directory and follow the naming convention `test_<module_name>.c`.  

5.3 **Automated Testing**  
- Implement CI/CD pipelines to automate testing and code quality checks.  

---

#### **6. Performance Optimization (Aligned with DOP)**  
6.1 **Efficient Data Structures**  
- Use data structures optimized for cache efficiency (e.g., arrays over linked lists when sequential access is frequent).  

6.2 **Profiling**  
- Profile code regularly using tools like `gprof` or `perf`.  

---

#### **7. 5S Maintenance and Sustainability**  
7.1 **Regular Reviews**  
- Conduct code reviews bi-weekly to ensure adherence to standards.  

7.2 **Continuous Training**  
- Train team members on 5S, DOP, and coding standards.  

7.3 **Audits**  
- Schedule periodic audits to ensure the workspace and codebase remain clean and organized.  

---

By adhering to these standards, the team will maintain clarity, ensure high performance, and achieve compliance with industry and project-specific requirements.

# EnvEng WebApp Development
 
Welcome to the **EnvEng WebApp Development** project! This repository contains the code and documentation for building a web application using **5S methodology** for project management and **Data-Oriented Programming (DOP)** for development.  
 
## Table of Contents
1. [Project Overview](#project-overview)
2. [Features](#features)
3. [Methodologies](#methodologies)
   - [5S Implementation](#5s-implementation)
   - [Data-Oriented Programming (DOP)](#data-oriented-programming-dop)
4. [Setup Instructions](#setup-instructions)
5. [Git Workflow](#git-workflow)
   - [Branching Strategy](#branching-strategy)
   - [Commit Guidelines](#commit-guidelines)
6. [CI/CD Workflow](#cicd-workflow)
7. [Contributing](#contributing)
8. [License](#license)
 
---
 
## Project Overview
 
The **EnvEng WebApp** aims to deliver a robust web application tailored for environmental engineering needs. Our goals include:
- Building a scalable, maintainable codebase.
- Ensuring clarity through **Data-Oriented Programming (DOP)** principles.
- Maintaining a clean and organized development process using **5S methodology**.
 
Key objectives:
- Implement user authentication and core features.
- Optimize data handling and performance.
- Adhere to best practices for project management and development.
 
---
 
## Features
 
- User authentication and authorization.
- Dashboard for managing environmental data.
- Integration with third-party APIs for real-time updates.
- Performance-optimized, clean, and maintainable codebase.
 
---
 
## Methodologies
 
### 5S Implementation
- **Sort**: Identify and eliminate unnecessary code and resources.
- **Set in Order**: Organize files and directories for easy access and maintainability.
- **Shine**: Maintain code quality with regular reviews and automated checks.
- **Standardize**: Follow coding standards and create reusable templates.
- **Sustain**: Regular training and audits to ensure long-term adherence.
 
### Data-Oriented Programming (DOP)
- **Data Separation**: Logical separation of data and application logic.
- **Efficient Data Handling**: Use optimized data structures for high performance.
- **Immutability**: Implement immutable data practices to enhance consistency.
 
---
 
## Setup Instructions
 
1. Clone the repository:
   ```bash
   git clone https://github.com/your-org/enveng-webapp.git
   cd enveng-webapp
   ```
 
2. Install dependencies:
   ```bash
   npm install
   ```
 
3. Run the development server:
   ```bash
   npm start
   ```
 
4. Build the project for production:
   ```bash
   npm run build
   ```
 
---
 
## Git Workflow
 
### Branching Strategy
- **Main**: Stable, production-ready code.
- **Develop**: Integration branch for new features and fixes.
- **Feature Branches**: `feature/feature-name`
- **Bugfix Branches**: `bugfix/bug-description`
 
### Commit Guidelines
- Write atomic commits focusing on one change.
- Use descriptive commit messages.
- Example:
  ```bash
  git commit -m "Add user authentication endpoint"
  ```
 
---
 
## CI/CD Workflow
 
### Continuous Integration (CI)
- Automated builds and tests for every push to `develop`.
- Static code analysis to ensure quality.
 
### Continuous Delivery (CD)
- Deploy to staging for integration tests.
- Require manual approval for production deployment.
 
### Pipeline Example
Refer to the `.github/workflows/ci-cd.yml` file for our complete CI/CD pipeline.
 
---
 
## Contributing
 
We welcome contributions to improve the EnvEng WebApp. Please follow these steps:
1. Fork the repository and clone it locally.
2. Create a new feature or bugfix branch.
3. Make your changes and commit them with a descriptive message.
4. Submit a pull request to the `develop` branch.
 
Refer to the [CONTRIBUTING.md](CONTRIBUTING.md) file for more details.
 
---
 
## License
 
This project is licensed under the [AGPLv3](LICENSE). See the `LICENSE` file for details.
 