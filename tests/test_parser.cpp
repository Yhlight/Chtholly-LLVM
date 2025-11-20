#include <gtest/gtest.h>
#include "../src/Parser.hpp"
#include "../src/Lexer.hpp"
#include "../src/ASTPrinter.hpp"

using namespace chtholly;

TEST(ParserTest, SimpleBinaryExpression) {
    std::string source = "1 + 2 * 3;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    ASSERT_EQ(stmts.size(), 1);
    auto exprStmt = std::dynamic_pointer_cast<ExpressionStmt>(stmts[0]);
    ASSERT_NE(exprStmt, nullptr);
    ASTPrinter printer;
    std::string result = printer.print(exprStmt->expression);
    EXPECT_EQ(result, "(+ 1 (* 2 3))");
}

TEST(ParserTest, GroupingExpression) {
    std::string source = "(1 + 2) * 3;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    ASSERT_EQ(stmts.size(), 1);
    auto exprStmt = std::dynamic_pointer_cast<ExpressionStmt>(stmts[0]);
    ASSERT_NE(exprStmt, nullptr);
    ASTPrinter printer;
    std::string result = printer.print(exprStmt->expression);
    EXPECT_EQ(result, "(* (group (+ 1 2)) 3)");
}

TEST(ParserTest, UnaryExpression) {
    std::string source = "-5 * !true;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    ASSERT_EQ(stmts.size(), 1);
    auto exprStmt = std::dynamic_pointer_cast<ExpressionStmt>(stmts[0]);
    ASSERT_NE(exprStmt, nullptr);
    ASTPrinter printer;
    std::string result = printer.print(exprStmt->expression);
    EXPECT_EQ(result, "(* (- 5) (! true))");
}

TEST(ParserTest, ComparisonExpression) {
    std::string source = "1 < 2 == 3 > 4;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    ASSERT_EQ(stmts.size(), 1);
    auto exprStmt = std::dynamic_pointer_cast<ExpressionStmt>(stmts[0]);
    ASSERT_NE(exprStmt, nullptr);
    ASTPrinter printer;
    std::string result = printer.print(exprStmt->expression);
    EXPECT_EQ(result, "(== (< 1 2) (> 3 4))");
}

TEST(ParserTest, VarDeclaration) {
    std::string source = "let x = 10;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    ASSERT_EQ(stmts.size(), 1);
    ASTPrinter printer;
    std::string result = printer.print(stmts);
    EXPECT_EQ(result, "(var x 10)\n");
}

TEST(ParserTest, FunctionDeclaration) {
    std::string source = "fn add(a, b) { return a + b; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    ASSERT_EQ(stmts.size(), 1);
    ASTPrinter printer;
    std::string result = printer.print(stmts);
    EXPECT_EQ(result, "(fun add (a b) (block (return (+ a b))))\n");
}

TEST(ParserTest, ReturnStatement) {
    std::string source = "return 123;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    ASSERT_EQ(stmts.size(), 1);
    ASTPrinter printer;
    std::string result = printer.print(stmts);
    EXPECT_EQ(result, "(return 123)\n");
}
