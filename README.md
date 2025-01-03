# EnvEng Web Server

A lightweight HTTP web server implementation in ANSI C (C89), focusing on POSIX compliance and minimal dependencies.

## Features
- Static file serving over HTTP/1.0
- Basic authentication and session management
- POSIX-compliant, musl libc based implementation
- Minimal footprint with static binary output
- ISO-8859-1 encoding support
- HTML4.01 Strict compliant
- CSS2.1 compliant
- ECMAScript 3 (ES3) compliant
- Mobile-first responsive design

## Prerequisites
- musl-gcc compiler
- musl libc
- GNU Make
- CUnit testing framework

## Build Quick Start

To build the project, follow these steps:

1. **Clone the repository**:
    ```sh
    git clone https://github.com/your-repo/web-app.git
    cd web-app
    ```

2. **Set up the environment**:
    ```sh
    export ENV=dev  # For development build
    # or
    export ENV=prod  # For production build
    ```

3. **Build the project**:
    ```sh
    make
    ```

4. **Install the project**:
    ```sh
    make install
    ```

5. **Clean build artifacts**:
    ```sh
    make clean
    ```

## Tool Requirements

Ensure you have the following tools installed:

- **GNU Make**: For building the project.
- **GCC**: The GNU Compiler Collection, for compiling C code.
- **musl libc**: A lightweight, POSIX-compliant standard C library.
- **Git**: For version control.
- **CUnit**: For unit testing.

## Build Commands

### Basic Build Commands
```bash
make                # Default build (verifies musl and builds production)
make prod          # Build production binary
make test          # Build and run tests with coverage
make check         # Build and run tests without coverage
make debug         # Build tests and launch GDB debugger
make clean         # Clean build artifacts

# Clean previous builds and create new release package
make release

# Create t4g-optimized release
make t4g-release

# The optimized package will be at:
# dist/web-app-0.0.1.tar.gz
```

## Project Structure
```
.
??? bin/                # Binary outputs
??? etc/               # Configuration files
??? include/           # Header files
??? lib/               # Libraries
??? src/              # Source code
??? test/             # Test suite
??? www/              # Static web content
    ??? assets/       # Static assets
    ??? error/        # Error pages
    ??? templates/    # HTML templates
```

## Usage
Start the server:
```bash
./bin/web_server -c /etc/web_server.conf
```

Access via browser:
- Login page: http://localhost:8080
- Admin panel: http://localhost:8080/admin
- Status page: http://localhost:8080/status

## Technical Details
- ISO/IEC 9899:1990 (C90) compliant
- POSIX.1 (IEEE 1003.1) compliant
- X/Open-500 compliant
- Static linking with musl libc
- ISO-8859-1 character encoding
- Memory-safe implementation
- Thread-safe operations
- No external dependencies

## Development Guidelines
- Mobile-first responsive design
- HTML 4.01 Strict / XHTML 1.0 Strict
- Native CSS2.1 only (no frameworks)
- ES3 JavaScript (no libraries)
- ISO-8859-1 encoding for all files

## Security
- Memory protection features
- Stack protection enabled
- Strict input validation
- Safe string handling
- Format string protection

## Running Tests

To ensure the quality and correctness of the web application, a comprehensive test suite has been implemented. Follow the steps below to set up and run the tests.

### Prerequisites

1. **Install Dependencies**: Ensure all necessary dependencies are installed. This includes the CUnit framework for unit testing.
    ```sh
    sudo apt-get update
    sudo apt-get install libcunit1 libcunit1-doc libcunit1-dev
    ```

2. **Build the Project**: Compile the project with the appropriate flags.
    ```sh
    make ENV=dev
    ```

### Running Unit Tests

Unit tests are located in the `test/unit` directory. To run the unit tests, execute the following command:
```sh
make test
```

### Running Integration Tests

Integration tests are located in the `test/integration` directory. To run the integration tests, execute the following command:
```sh
make integration-test
```

### Running Performance Tests

Performance tests are located in the `test/performance` directory. To run the performance tests, execute the following command:
```sh
make performance-test
```

### Running Security Tests

Security tests are located in the `test/security` directory. To run the security tests, execute the following command:
```sh
make security-test
```

### Cleaning Up

To clean up the build artifacts and temporary files after running the tests, execute the following command:
```sh
make clean
```

### Additional Notes

- Ensure that the environment variables and configurations are correctly set up before running the tests.
- Use `valgrind` or equivalent tools to check for memory leaks and other issues during testing.

For more detailed information on the test suite and individual test cases, refer to the documentation in the `docs` directory.

## Documentation Links

- [API Documentation](docs/api/API.md)
- [User Guide](docs/user/USER_GUIDE.md)
- [Developer Guide](docs/dev/DEVELOPER_GUIDE.md)
- [Installation Instructions](docs/user/INSTALL.md)
- [Frequently Asked Questions](docs/user/FAQ.md)
- [Support Information](docs/user/SUPPORT.md)
- [Contributing Guidelines](docs/dev/CONTRIBUTING.md)
- [Project Roadmap](docs/project/ROADMAP.md)
- [Security Policies](docs/security/SECURITY.md)

## License
AGPL-3.0-or-later
