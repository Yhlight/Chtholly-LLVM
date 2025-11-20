#include "Chtholly.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    chtholly::Chtholly chtholly;

    if (argc > 2) {
        std::cout << "Usage: chtholly [script]" << std::endl;
        return 64; // According to UNIX sysexits.h
    } else if (argc == 2) {
        chtholly.runFile(argv[1]);
    } else {
        chtholly.runPrompt();
    }

    return 0;
}
