add_library(chtholly STATIC
    Token.hpp
    Lexer.hpp
    Lexer.cpp
)

add_executable(chtholly_cli
    main.cpp
)

target_link_libraries(chtholly_cli PRIVATE chtholly)
