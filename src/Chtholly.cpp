#include "Chtholly.hpp"
#include "Lexer.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace chtholly {

void Chtholly::runFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Could not open file: " << path << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    run(buffer.str());
}

void Chtholly::runPrompt() {
    std::string line;
    std::cout << "> ";
    while (std::getline(std::cin, line)) {
        run(line);
        std::cout << "> ";
    }
}

void Chtholly::run(const std::string& source) {
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    for (const auto& token : tokens) {
        std::cout << token.lexeme << std::endl;
    }
}

} // namespace chtholly
