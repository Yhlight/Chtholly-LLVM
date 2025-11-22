#include "gtest/gtest.h"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/ASTPrinter.hpp"
#include "../src/Transpiler.hpp"
#include "test_helper.hpp"

TEST(MainArgsTest, MainArgs) {
    std::string source = R"(
        fn main(args: string[]) {
            return args.size();
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
            std::vector<std::string> args(argv, argv + argc);
            return args.size();
        }
    )";
    ASSERT_EQ(normalize(result), normalize(expected));
}
