#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Transpiler.hpp"

void run(const std::string& source) {
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    Transpiler transpiler;
    std::string cppCode = transpiler.transpile(statements);
    std::cout << cppCode << std::endl;
}

void runFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        return;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    run(buffer.str());
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cout << "Usage: chtholly_cli [script]" << std::endl;
        return 64;
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        std::cout << "Usage: chtholly_cli [script]" << std::endl;
    }
    return 0;
}
