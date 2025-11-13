# Chtholly Language Development RoadMap

This document outlines the development plan for the Chtholly programming language.

## Phase 1: Core Compiler Infrastructure

- [ ] **Project Setup**: Initialize the project with CMake, a C++ compiler, and a basic directory structure.
- [ ] **Build System**: Implement a Python script to automate the build process.
- [ ] **Lexer**: Implement a lexical analyzer to tokenize Chtholly source code.
- [ ] **Parser**: Implement a recursive descent parser to build an Abstract Syntax Tree (AST).
- [ ] **AST**: Define the data structures for the AST.
- [ ] **C++ Transpiler**: Implement a code generator that translates the AST into C++ code.

## Phase 2: Language Features

- [ ] **Variables**: Implement `let` and `mut` variable declarations.
- [ ] **Data Types**: Support for basic data types (`int`, `double`, `char`, `bool`, `string`).
- [ ] **Functions**: Implement function definitions and calls.
- [ ] **Ownership System**: Implement the ownership system with move and copy semantics.
- [ ] **Structs**: Implement `struct` definitions and instantiations.
- [ ] **Control Flow**: Implement `if-else`, `while`, and `for` loops.

## Phase 3: Advanced Features

- [ ] **Generics**: Implement generic functions and structs.
- [ ] **Traits and Impls**: Implement traits for defining shared behavior.
- [ ] **Modules**: Implement a module system with `import` statements.
- [ ] **Standard Library**: Implement a basic standard library with I/O and collection types.
- [ ] **Error Handling**: Implement a robust error handling mechanism.

## Phase 4: Self-Hosting

- [ ] **Self-Hosting Compiler**: Rewrite the Chtholly compiler in Chtholly itself.
