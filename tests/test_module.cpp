#include "gtest/gtest.h"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/ASTPrinter.hpp"
#include "../src/Transpiler.hpp"
#include "test_helper.hpp"

TEST(ModuleTest, BasicImport) {
    std::string source = R"(
        import "math.cns";
        fn main() {
            add(1, 2);
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

int add(int a, int b) {
return a + b;
}
int main(int argc, char* argv[]) {
add(1, 2);
}
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}
