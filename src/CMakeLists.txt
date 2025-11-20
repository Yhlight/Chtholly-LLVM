add_library(chtholly STATIC
    Compiler.cpp
    Compiler.hpp
    Lexer.cpp
    Lexer.hpp
    Token.hpp
    AST.hpp
)

add_executable(chtholly_cli
    main.cpp
)

target_link_libraries(chtholly_cli chtholly)
