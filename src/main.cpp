#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "ASTPrinter.hpp"

void run(const std::string& source) {
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

    if (!parser.had_error) {
        ASTPrinter printer;
        std::cout << printer.print(statements) << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <source_file>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    run(source);

    return 0;
}
