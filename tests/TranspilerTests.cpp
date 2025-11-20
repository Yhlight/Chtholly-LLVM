#include <gtest/gtest.h>
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/Transpiler.hpp"
#include <vector>
#include <string>

using namespace chtholly;

TEST(TranspilerTest, TestSimpleExpression) {
    std::string source = "1 + 2 * 3;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    Parser parser(tokens);
    auto stmts = parser.parse();

    Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    EXPECT_EQ(result, "1 + 2 * 3;\n");
}

TEST(TranspilerTest, TestParenthesizedExpression) {
    std::string source = "(1 + 2) * 3;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    Parser parser(tokens);
    auto stmts = parser.parse();

    Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    EXPECT_EQ(result, "(1 + 2) * 3;\n");
}

TEST(TranspilerTest, TestLetDeclaration) {
    std::string source = "let a = 10;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    Parser parser(tokens);
    auto stmts = parser.parse();

    Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    EXPECT_EQ(result, "const auto a = 10;\n");
}

TEST(TranspilerTest, TestMutDeclaration) {
    std::string source = "mut b = 20;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    Parser parser(tokens);
    auto stmts = parser.parse();

    Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    EXPECT_EQ(result, "auto b = 20;\n");
}

TEST(TranspilerTest, TestTypedLetDeclaration) {
    std::string source = "let a: int = 10;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    Parser parser(tokens);
    auto stmts = parser.parse();

    Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    EXPECT_EQ(result, "const int a = 10;\n");
}
