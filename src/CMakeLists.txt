# Create a static library for the core compiler components
add_library(chtholly_lib
    Token.hpp
    Lexer.hpp
    Lexer.cpp
)

# The CLI executable links against the library
add_executable(chtholly_cli main.cpp)
target_link_libraries(chtholly_cli PRIVATE chtholly_lib)
