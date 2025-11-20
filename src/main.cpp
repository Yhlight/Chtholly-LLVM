#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Transpiler.hpp"

void run(const std::string& source) {
    chtholly::Lexer lexer(source);
    std::vector<chtholly::Token> tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    // Check for parsing errors
    if (stmts.empty() && !tokens.empty()) {
        // A simple check; more robust error handling can be added later
        std::cerr << "Parsing failed." << std::endl;
        return;
    }

    chtholly::Transpiler transpiler;
    std::string output = transpiler.transpile(stmts);
    std::cout << output;
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
        return 64; // Exit code for command line usage error
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        // No file provided, we could run a REPL in the future
        std::cout << "Usage: chtholly_cli [script]" << std::endl;
    }

    return 0;
}
