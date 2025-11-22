#include "gtest/gtest.h"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/ASTPrinter.hpp"
#include "../src/Transpiler.hpp"
#include "test_helper.hpp"

TEST(OperatorTest, LogicalOperators) {
    std::string source = "fn main() { return true && false || true; }";
    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();
    chtholly::Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <string>
        #include <vector>
        int main(int argc, char* argv[]) {
            return true && false || true;
        }
    )";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(OperatorTest, BitwiseOperators) {
    std::string source = "fn main() { return (1 | 2) & 3 ^ 4 << 1 >> 1; }";
    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();
    chtholly::Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <string>
        #include <vector>
        int main(int argc, char* argv[]) {
            return (1 | 2) & 3 ^ 4 << 1 >> 1;
        }
    )";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(OperatorTest, BitwiseNot) {
    std::string source = "fn main() { return ~10; }";
    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();
    chtholly::Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <string>
        #include <vector>
        int main(int argc, char* argv[]) {
            return ~10;
        }
    )";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(OperatorTest, Precedence) {
    std::string source = "fn main() { return 1 + 2 * 3 == 7 && 4 | 1 == 5; }";
    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();
    chtholly::Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <string>
        #include <vector>
        int main(int argc, char* argv[]) {
            return 1 + 2 * 3 == 7 && 4 | 1 == 5;
        }
    )";
    ASSERT_EQ(normalize(result), normalize(expected));
}
