# C Style Guide for hyper Project

This guide outlines the coding standards and best practices for the hyper project. It aims to ensure code consistency, memory safety, efficiency, and maintainability across the codebase.

## Table of Contents
1. [Code Formatting](#code-formatting)
2. [Naming Conventions](#naming-conventions)
3. [Memory Safety](#memory-safety)
4. [Header Files](#header-files)
5. [Functions](#functions)
6. [Comments](#comments)
7. [Error Handling](#error-handling)
8. [Types and Variables](#types-and-variables)

## Code Formatting

### Line Length
- Maximum line length is 80 characters
- Exceptions allowed for:
  - Long string literals that cannot be reasonably split
  - Complex macro definitions
  - URLs in comments

### Indentation and Spacing
- Use spaces for indentation, not tabs
- Indent with 4 spaces (standard for C projects)
- No trailing whitespace
- One space after keywords (if, for, while, switch)
- One space around operators (=, +, -, <, >, etc.)
- No space between function name and opening parenthesis
- One space after commas in parameter lists

### Braces
```c
if (condition) {
    // code
} else {
    // code
}

while (condition) {
    // code
}

for (int i = 0; i < n; i++) {
    // code
}
```

- Opening brace on the same line as control statement
- Closing brace on its own line
- Always use braces for control statements, even for single-line blocks

## Naming Conventions

### Files
- Use lowercase with underscores
- Source files: `.c` extension
- Header files: `.h` extension
- Names should be descriptive and specific
- Examples: `http_server.c`, `memory_pool.h`

### Functions
- Use snake_case
- Prefix with module name for better scoping
- Clear and descriptive names
- Examples: `http_parse_request()`, `client_send_message()`

### Variables
- Use snake_case
- Descriptive names that indicate purpose
- Loop indices can use `i`, `j`, `k`
- Avoid abbreviations unless widely known

### Constants and Macros
- Use UPPERCASE with underscores
- Examples: `MAX_BUFFER_SIZE`, `HTTP_PORT`

### Types
- Use PascalCase for typedef'd types
- End struct typedefs with `_t`
- Examples: `HttpRequest_t`, `ClientConfig_t`

## Memory Safety

### Allocation
- Always check return values of malloc/calloc
- Use wrapper functions for memory allocation
- Track memory ownership clearly
- Free memory at the same level it was allocated

### Buffer Operations
- Use bounded string functions (snprintf, strncpy)
- Always specify buffer sizes
- Check return values
- Avoid direct pointer arithmetic when possible

### Resource Management
- Follow RAII-like patterns where possible
- Clean up resources in reverse order of acquisition
- Use consistent patterns for resource handling

## Header Files

### Header Guards
```c
#ifndef HYPER_MODULE_NAME_H
#define HYPER_MODULE_NAME_H

// declarations

#endif /* HYPER_MODULE_NAME_H */
```

### Organization
- Public API first
- Internal functions/types last
- Group related declarations together
- Minimize includes in headers

### Include Order
1. Standard library headers
2. System headers
3. Third-party library headers
4. Project headers

## Functions

### Design
- Single responsibility principle
- Maximum of 40 lines recommended
- Minimize side effects
- Clear input/output parameters

### Parameters
- Maximum 5 parameters recommended
- Use structs for many related parameters
- Document parameter constraints
- Validate input parameters

### Return Values
- Consistent error handling
- Return error codes for failures
- Document return value meanings
- Use output parameters for multiple returns

## Comments

### Function Comments
```c
/**
 * Brief description of the function.
 *
 * Detailed description if needed.
 *
 * @param param1 Description of first parameter
 * @param param2 Description of second parameter
 * @return Description of return value
 * @note Any special notes about the function
 */
```

### File Comments
```c
/**
 * @file filename.c
 * @brief Brief description of the file
 *
 * Detailed description of the file's purpose and contents.
 */
```

### Implementation Comments
- Explain complex algorithms
- Document non-obvious decisions
- Avoid obvious comments
- Keep comments up to date

## Error Handling

### Error Codes
- Use consistent error codes
- Document error conditions
- Return negative values for errors
- Zero for success

### Error Reporting
- Clear error messages
- Include context in errors
- Log errors appropriately
- Clean up resources on error

## Types and Variables

### Type Safety
- Use appropriate types for data
- Avoid type casting when possible
- Use size_t for sizes
- Use fixed-width types when needed

### Variable Declaration
- One variable per line
- Initialize at declaration when possible
- Use const when appropriate
- Minimize variable scope

### Integer Types
- Use `size_t` for sizes and counts
- Use `uint32_t`, `int64_t` etc. for fixed width
- Use `int` for small numbers and loop indices
- Document assumptions about ranges

## Best Practices

1. **Security**
   - Validate all input
   - Check buffer boundaries
   - Avoid dangerous functions
   - Use secure alternatives

2. **Performance**
   - Profile before optimizing
   - Consider cache effects
   - Minimize allocations
   - Use appropriate data structures

3. **Maintainability**
   - Write self-documenting code
   - Keep functions and files focused
   - Document trade-offs and decisions
   - Write with future maintainers in mind

4. **Testing**
   - Write testable code
   - Create unit tests
   - Test error conditions
   - Document test cases

---

This guide was generated by Claude 3.5 Sonnet, based on Google's C++ conventions. After I write code for this project, I ask the AI how I can improve it to better follow these guidelines. Beyond autocomplete, I do not use AI outputs for writing code. Rather, I use it simply as a way to scrutinize my code.