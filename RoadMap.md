# RoadMap

## Phase 1: Core Compiler Infrastructure
- [x] Set up project structure (`src`, `tests`, `build`).
- [x] Integrate CMake build system.
- [x] Create a Python build script (`build.py`).
- [x] Integrate Google Test for unit testing.
- [ ] Implement a basic command-line interface (`chtholly_cli`).
- [ ] Implement the Lexer for tokenizing Chtholly source code.
- [ ] Implement the Parser for building the Abstract Syntax Tree (AST).
- [ ] Implement the C++ Transpiler using the Visitor pattern.
- [ ] Implement an ASTPrinter for debugging.

## Phase 2: Language Features
- [ ] **Control Flow:**
  - [ ] `if-else` statements.
  - [ ] `while` loops.
  - [ ] `for` loops (C-style and for-each).
- [ ] **Data Types:**
  - [ ] Basic types (int, double, bool, char, string).
  - [ ] Arrays (static and dynamic).
- [ ] **Functions:**
  - [ ] Function definitions and calls.
  - [ ] Return statements.
- [ ] **Structs and Methods:**
  - [ ] Struct definitions.
  - [ ] Member fields.
  - [ ] Member methods (`self`, `&self`, `&mut self`).
  - [ ] Static methods.
  - [ ] Struct instantiation.
- [ ] **Variables:**
  - [ ] `let` and `mut` declarations.
  - [ ] Type inference and optional type annotations.
- [ ] **Modules:**
  - [ ] `import` statements for file-based modules.
- [ ] **Advanced Features (Future):**
  - [ ] Generics.
  - [ ] Enums.
  - [ ] Pattern matching (`switch`).
  - [ ] Standard library modules.
  - [ ] Package management.
