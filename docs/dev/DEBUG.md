# Debugging Guide

## Setting Up the Debug Environment
1. **Build with Debug Symbols**:
   ```sh
   make ENV=dev
   ```

2. **Run the Application with GDB**:
   ```sh
   gdb ./bin/enssol
   ```

3. **Set Breakpoints**:
   ```gdb
   (gdb) break main
   ```

4. **Run the Application**:
   ```gdb
   (gdb) run
   ```

5. **Inspect Variables**:
   ```gdb
   (gdb) print variable_name
   ```

6. **Step Through Code**:
   ```gdb
   (gdb) step
   ```

## Memory Analysis with Valgrind
1. **Run the Application with Valgrind**:
   ```sh
   valgrind --leak-check=full ./bin/enssol
   ```

2. **Analyze the Output**:
   - Look for memory leaks and invalid memory accesses.

## Common Debugging Tips
- **Check Return Values**: Always check the return values of system calls and library functions.
- **Use Logging**: Add logging statements to trace the execution flow.
- **Validate Inputs**: Ensure all inputs are validated to prevent unexpected behavior.
