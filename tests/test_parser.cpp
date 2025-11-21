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
    std::string source = "fn add(a: int, b: int): int { return a + b; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    ASSERT_EQ(stmts.size(), 1);
    ASTPrinter printer;
    std::string result = printer.print(stmts);
    EXPECT_EQ(result, "(fun add (a: int, b: int): int (block (return (+ a b))))\n");
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

TEST(ParserTest, IfStatement) {
    std::string source = "if (x > 5) { return true; } else { return false; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    ASSERT_EQ(stmts.size(), 1);
    ASTPrinter printer;
    std::string result = printer.print(stmts);
    EXPECT_EQ(result, "(if (> x 5) (block (return true)) (block (return false)))\n");
}

TEST(ParserTest, WhileStatement) {
    std::string source = "while (x < 10) { x = x + 1; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    ASSERT_EQ(stmts.size(), 1);
    ASTPrinter printer;
    std::string result = printer.print(stmts);
    EXPECT_EQ(result, "(while (< x 10) (block (expr_stmt (= x (+ x 1)))))\n");
}

TEST(ParserTest, ForStatement) {
    std::string source = "for (let i = 0; i < 10; i = i + 1) { }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    ASSERT_EQ(stmts.size(), 1);
    ASTPrinter printer;
    std::string result = printer.print(stmts);
    EXPECT_EQ(result, "(for (var i 0) (< i 10) (= i (+ i 1)) (block))\n");
}

TEST(ParserTest, SwitchStatement) {
    std::string source = R"(
        switch (x) {
            case 1: {
                return 1;
            }
            case 2: {
                return 2;
                break;
            }
            case 3: {
                fallthrough;
            }
            case 4: {
                return 4;
            }
        }
    )";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    ASSERT_EQ(stmts.size(), 1);
    ASTPrinter printer;
    std::string result = printer.print(stmts);
    EXPECT_EQ(result, "(switch x (case 1 (block (return 1))) (case 2 (block (return 2) (break))) (case 3 (block (fallthrough))) (case 4 (block (return 4))))\n");
}

TEST(ParserTest, ArrayDeclaration) {
    std::string source = "let a: int[] = [1, 2, 3]; a[0] = 10;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    ASSERT_EQ(stmts.size(), 2);
    ASTPrinter printer;
    std::string result = printer.print(stmts);
    EXPECT_EQ(result, "(var a : int[] (array 1 2 3))\n(expr_stmt (= (subscript a 0) 10))\n");
}
