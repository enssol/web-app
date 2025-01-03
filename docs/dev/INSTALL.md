# Installation Instructions

## Prerequisites
- GCC with musl libc
- Make
- Git
- CUnit (for testing)
- Valgrind (for memory analysis)
- BearSSL (for SSL/TLS)

## Environment Setup
Set the following environment variables:
```bash
export LANG=en_AU.ISO8859-1
export LC_ALL=en_AU.ISO8859-1
export CHARSET=ISO-8859-1
```

## Clone the Repository
```bash
git clone https://github.com/your-repo/enveng-web-app.git
cd enveng-web-app
```

## Build the Project
### Development Build
```bash
make ENV=dev
```

### Production Build
```bash
make ENV=prod
```

## Install the Application
```bash
make install
```

## Run Tests
```bash
make test
```

## Clean Build Artifacts
```bash
make clean
```

## Post-Installation
- Ensure all configuration files are in place
- Verify the application is running correctly
- Check logs for any errors or warnings
