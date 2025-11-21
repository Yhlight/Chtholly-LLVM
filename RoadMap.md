# Chtholly Language Development Roadmap

## Phase 1: Core Compiler Infrastructure

- [X] **Project Setup:**
  - [X] Initialize Git repository.
  - [X] Set up CMake build system.
  - [X] Create `build.py` script for automation.
  - [X] Configure Google Test for unit testing.
- [X] **Lexer (Tokenizer):**
  - [X] Implement token definitions for all language constructs.
  - [X] Handle basic literals (integers, doubles, strings, booleans).
  - [X] Support single-line and multi-line comments.
- [X] **Parser (AST Generation):**
  - [X] Implement basic AST node structures.
  - [X] Parse variable declarations (`let`, `mut`).
  - [X] Parse function definitions (`fn`).
  - [X] Parse basic expressions and operators.
- [X] **Transpiler (C++ Code Generation):**
  - [X] Implement a visitor pattern for the AST.
  - [X] Transpile basic expressions and variable declarations.
  - [X] Transpile function definitions.
  - [X] Generate a runnable C++ `main` function from `fn main`.
- [X] **Type System Foundation:**
  - [X] Implement basic type representation (`int`, `double`, `string`, `bool`).
  - [X] Support for array types (`int[]`).
  - [X] Support for typed function signatures.

## Phase 2: Language Features

- [X] **Control Flow:**
  - [X] `if`/`else if`/`else` statements.
  - [X] `while` loops.
  - [X] `for` loops (C-style and range-based).
  - [X] `switch` statements.
- [X] **Data Structures:**
  - [X] Arrays (static and dynamic).
  - [ ] Enums.
- [ ] **Functions:**
  - [ ] Lambda functions.
  - [ ] Function types.
- [ ] **Classes:**
  - [ ] Class definitions with members and methods.
  - [ ] Constructors and destructors.
  - [ ] `public`/`private` access modifiers.
  - [ ] `static` members and methods.
- [ ] **Generics:**
  - [ ] Generic functions.
  - [ ] Generic classes.

## Phase 3: Module System and Standard Library

- [ ] **Module System:**
  - [ ] Implement `import` for file-based modules.
  - [ ] Support for module aliasing (`as`).
  - [ ] Package and namespace system (`package`).
- [ ] **Standard Library:**
  - [ ] Basic I/O (`iostream`).
  - [ ] String manipulation utilities.
  - [ ] Math library.
- [ ] **Type System:**
  - [ ] Implement safe type casting (`type_cast`).
  - [ ] Refine rules for references and ownership (`&`, `*`, `&&`).
