#include "gtest/gtest.h"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/ASTPrinter.hpp"
#include "../src/Transpiler.hpp"
#include "test_helper.hpp"

TEST(StdlibTest, Iostream) {
    std::string source = R"(
        import iostream;
        fn main() {
            iostream::print("Hello, World!");
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
#include <iostream>

namespace iostream {
    template<typename T>
    void print(const T& msg) {
        std::cout << msg;
    }

    template<typename T>
    void println(const T& msg) {
        std::cout << msg << std::endl;
    }
}

int main(int argc, char* argv[]) {
    iostream::print("Hello, World!");
}
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}
