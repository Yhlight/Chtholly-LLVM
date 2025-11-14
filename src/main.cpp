#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Transpiler.hpp"

void runFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Could not open file: " << path << std::endl;
        return;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    // 1. Lexer
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    // 2. Parser
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

    // 3. Transpiler
    Transpiler transpiler;
    std::string cpp_code = transpiler.transpile(statements);

    std::cout << cpp_code << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: chtholly [script]" << std::endl;
        return 1;
    }

    runFile(argv[1]);

    return 0;
}
