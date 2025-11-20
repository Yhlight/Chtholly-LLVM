#include <iostream>
#include <fstream>
#include <sstream>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Transpiler.hpp"

void run(const std::string& source) {
    chtholly::Lexer lexer(source);
    std::vector<chtholly::Token> tokens = lexer.scan_tokens();

    chtholly::Parser parser(tokens);
    std::vector<std::shared_ptr<chtholly::Stmt>> statements = parser.parse();

    chtholly::Transpiler transpiler;
    std::cout << transpiler.transpile(statements) << std::endl;
}

void run_file(const std::string& path) {
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
        run_file(argv[1]);
    } else {
        std::cout << "Usage: chtholly_cli [script]" << std::endl;
    }

    return 0;
}
