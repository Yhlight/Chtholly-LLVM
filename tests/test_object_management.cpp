#include "gtest/gtest.h"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/ASTPrinter.hpp"
#include "../src/Transpiler.hpp"
#include "test_helper.hpp"

TEST(ObjectManagementTest, Copy) {
    std::string source = R"(
        class MyObject {
            fn clone() {}
        }

        fn main() {
            let a = MyObject();
            let b = *a;
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

        class MyObject {
            public:
            auto clone() {}
        };

        int main(int argc, char* argv[]) {
            const auto a = MyObject();
            const auto b = a.clone();
        }
    )";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(ObjectManagementTest, Move) {
    std::string source = R"(
        class MyObject {
            fn move() {}
        }

        fn main() {
            let a = MyObject();
            let b = &&a;
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

        class MyObject {
            public:
            auto move() {}
        };

        int main(int argc, char* argv[]) {
            const auto a = MyObject();
            const auto b = a.move();
        }
    )";
    ASSERT_EQ(normalize(result), normalize(expected));
}
