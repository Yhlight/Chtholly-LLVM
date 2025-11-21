add_executable(chtholly_tests
  test_lexer.cpp
  test_parser.cpp
  test_transpiler.cpp
  test_class.cpp
  test_generics.cpp
  test_module.cpp
  test_stdlib.cpp
  test_type_system.cpp
  test_string.cpp
  test_math.cpp
)

# Link against the core library and Google Test
target_link_libraries(chtholly_tests PRIVATE chtholly_lib gtest_main)

# Add the src directory to the include path
target_include_directories(chtholly_tests PRIVATE ../src)

include(GoogleTest)
gtest_discover_tests(chtholly_tests)
