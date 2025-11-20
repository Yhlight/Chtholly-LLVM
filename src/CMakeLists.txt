add_library(chtholly STATIC
    Compiler.cpp
    Compiler.hpp
)

add_executable(chtholly_cli
    main.cpp
)

target_link_libraries(chtholly_cli chtholly)
