cmake_minimum_required(VERSION 3.14)

project(Chtholly CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Library ---
# Create a library from the core compiler source files.
# This allows it to be used by both the main executable and the tests.
add_library(chtholly
    src/Lexer.cpp
    src/Lexer.hpp
    src/Parser.cpp
    src/Parser.hpp
    src/Token.hpp
    src/AST.hpp
    src/ASTPrinter.cpp
    src/ASTPrinter.hpp
    src/Transpiler.cpp
    src/Transpiler.hpp
)

# --- Main Executable ---
# This is the command-line interface for the compiler.
add_executable(chtholly_cli src/main.cpp)
target_link_libraries(chtholly_cli PRIVATE chtholly)


# --- Google Test ---
# Download and build GoogleTest as part of the project.
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# --- Testing ---
enable_testing()

# Define the test executable
add_executable(chtholly_tests
    tests/test_parser.cpp
)

# Link the test executable against our library and Google Test
target_link_libraries(chtholly_tests PRIVATE chtholly GTest::gtest_main)

# Automatically discover and add tests to CTest
include(GoogleTest)
gtest_discover_tests(chtholly_tests)
