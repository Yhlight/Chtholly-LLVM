include(GoogleTest)

add_executable(chtholly_tests test_main.cpp)
target_link_libraries(chtholly_tests PRIVATE gtest_main chtholly)

gtest_discover_tests(chtholly_tests)
