#include <gtest/gtest.h>
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/Transpiler.hpp"

TEST(FunctionTests, TranspileSimpleFunction) {
    std::string source = R"(
        fn add(a: int, b: int): int {
            return a + b;
        }
    )";
    chtholly::Lexer lexer(source);
    auto tokens = lexer.scan_tokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();
    chtholly::Transpiler transpiler;
    std::string output = transpiler.transpile(stmts);
    std::string expected = "int add(int a, int b) {\nreturn a + b;\n}\n";
    ASSERT_EQ(output, expected);
}

TEST(FunctionTests, TranspileFunctionCall) {
    std::string source = R"(
        fn main(): int {
            add(1, 2);
            return 0;
        }
    )";
    chtholly::Lexer lexer(source);
    auto tokens = lexer.scan_tokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();
    chtholly::Transpiler transpiler;
    std::string output = transpiler.transpile(stmts);
    std::string expected = "int main() {\nadd(1, 2);\nreturn 0;\n}\n";
    ASSERT_EQ(output, expected);
}
