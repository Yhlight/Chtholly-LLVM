include(GoogleTest)

add_executable(chtholly_tests LexerTests.cpp ParserTests.cpp)
target_link_libraries(chtholly_tests PRIVATE gtest_main chtholly)

gtest_discover_tests(chtholly_tests)
