#include "Chtholly.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Transpiler.hpp"
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

    if (hadError) exit(65);
}

void Chtholly::runPrompt() {
    std::string line;
    std::cout << "> ";
    while (std::getline(std::cin, line)) {
        run(line);
        hadError = false; // Reset error for the next line in REPL
        std::cout << "> ";
    }
}

void Chtholly::run(const std::string& source) {
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

    if (hadError) return;

    Transpiler transpiler;
    std::cout << transpiler.transpile(statements) << std::endl;
}

void Chtholly::report(int line, const std::string& where, const std::string& message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
    hadError = true;
}

} // namespace chtholly
