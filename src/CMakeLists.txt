# Define the executable for the Chtholly compiler
add_executable(chtholly main.cpp lexer/Lexer.cpp lexer/Token.cpp)

target_include_directories(chtholly PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
