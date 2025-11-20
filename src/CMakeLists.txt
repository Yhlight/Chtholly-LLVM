add_library(chtholly STATIC
    Compiler.cpp
    Compiler.hpp
    Lexer.cpp
    Lexer.hpp
    Token.hpp
    AST.hpp
    ASTPrinter.cpp
    ASTPrinter.hpp
    Parser.cpp
    Parser.hpp
)

add_executable(chtholly_cli
    main.cpp
)

target_link_libraries(chtholly_cli chtholly)
