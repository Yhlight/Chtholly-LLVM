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

TEST(SwitchTest, SwitchWithArbitraryExpression) {
    std::string source = R"(
        fn main() {
            let x = 2;
            let y = 1;
            switch (x) {
                case y + 1: {
                    return 1;
                }
                case y + 2: {
                    return 2;
                }
            }
            return 0;
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
            const auto y = 1;
            if (x == y + 1) {
                return 1;
            } else if (x == y + 2) {
                return 2;
            }
            return 0;
        }
    )";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(SwitchTest, SwitchWithExpression) {
    std::string source = R"(
        fn main() {
            let x = 2;
            let y = 2;
            switch (x) {
                case 1: {
                    return 1;
                }
                case y: {
                    return 2;
                }
            }
            return 0;
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
            const auto y = 2;
            if (x == 1) {
                return 1;
            } else if (x == y) {
                return 2;
            }
            return 0;
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
