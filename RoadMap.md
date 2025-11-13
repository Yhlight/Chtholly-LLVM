# Chtholly Language Development RoadMap

This document outlines the development plan and progress for the Chtholly programming language.

## Phase 1: Core Compiler Infrastructure

- [x] **Project Setup**: Initialize the project structure, build system (CMake), and version control (.gitignore).
- [ ] **Lexer (Tokenizer)**: Implement the lexical analyzer to convert source code into a stream of tokens. (In Progress)
- [ ] **Parser (AST)**: Implement the recursive descent parser to build an Abstract Syntax Tree (AST) from the token stream.
- [ ] **C++ Transpiler**: Implement the basic mechanism to traverse the AST and generate equivalent C++ code.
- [ ] **Basic Language Features**:
    - [ ] Variables (`let`, `mut`)
    - [ ] Basic types (`int`, `double`, `bool`, `string`)
    - [ ] Function definitions (`func`)
    - [ ] `main` function as the entry point.
    - [ ] Basic operators (+, -, *, /)
    - [ ] `print` and `input` built-in functions.

## Phase 2: Intermediate Language Features

- [ ] Control Flow (`if`, `else`, `while`, `for`)
- [ ] Ownership and Borrowing (`&`, `&mut`)
- [ ] Structs (`struct`) and member access
- [ ] Arrays (`array`)
- [ ] Scoping and Environments

## Phase 3: Advanced Language Features

- [ ] Generics (`<T>`)
- [ ] Traits and Constraints (`impl`, `?`)
- [ ] Modules and Imports (`import`)
- [ ] Standard Library Modules (`iostream`, `filesystem`, `math`, etc.)
- [ ] Error Handling (`option<T>`)

## Phase 4: Self-Hosting and Optimization

- [ ] Self-hosting compiler
- [ ] Performance optimizations

## Phase 5: Tooling and Ecosystem

- [ ] Language Server Protocol (LSP) for IDE support
- [ ] Package manager
- [ ] Formatter
