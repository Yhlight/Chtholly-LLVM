#include <gtest/gtest.h>
#include "../src/Parser.hpp"
#include "../src/Lexer.hpp"
#include "../src/ASTPrinter.hpp"

TEST(ParserTest, ParsesVariableDeclaration) {
    std::string source = "let x = 10;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    ASTPrinter printer;

    ASSERT_EQ(statements.size(), 1);
    std::string result = printer.print(statements[0]);
    EXPECT_EQ(result, "(var x 10)");
}

TEST(ParserTest, ParsesAssignmentExpression) {
    std::string source = "x = 10;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    ASTPrinter printer;

    ASSERT_EQ(statements.size(), 1);
    std::string result = printer.print(statements[0]);
    EXPECT_EQ(result, "(; (= x 10))");
}

TEST(ParserTest, ParsesExpressionStatement) {
    std::string source = "1 + 2 * 3;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    ASTPrinter printer;

    ASSERT_EQ(statements.size(), 1);
    std::string result = printer.print(statements[0]);
    EXPECT_EQ(result, "(; (+ 1 (* 2 3)))");
}

TEST(ParserTest, HandlesParserErrors) {
    std::string source = "let x = ;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

    ASSERT_EQ(statements.size(), 0);
    EXPECT_TRUE(parser.had_error);
}
