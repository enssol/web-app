Certainly! Below is a structured body for a `copilot-instructions.md` file that integrates your coding standards in a way Copilot can leverage effectively to guide its suggestions.

---

# **Copilot Instructions for EnvEng Web Application Development**

This document outlines coding standards and practices for the EnvEng Web Application development project. These instructions help align Copilot's suggestions with our project's requirements.

---

## **1. General Guidelines**

-   Adhere to **ISO/IEC 9899:2024** (C Standard), **POSIX.1-2024**, and **X/Open 800** compliance in all code suggestions.
-   Ensure all code is portable, cross-platform, and cross-architecture. Avoid platform-specific features unless encapsulated for easy replacement.

---

## **2. Code Structure**

### File Organization

-   Maintain the following directory structure:
    -   `src/`: Source code files.
    -   `include/`: Header files.
    -   `tests/`: Unit and integration tests.
    -   `docs/`: Documentation.
    -   `build/`: Build outputs.
    -   `scripts/`: Utility scripts.
-   Use **snake_case** for file names, e.g., `data_manager.c`, `user_auth.h`.

### Naming Conventions

-   **Variables**: Use `snake_case` (e.g., `user_id`).
-   **Functions**: Use `camelCase` (e.g., `processData`).
-   **Constants**: Use `UPPER_CASE` (e.g., `MAX_BUFFER_SIZE`).

---

## **3. Coding Practices**

### Data Handling (Aligned with Data-Oriented Programming)

-   **Separation**: Keep data definitions separate from logic. Define data structures in header files and implement logic in C files.
-   **Immutability**: Use immutable data structures whenever applicable.

### Error Handling

-   Always check return values of system calls and library functions. Example:
    ```c
    if ((fd = open(filename, O_RDONLY)) < 0)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    ```

### Memory Management

-   Avoid memory leaks. Always pair memory allocation (`malloc`) with deallocation (`free`).
-   Use tools like `valgrind` for debugging memory issues.

### Performance

-   Optimize for CPU cache efficiency with data structures (e.g., prefer arrays over linked lists for sequential access).

### Thread Safety

-   Ensure thread safety using POSIX threading primitives (e.g., `pthread_mutex_t`).

---

## **4. Style and Formatting**

### Indentation

-   Use spaces (not tabs) for indentation, with an indent size of 4.

### Brace Style

-   Use the following brace style:
    ```c
    if (condition)
    {
        // Code block
    }
    ```

### Line Length

-   Limit lines to 80 characters for readability.

---

## **5. Documentation**

-   Document all files and functions using block comments:

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

-   Use Markdown for external documentation and place it in the `docs/` directory.

---

## **6. Testing**

-   Write unit tests for all functions and features.
-   Follow the naming convention `test_<module_name>.c` for test files.
-   Use a unit testing framework like `Unity` or `CMock`.

---

## **7. Tools and Practices**

-   Use Git for version control, and ensure commit messages follow the format:
    ```
    [Type] Short description (e.g., [Fix] Resolve memory leak in data processing)
    ```
-   Use CI/CD pipelines to automate testing and code quality checks.
-   Regularly review and refactor code for maintainability and performance.

---

These instructions ensure that Copilot assists with generating consistent, maintainable, and compliant code for the EnvEng Web Application project.

---

### How to Use:

Paste this into your `copilot-instructions.md` file to guide GitHub Copilot's behavior while assisting in code generation.
