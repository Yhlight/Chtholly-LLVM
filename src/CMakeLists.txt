# Create a static library for the core compiler components
add_library(chtholly_lib
    Token.hpp
    Lexer.hpp
    Lexer.cpp
    AST.hpp
    ASTPrinter.hpp
    ASTPrinter.cpp
    Parser.hpp
    Parser.cpp
    Transpiler.hpp
    Transpiler.cpp
    Stdlib.hpp
    Stdlib.cpp
    stdlib/iostream.hpp
    stdlib/iostream.cpp
    stdlib/string.hpp
    stdlib/string.cpp
    stdlib/math.hpp
    stdlib/math.cpp
)

# The CLI executable links against the library
add_executable(chtholly_cli main.cpp)
target_link_libraries(chtholly_cli PRIVATE chtholly_lib)
