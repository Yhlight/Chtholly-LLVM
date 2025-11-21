#include "gtest/gtest.h"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/ASTPrinter.hpp"
#include "../src/Transpiler.hpp"
#include "test_helper.hpp"

TEST(ModuleTest, NamespacedImport) {
    std::string source = R"(
        import "math.cns";
        fn main() {
            math::add(1, 2);
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("tests/main.cns");
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(#include <iostream>
#include <string>
#include <vector>

namespace math {
int add(int a, int b) {
    return a + b;
}
}
int main(int argc, char* argv[]) {
math::add(1, 2);
}
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(ModuleTest, ModuleAlias) {
    std::string source = R"(
        import "math.cns" as m;
        fn main() {
            m::add(1, 2);
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("tests/main.cns");
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(#include <iostream>
#include <string>
#include <vector>

namespace m {
int add(int a, int b) {
    return a + b;
}
}
int main(int argc, char* argv[]) {
m::add(1, 2);
}
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(ModuleTest, Package) {
    std::string source = R"(
        package my_package;
        fn add(a: int, b: int): int {
            return a + b;
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("tests/main.cns");
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(
#include <iostream>
#include <string>
#include <vector>

namespace my_package {
int add(int a, int b) {
    return a + b;
}
}
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}
