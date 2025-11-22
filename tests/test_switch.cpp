#include "gtest/gtest.h"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/ASTPrinter.hpp"
#include "../src/Transpiler.hpp"
#include "test_helper.hpp"

TEST(SwitchTest, BasicSwitch) {
    std::string source = R"(
        fn main() {
            let x = 2;
            switch(x) {
                case 1: {
                    return 1;
                }
                case 2: {
                    return 2;
                }
                case 3: {
                    return 3;
                }
            }
        }
    )";

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
            const auto x = 2;
            switch (x) {
                case 1:
                {
                    return 1;
                }
                case 2:
                {
                    return 2;
                }
                case 3:
                {
                    return 3;
                }
            }
        }
    )";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(SwitchTest, SwitchWithFallthrough) {
    std::string source = R"(
        fn main() {
            let x = 1;
            mut y = 0;
            switch(x) {
                case 1: {
                    y = 1;
                    fallthrough;
                }
                case 2: {
                    y = 2;
                }
            }
            return y;
        }
    )";

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
            const auto x = 1;
            auto y = 0;
            switch (x) {
                case 1:
                {
                    y = 1;
                    [[fallthrough]];
                }
                case 2:
                {
                    y = 2;
                }
            }
            return y;
        }
    )";
    ASSERT_EQ(normalize(result), normalize(expected));
}
