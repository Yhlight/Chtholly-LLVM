#include "gtest/gtest.h"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/ASTPrinter.hpp"
#include "../src/Transpiler.hpp"
#include "test_helper.hpp"

TEST(TypeSystemTest, TypeCast) {
    std::string source = R"(
        fn main() {
            let a: int = type_cast<int>(10.5);
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("tests/main.cns");
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    int a = static_cast<int>(10.5);
}
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(TypeSystemTest, MutableReference) {
    std::string source = R"(
        fn test(a: &int) {}
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("tests/main.cns");
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(
#include <string>
#include <vector>

auto test(int& a) {}
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(TypeSystemTest, MoveReference) {
    std::string source = R"(
        fn test(a: &&int) {}
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("tests/main.cns");
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(
#include <string>
#include <vector>

auto test(int&& a) {}
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(TypeSystemTest, Copy) {
    std::string source = R"(
        fn test(a: *int) {}
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("tests/main.cns");
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(
#include <string>
#include <vector>

auto test(int a) {}
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(TypeSystemTest, DefaultImmutableReference) {
    std::string source = R"(
        fn test(a: string) {}
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("tests/main.cns");
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(
#include <string>
#include <vector>

auto test(const std::string& a) {}
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}
