add_library(chtholly
    Lexer.cpp
    Parser.cpp
    AST.cpp
    Chtholly.cpp
    ASTPrinter.cpp
)

add_executable(chtholly_cli main.cpp)

target_link_libraries(chtholly_cli chtholly)
