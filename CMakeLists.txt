cmake_minimum_required(VERSION 3.10)
project(Chtholly)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the source directory for include headers
include_directories(src)

# Create a library for the compiler components
add_library(chtholly STATIC src/lib.cpp src/Lexer.cpp)

# Add the command-line interface
add_executable(chtholly_cli src/main.cpp)
target_link_libraries(chtholly_cli chtholly)

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Add the tests subdirectory
add_subdirectory(tests)

# Link tests against Google Test
