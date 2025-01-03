# Testing Guide

## Unit Testing
1. **Write Unit Tests**:
   - Place unit tests in the `test/unit` directory.
   - Use the `CUnit` framework for writing tests.

2. **Run Unit Tests**:
   ```sh
   make test
   ```

## Integration Testing
1. **Write Integration Tests**:
   - Place integration tests in the `test/integration` directory.

2. **Run Integration Tests**:
   ```sh
   make test-integration
   ```

## Performance Testing
1. **Write Performance Tests**:
   - Place performance tests in the `test/performance` directory.

2. **Run Performance Tests**:
   ```sh
   make test-performance
   ```

## Security Testing
1. **Write Security Tests**:
   - Place security tests in the `test/security` directory.

2. **Run Security Tests**:
   ```sh
   make test-security
   ```

## Continuous Integration
- **GitHub Actions**: Ensure all tests are run automatically on each push and pull request.
