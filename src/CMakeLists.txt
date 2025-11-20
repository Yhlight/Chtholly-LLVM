add_library(chtholly
    Lexer.cpp
    Parser.cpp
    ASTPrinter.cpp
    Transpiler.cpp
)

add_executable(chtholly_cli main.cpp)

target_link_libraries(chtholly_cli chtholly)
