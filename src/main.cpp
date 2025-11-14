#include <iostream>
#include <vector>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "ASTPrinter.hpp"

int main(int argc, char* argv[]) {
    std::string source = R"(
        let x = 10;
        let y = x + 5;
        print y;
    )";

    // 1. Lexer
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    // 2. Parser
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

    // 3. AST Printer (for verification)
    ASTPrinter astPrinter;
    std::string ast_representation = astPrinter.print(statements);

    std::cout << "--- AST ---" << std::endl;
    std::cout << ast_representation << std::endl;
    std::cout << "-----------" << std::endl;

    return 0;
}
