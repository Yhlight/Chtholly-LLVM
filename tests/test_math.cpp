#include "gtest/gtest.h"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/ASTPrinter.hpp"
#include "../src/Transpiler.hpp"
#include "test_helper.hpp"

TEST(MathTest, Sqrt) {
    std::string source = R"(
        import math;
        import iostream;
        fn main() {
            let x = 16.0;
            iostream::print(math::sqrt(x));
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("tests/main.cns");
    std::string result = transpiler.transpile(stmts);
    ASSERT_NE(result.find("math::sqrt(x)"), std::string::npos);
}

TEST(MathTest, Pow) {
    std::string source = R"(
        import math;
        import iostream;
        fn main() {
            let x = 2.0;
            let y = 3.0;
            iostream::print(math::pow(x, y));
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("tests/main.cns");
    std::string result = transpiler.transpile(stmts);
    ASSERT_NE(result.find("math::pow(x, y)"), std::string::npos);
}

TEST(MathTest, Sin) {
    std::string source = R"(
        import math;
        import iostream;
        fn main() {
            let x = 0.0;
            iostream::print(math::sin(x));
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("tests/main.cns");
    std::string result = transpiler.transpile(stmts);
    ASSERT_NE(result.find("math::sin(x)"), std::string::npos);
}
