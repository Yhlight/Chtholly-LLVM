#include <gtest/gtest.h>
#include "Parser.hpp"
#include "Lexer.hpp"
#include "ASTPrinter.hpp"

TEST(ParserTest, ParsesSimpleExpression) {
    std::string source = "let x = 1 + 2;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

    ASSERT_EQ(statements.size(), 1);
    auto varStmt = std::dynamic_pointer_cast<VarStmt>(statements[0]);
    ASSERT_NE(varStmt, nullptr);
    EXPECT_EQ(varStmt->name.lexeme, "x");

    ASTPrinter printer;
    std::string result = printer.print(varStmt->initializer);
    EXPECT_EQ(result, "(+ 1.000000 2.000000)");
}

TEST(ParserTest, ParsesExpressionWithPrecedence) {
    std::string source = "let y = 2 * (3 + 4);";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

    ASSERT_EQ(statements.size(), 1);
    auto varStmt = std::dynamic_pointer_cast<VarStmt>(statements[0]);
    ASSERT_NE(varStmt, nullptr);

    ASTPrinter printer;
    std::string result = printer.print(varStmt->initializer);
    EXPECT_EQ(result, "(* 2.000000 (group (+ 3.000000 4.000000)))");
}

TEST(ParserTest, ParsesLogicalExpression) {
    std::string source = "let z = 1 > 2 || 3 < 4;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

    ASSERT_EQ(statements.size(), 1);
    auto varStmt = std::dynamic_pointer_cast<VarStmt>(statements[0]);
    ASSERT_NE(varStmt, nullptr);

    ASTPrinter printer;
    std::string result = printer.print(varStmt->initializer);
    EXPECT_EQ(result, "(|| (> 1.000000 2.000000) (< 3.000000 4.000000))");
}

TEST(ParserTest, ParsesAssignment) {
    std::string source = "let a = 1; a = 2;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

    ASSERT_EQ(statements.size(), 2);

    auto exprStmt = std::dynamic_pointer_cast<ExpressionStmt>(statements[1]);
    ASSERT_NE(exprStmt, nullptr);

    ASTPrinter printer;
    std::string result = printer.print(exprStmt->expression);
    EXPECT_EQ(result, "(= a 2.000000)");
}

TEST(ParserTest, ParsesVariableWithType) {
    std::string source = "let x: int = 10;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

    ASSERT_EQ(statements.size(), 1);
    auto varStmt = std::dynamic_pointer_cast<VarStmt>(statements[0]);
    ASSERT_NE(varStmt, nullptr);
    EXPECT_TRUE(varStmt->type.has_value());
    EXPECT_EQ(varStmt->type->lexeme, "int");
}
