add_library(chtholly
    chtholly.cpp
    Lexer.cpp
    Parser.cpp
    ASTPrinter.cpp
)

add_executable(chtholly_cli main.cpp)

target_link_libraries(chtholly_cli chtholly)
