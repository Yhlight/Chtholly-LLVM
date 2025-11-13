include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(chtholly_tests LexerTest.cpp)

# Link against the chtholly executable's sources
target_link_libraries(chtholly_tests GTest::gtest_main)
target_include_directories(chtholly_tests PUBLIC ../src)
target_sources(chtholly_tests PRIVATE ../src/lexer/Lexer.cpp ../src/lexer/Token.cpp)
