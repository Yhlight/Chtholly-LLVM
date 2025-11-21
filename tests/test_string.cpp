#include "gtest/gtest.h"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/ASTPrinter.hpp"
#include "../src/Transpiler.hpp"
#include "test_helper.hpp"

TEST(StringTest, Concat) {
    std::string source = R"(
        import string;
        import iostream;
        fn main() {
            let s1 = "Hello, ";
            let s2 = "World!";
            iostream::print(string::concat(s1, s2));
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("tests/main.cns");
    std::string result = transpiler.transpile(stmts);

    // To verify the result, we can compile and run the generated C++ code.
    // For simplicity in this test, we'll just check for the presence of the correct function call.
    ASSERT_NE(result.find("string::concat(s1, s2)"), std::string::npos);
}

TEST(StringTest, Len) {
    std::string source = R"(
        import string;
        import iostream;
        fn main() {
            let s = "Hello";
            iostream::print(string::len(s));
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("tests/main.cns");
    std::string result = transpiler.transpile(stmts);
    ASSERT_NE(result.find("string::len(s)"), std::string::npos);
}

TEST(StringTest, Substr) {
    std::string source = R"(
        import string;
        import iostream;
        fn main() {
            let s = "Hello, World!";
            iostream::print(string::substr(s, 7, 5));
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("tests/main.cns");
    std::string result = transpiler.transpile(stmts);
    ASSERT_NE(result.find("string::substr(s, 7, 5)"), std::string::npos);
}
