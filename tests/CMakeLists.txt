add_executable(chtholly_tests
    LexerTests.cpp
    ParserTests.cpp
    TranspilerTests.cpp
)

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG    release-1.12.1
)
FetchContent_MakeAvailable(googletest)

target_link_libraries(chtholly_tests chtholly gtest_main)
