# Undoable List - Dynamic String List with Undo Functionality

An advanced dynamic string list implementation featuring comprehensive undo capabilities. This C++ data structure allows users to perform various list operations and reverse them using a sophisticated command-based undo system.

## What it Does

The Undoable List is a dynamic array-based string container that maintains a complete history of modifications, enabling users to undo any operation that changes the list's state. The system supports all standard list operations while providing the ability to reverse changes in LIFO (Last In, First Out) order.

## Key Features

- **Dynamic Array Management**: Automatic capacity expansion when needed
- **Complete Undo System**: All modifying operations can be undone
- **Memory Efficient**: Stack-based command storage for undo operations
- **Safe Operations**: Bounds checking with exception handling
- **Copy Semantics**: Proper copy constructor and assignment operator (undo stack not copied)

## How to Run

### Compilation
```bash
make Stringlist_test    # Run original functionality tests
make a2_test           # Run undo functionality tests
```

Or manually compile:
```bash
g++ -std=c++17 -Wall -Wextra -Werror -Wfatal-errors -Wno-sign-compare -Wnon-virtual-dtor -g Stringlist_test.cpp -o Stringlist_test
g++ -std=c++17 -Wall -Wextra -Werror -Wfatal-errors -Wno-sign-compare -Wnon-virtual-dtor -g a2_test.cpp -o a2_test
```

### Execution
```bash
./Stringlist_test      # Test basic functionality
./a2_test             # Test undo functionality
valgrind ./a2_test    # Test with memory leak detection
```

### Example Usage
```cpp
#include "Stringlist.h"
#include <iostream>

int main() {
    Stringlist lst;
    
    // Add elements
    lst.insert_back("one");
    lst.insert_back("two");
    lst.insert_back("three");
    // lst == {"one", "two", "three"}
    
    // Undo operations
    lst.undo(); // removes "three"
    lst.undo(); // removes "two"
    lst.undo(); // removes "one"
    // lst == {}
    
    return 0;
}
```

## Supported Operations

### Undoable Operations
All of these operations can be reversed using `undo()`:

1. **insert_before(index, string)** - Insert string at specified position
2. **insert_back(string)** - Append string to end of list
3. **insert_front(string)** - Insert string at beginning of list
4. **remove_at(index)** - Remove string at specified position
5. **remove_all()** - Clear all strings from list
6. **remove_first(string)** - Remove first occurrence of string
7. **set(index, string)** - Replace string at specified position
8. **operator=(other)** - Assign another list (undo restores original state)

### Non-Undoable Operations
- **undo()** - Reverse the most recent undoable operation
- **get(index)** - Retrieve string at position
- **size()** - Get number of elements
- **empty()** - Check if list is empty
- **contains(string)** - Check if string exists in list
- **index_of(string)** - Find position of string

## Data Structures and Algorithms

### Core Data Structure
- **Dynamic Array**: Resizable string array with automatic capacity doubling
- **Capacity Management**: Starts with capacity 10, doubles when full
- **Efficient Access**: O(1) random access to elements

### Undo Stack Implementation
- **Stack Structure**: Singly-linked list implementation
- **Command Storage**: String-based command format for space efficiency
- **Memory Management**: Automatic cleanup of undo history

#### Stack Node Structure
```cpp
struct Stack {
    string action;    // Command string for undo operation
    Stack *next;      // Pointer to next command
    bool undone;      // Flag to track processed commands
};
```

### Undo Command Format

The system uses a compact string-based command format:

- **INSERT operations**: `"REMOVE index"` - Stores removal command
- **REMOVE operations**: `"INSERT index value"` - Stores insertion command  
- **SET operations**: `"SET index original_value"` - Stores restoration command
- **Assignment**: `"set original_values"` or `"Empty "` - Stores complete state
- **Remove All**: `"Set original_values"` - Stores complete restoration

### Algorithm Complexity

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Insert (amortized) | O(1) | O(1) |
| Remove | O(n) | O(1) |
| Set | O(1) | O(1) |
| Undo | O(n) | O(1) |
| Assignment | O(n) | O(n) |

### Key Implementation Details

**Command Parsing**: Uses `substr()` and `stoi()` for efficient string parsing and command extraction

**Memory Management**: 
- Dynamic node allocation for stack operations
- Proper cleanup in destructor prevents memory leaks
- Undo stack cleared when undone commands are no longer needed

**String Concatenation**: Special `toLongString()` helper for efficient state serialization during bulk operations

**Edge Case Handling**:
- Empty list assignments tracked with special "Empty" command
- Self-assignment detection prevents unnecessary operations
- Bounds checking with descriptive error messages

## Technical Implementation

- **Language**: C++17
- **Memory Model**: RAII principles with manual linked list management
- **Error Handling**: Exception-based bounds checking
- **String Processing**: STL string operations with custom parsing logic
- **Copy Semantics**: Deep copy of data array, shallow copy of undo stack (by design)

## Memory Safety

- **Valgrind Clean**: No memory leaks or errors under normal operation
- **RAII Compliance**: Automatic resource cleanup in destructors
- **Exception Safety**: Proper cleanup on bounds errors
- **Iterator Safety**: No dangling pointers in stack operations

## Testing

The implementation includes comprehensive test suites:

- **Basic Functionality**: Tests all core operations without undo
- **Undo Functionality**: Tests all undoable operations and edge cases
- **Memory Safety**: Valgrind testing for leak detection
- **Edge Cases**: Empty lists, self-assignment, and boundary conditions
