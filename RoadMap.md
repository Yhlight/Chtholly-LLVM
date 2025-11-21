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
  - [X] Enums.
- [X] **Functions:**
  - [X] Lambda functions.
  - [X] Function types.
- [X] **Classes:**
  - [X] Class definitions with members and methods.
  - [X] Constructors and destructors.
  - [X] `public`/`private` access modifiers.
  - [X] `static` members and methods.
- [X] **Generics:**
  - [X] Generic functions.
  - [X] Generic classes.

## Phase 3: Module System and Standard Library

- [X] **Module System:**
  - [X] Implement `import` for file-based modules.
  - [X] Support for module aliasing (`as`).
  - [X] Package and namespace system (`package`).
- [ ] **Standard Library:**
  - [X] Refactor stdlib into a modular system.
  - [X] Basic I/O (`iostream`).
  - [X] String manipulation utilities (`string`).
  - [X] Math library (`math`).
- [X] **Type System:**
  - [X] Implement safe type casting (`type_cast`).
  - [X] Refine rules for references and ownership (`&`, `*`, `&&`).
