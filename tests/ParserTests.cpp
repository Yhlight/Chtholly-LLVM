#include <gtest/gtest.h>
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/ASTPrinter.hpp"
#include <vector>
#include <string>

using namespace chtholly;

TEST(ParserTest, TestSimpleExpression) {
    std::string source = "1 + 2 * 3";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    Parser parser(tokens);
    auto stmts = parser.parse();

    ASTPrinter printer;
    std::string result = printer.print(stmts);

    EXPECT_EQ(result, "(; (+ 1 (* 2 3)))");
}

TEST(ParserTest, TestParenthesizedExpression) {
    std::string source = "(1 + 2) * 3";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    Parser parser(tokens);
    auto stmts = parser.parse();

    ASTPrinter printer;
    std::string result = printer.print(stmts);

    EXPECT_EQ(result, "(; (* (group (+ 1 2)) 3))");
}

TEST(ParserTest, TestUnaryExpression) {
    std::string source = "-5";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    Parser parser(tokens);
    auto stmts = parser.parse();

    ASTPrinter printer;
    std::string result = printer.print(stmts);

    EXPECT_EQ(result, "(; (- 5))");
}

TEST(ParserTest, TestComparisonExpression) {
    std::string source = "1 < 2 == 3 > 4";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    Parser parser(tokens);
    auto stmts = parser.parse();

    ASTPrinter printer;
    std::string result = printer.print(stmts);

    EXPECT_EQ(result, "(; (== (< 1 2) (> 3 4)))");
}
