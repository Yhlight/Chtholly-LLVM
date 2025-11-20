# Chtholly Language Development Roadmap

This document outlines the development plan for the Chtholly programming language. The development is divided into several phases, each focusing on a specific set of features and milestones.

## Phase 1: Core Compiler Infrastructure (In Progress)

This phase focuses on setting up the basic infrastructure for the compiler.

- [x] Project Setup (CMake, Python build script)
- [x] Lexer (Tokenization)
- [x] Parser (AST Generation)
- [x] C++ Transpiler (Code Generation)
- [ ] Basic CLI (Command Line Interface)
- [ ] Testing Framework Integration (Google Test)

## Phase 2: Language Features

This phase will implement the core features of the Chtholly language.

- [ ] Variables (let, mut, type inference, type annotations)
- [ ] Basic Data Types (int, double, bool, char, string)
- [ ] Operators
- [ ] Control Flow (if/else, while, for)
- [ ] Functions (including parameters and return types)
- [ ] Structs (including methods and member variables)
- [ ] Enums
- [ ] Arrays (static and dynamic)

## Phase 3: Standard Library

This phase will focus on developing a basic standard library.

- [ ] I/O Module (iostream)
- [ ] String Manipulation
- [ ] Math Library
- [ ] Collection Types (Vector, Map)

## Phase 4: Advanced Features

This phase will add more advanced features to the language.

- [ ] Generics (Functions and Structs)
- [ ] Module System (import, package)
- [ ] Memory Management Features (move, clone)
- [ ] Error Handling

## Phase 5: Optimization and Performance

This phase will focus on improving the performance of the transpiled C++ code.

- [ ] Code Generation Optimizations
- [ ] Benchmarking

## Phase 6: Community and Tooling

This phase will focus on building a community and providing better tooling.

- [ ] Documentation
- [ ] Package Manager
- [ ] Formatter
- [ ] Language Server Protocol (LSP) integration
