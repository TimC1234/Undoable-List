# Undoable List

Undoable List is a command-driven, list-like container with built-in undo and redo of previous editing operations. It processes plain-text commands from a file or standard input and applies them deterministically to a persistent in-memory list.

## What it does

Undoable List maintains an editable sequence and a bounded or unbounded history of edits so changes can be rolled back and reapplied. Typical operations include inserting, erasing, updating elements, and querying or printing state, alongside UNDO and REDO that revert or reapply prior edits.

## How to run

- Build
  - Using make:
    - make a2
  - Using g++ directly:
    - g++ -std=c++17 -Wall -Wextra -Werror -Wfatal-errors -Wno-sign-compare -Wnon-virtual-dtor -g a2.cpp -o a2

- Execute
  - From a command file:
    - ./a2 input.txt
  - Interactive (if supported by the build):
    - ./a2
    - Then type commands line-by-line and press Enter

- Example
  - ./a2 commands.txt
  - Output is printed to standard output as operations that produce visible results occur (e.g., PRINT, SIZE, or error messages).

## Commands

The system is designed to parse and execute a sequence of high-level list operations. The exact command names below reflect a standard, minimal interface commonly used with undoable sequence structures; adjust the names if the implementation uses slightly different tokens.

- Mutation
  - PUSH_BACK value
  - PUSH_FRONT value
  - INSERT index value
  - ERASE index
  - SET index value
  - CLEAR
- Queries
  - SIZE
  - EMPTY
  - GET index
  - PRINT
- History
  - UNDO
  - REDO

Index-based commands operate on zero-based indices. Commands are one per line. Whitespace-delimited tokens are expected; values containing spaces must be provided in a manner supported by the parser (e.g., as a single token or quoted, depending on the implementation).

## Input and output

- Input: a plain-text file with one command per line, or interactive input.
- Output: textual responses and diagnostics on standard output:
  - PRINT shows the current list in a compact representation, e.g., [a, b, c]
  - SIZE prints the current length as an integer
  - GET prints the value at the index
  - UNDO/REDO print nothing unless the implementation reports history actions or errors
  - Invalid operations (e.g., out-of-range indices, empty UNDO/REDO history) emit clear error messages

## Data structures and algorithms

- Core container
  - Custom linked list implementing FIFO, random-access via traversal, and stable iterators for edit operations
  - Typical choice: doubly linked list for O(1) insert/erase at known positions; singly linked list if design favors minimal memory and forward traversal
- History model
  - Two-structure history: a “past” stack for undo and a “future” stack for redo
  - Mutation commands push an inverse operation onto “past” and clear “future” (branching semantics); UNDO pops from “past” and applies the inverse while pushing the original onto “future”; REDO pops from “future”, applies it, and pushes the inverse onto “past”
- Complexity
  - PUSH_FRONT/PUSH_BACK: O(1)
  - INSERT/ERASE at index: O(n) traversal + O(1) link fix-up (with doubly linked list)
  - SET/GET at index: O(n) traversal
  - UNDO/REDO of recorded operations: O(1) to dispatch + cost of the underlying inverse mutation (e.g., O(1) for push/pop-like actions; O(n) if index traversal is required)
- Error handling
  - Robust checks for out-of-range indices, empty structure conditions, and empty history stacks

## Implementation details

- Language and standard: C++17
- Project layout: single-file or few-file solution with all logic in a2.cpp or split into minimal headers for the list and history primitives
- Parsing: token-based parsing of lines with straightforward dispatch on the first token
- Immutability and inverses: edits are captured as self-contained operation records that include all data required to invert (e.g., original value, index, and action type)
- Memory management: manual allocation/deallocation for list nodes; RAII scoping and destructors ensure no leaks
- Defensive programming: runtime checks with meaningful messages; assertions in internal helpers where appropriate

## Examples

- Example input
  - PUSH_BACK apple
  - PUSH_BACK banana
  - INSERT 1 cherry
  - PRINT
  - UNDO
  - PRINT
  - REDO
  - SET 2 durian
  - GET 2
  - SIZE
  - ERASE 0
  - PRINT

- Example output
  - [apple, cherry, banana]
  - [apple, banana]
  - durian
  - 3
  - [banana, durian]

## Notes

- History capacity can be fixed or unbounded. If fixed, the oldest entries are dropped when the limit is reached.
- After any new mutation, redo history is cleared to ensure correct branching semantics.
- For large lists or frequent indexed operations, consider augmenting with cached cursors or a skip structure if required by performance goals.

## Build configuration

A typical make target:
- a2: builds the executable with C++17, warnings-as-errors, and debug symbols

A typical g++ invocation:
- g++ -std=c++17 -Wall -Wextra -Werror -Wfatal-errors -Wno-sign-compare -Wnon-virtual-dtor -g a2.cpp -o a2
