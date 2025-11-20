add_library(chtholly STATIC
    # Add your source files here
)

add_executable(chtholly_cli
    main.cpp
)

target_link_libraries(chtholly_cli PRIVATE chtholly)
