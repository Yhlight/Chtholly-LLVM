cmake_minimum_required(VERSION 3.10)

project(Chtholly CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(chtholly src/main.cpp src/Lexer.cpp src/Parser.cpp src/ASTPrinter.cpp src/Transpiler.cpp)

enable_testing()

# Example of how to add a test
# add_test(NAME MyTest COMMAND chtholly_tests)
# add_executable(chtholly_tests tests/test_main.cpp)
# target_link_libraries(chtholly_tests PRIVATE chtholly)
