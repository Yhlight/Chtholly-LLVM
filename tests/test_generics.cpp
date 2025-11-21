#include "gtest/gtest.h"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/ASTPrinter.hpp"
#include "../src/Transpiler.hpp"
#include "test_helper.hpp"

TEST(GenericsTest, BasicGenericFunction) {
    std::string source = R"(
        fn add<T>(a: T, b: T): T {
            return a + b;
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(#include <iostream>
#include <string>
#include <vector>

template <typename T>
T add(T a, T b) {
return a + b;
}
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(GenericsTest, GenericFunctionCall) {
    std::string source = R"(
        fn add<T>(a: T, b: T): T {
            return a + b;
        }
        fn main() {
            add<int>(1, 2);
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(#include <iostream>
#include <string>
#include <vector>

template <typename T>
T add(T a, T b) {
return a + b;
}

int main(int argc, char* argv[]) {
add<int>(1, 2);
}
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}
