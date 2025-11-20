#include <gtest/gtest.h>
#include "../src/Parser.hpp"
#include "../src/ASTPrinter.hpp"

using namespace chtholly;

TEST(ParserTest, VariableDeclaration) {
    std::string source = "let x = 10;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    ASTPrinter printer;
    std::string result = printer.print(statements);
    EXPECT_EQ(result, "(var x 10)");
}

TEST(ParserTest, ExpressionStatement) {
    std::string source = "1 + 2;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    ASTPrinter printer;
    std::string result = printer.print(statements);
    EXPECT_EQ(result, "(; (+ 1 2))");
}

TEST(ParserTest, Assignment) {
    std::string source = "x = 10;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    ASTPrinter printer;
    std::string result = printer.print(statements);
    EXPECT_EQ(result, "(; (= x 10))");
}
