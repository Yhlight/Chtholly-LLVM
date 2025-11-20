#include <gtest/gtest.h>
#include "../src/Parser.hpp"
#include "../src/Lexer.hpp"
#include "../src/ASTPrinter.hpp"

using namespace chtholly;

TEST(ParserTest, SimpleBinaryExpression) {
    std::string source = "1 + 2 * 3";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::shared_ptr<Expr> expr = parser.parse();
    ASTPrinter printer;
    std::string result = printer.print(*expr);
    EXPECT_EQ(result, "(+ 1 (* 2 3))");
}

TEST(ParserTest, GroupingExpression) {
    std::string source = "(1 + 2) * 3";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::shared_ptr<Expr> expr = parser.parse();
    ASTPrinter printer;
    std::string result = printer.print(*expr);
    EXPECT_EQ(result, "(* (group (+ 1 2)) 3)");
}

TEST(ParserTest, UnaryExpression) {
    std::string source = "-5 * !true";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::shared_ptr<Expr> expr = parser.parse();
    ASTPrinter printer;
    std::string result = printer.print(*expr);
    EXPECT_EQ(result, "(* (- 5) (! true))");
}

TEST(ParserTest, ComparisonExpression) {
    std::string source = "1 < 2 == 3 > 4";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::shared_ptr<Expr> expr = parser.parse();
    ASTPrinter printer;
    std::string result = printer.print(*expr);
    EXPECT_EQ(result, "(== (< 1 2) (> 3 4))");
}
