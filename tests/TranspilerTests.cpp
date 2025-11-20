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

TEST(TranspilerTest, TestIfStatement) {
    std::string source = "if (true) { 1; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    Parser parser(tokens);
    auto stmts = parser.parse();

    Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    EXPECT_EQ(result, "if (true) {\n1;\n}\n");
}

TEST(TranspilerTest, TestIfElseStatement) {
    std::string source = "if (false) 1; else 2;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    Parser parser(tokens);
    auto stmts = parser.parse();

    Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    EXPECT_EQ(result, "if (false) 1;\nelse 2;\n");
}

TEST(TranspilerTest, TestWhileStatement) {
    std::string source = "while (true) { 1; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    Parser parser(tokens);
    auto stmts = parser.parse();

    Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    EXPECT_EQ(result, "while (true) {\n1;\n}\n");
}

TEST(TranspilerTest, TestForStatement) {
    std::string source = "for (let i = 0; i < 10; i = i + 1) { 1; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    Parser parser(tokens);
    auto stmts = parser.parse();

    Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    EXPECT_EQ(result, "for (const auto i = 0; i < 10; i = i + 1) {\n1;\n}\n");
}

TEST(TranspilerTest, TestFunctionDeclaration) {
    std::string source = "fn add(a: int, b: int): int { 1; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    Parser parser(tokens);
    auto stmts = parser.parse();

    Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    EXPECT_EQ(result, "int add(int a, int b) {\n1;\n}\n");
}

TEST(TranspilerTest, TestFunctionCall) {
    std::string source = "add(1, 2);";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    Parser parser(tokens);
    auto stmts = parser.parse();

    Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    EXPECT_EQ(result, "add(1, 2);\n");
}
