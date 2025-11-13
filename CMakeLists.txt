cmake_minimum_required(VERSION 3.10)
project(Chtholly)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Add the source directory
add_subdirectory(src)

# Enable testing with CTest
enable_testing()
add_subdirectory(tests)
