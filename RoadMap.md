# Chtholly Lang RoadMap

This document outlines the development roadmap for the Chtholly programming language.

## Phase 1: Core Compiler Infrastructure

- [ ] Set up project structure (`src`, `tests`, `examples`)
- [ ] Implement basic CMake build system (`CMakeLists.txt`)
- [ ] Create a Python build script (`build.py`)
- [ ] Implement a command-line interface (CLI) for the compiler
- [ ] Implement a basic lexer to tokenize Chtholly source code
- [ ] Implement a basic parser for a subset of the language (e.g., function definitions, variable declarations)
- [ ] Implement a basic Abstract Syntax Tree (AST)
- [ ] Implement a C++ code generator/transpiler
- [ ] "Hello, World!" test case

## Phase 2: Language Features

- [ ] Implement basic types (`int`, `double`, `bool`, `char`, `string`)
- [ ] Implement variable declarations (`let`, `mut`)
- [ ] Implement function definitions and calls
- [ ] Implement basic operators (+, -, *, /)
- [ ] Implement control flow (if/else, while)
- [ ] Implement ownership and borrowing (`&`, `&mut`)
- [ ] Implement structs
- [ ] Implement enums
- [ ] Implement modules and `import` statement
- [ ] Implement generics
- [ ] Implement traits and constraints

## Phase 3: Standard Library

- [ ] Implement `iostream` module (`print`, `input`)
- [ ] Implement `filesystem` module
- [ ] Implement `math` module
- [ ] Implement `string` methods
- [ ] Implement `array` methods
- [ ] Implement `os` module
- [ ] Implement `random` module
- [ ] Implement `time` module
- [ ] Implement `operator` module
- [ ] Implement `reflect` module
- [ ] Implement `meta` module
- [ ] Implement `util` module

## Phase 4: Advanced Features & Self-Hosting

- [ ] Implement advanced error handling and reporting
- [ ] Implement a more robust test suite
- [ ] Write a portion of the compiler in Chtholly
- [ ] Full self-hosting: Compile the Chtholly compiler with itself
