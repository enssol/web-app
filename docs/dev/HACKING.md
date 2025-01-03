# Hacking Guide

## Overview
This document provides guidelines for contributing to the EnvEng Web Application Development project.

## Getting Started
1. Fork the repository on GitHub
2. Clone your forked repository
```bash
git clone https://github.com/your-username/enveng-web-app.git
cd enveng-web-app
```

## Development Environment
- Use GCC with musl libc for compilation
- Set environment variables for ISO-8859-1 encoding
- Follow the project's coding standards and style guidelines

## Making Changes
1. Create a new branch for your changes
```bash
git checkout -b feature-branch
```
2. Make your changes and commit them
```bash
git add .
git commit -m "[Type] Short description"
```
3. Push your changes to your forked repository
```bash
git push origin feature-branch
```
4. Create a pull request on GitHub

## Code Quality
- Ensure your code adheres to the project's coding standards
- Write unit tests for new functionality
- Run all tests and ensure they pass
- Use static analysis tools to check for coding violations

## Documentation
- Update documentation for any new features or changes
- Ensure all comments and documentation are clear and concise

## Submitting a Pull Request
1. Ensure your branch is up-to-date with the main branch
```bash
git checkout main
git pull origin main
git checkout feature-branch
git rebase main
```
2. Create a pull request on GitHub
3. Provide a clear description of your changes
4. Address any feedback or requested changes

## Contact
For any questions or issues, please refer to the [Support Information](../user/SUPPORT.md).
