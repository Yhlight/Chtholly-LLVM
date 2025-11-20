find_package(GTest REQUIRED)

add_executable(chtholly_tests
    test_main.cpp
    test_lexer.cpp
    test_parser.cpp
    test_transpiler.cpp
    test_variables.cpp
    test_errors.cpp
    test_operators.cpp
)

target_link_libraries(chtholly_tests
  PRIVATE
  chtholly
  GTest::gtest
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(chtholly_tests)
