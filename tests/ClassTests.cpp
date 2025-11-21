#include <gtest/gtest.h>
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/Transpiler.hpp"

const std::string includes = "#include <iostream>\n"
                             "#include <string>\n"
                             "#include <vector>\n"
                             "#include <memory>\n\n";

TEST(ClassTests, TranspileSimpleClass) {
    std::string source = R"(
        class Test {
            let a: int = 1;
            fn add(a: int, b: int): int {
                return a + b;
            }
        }
    )";
    chtholly::Lexer lexer(source);
    auto tokens = lexer.scan_tokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();
    chtholly::Transpiler transpiler;
    std::string output = transpiler.transpile(stmts);
    std::string expected = includes + "class Test {\npublic:\nconst int a = 1;\nint add(int a, int b) {\nreturn a + b;\n}\n};\n";
    ASSERT_EQ(output, expected);
}

TEST(ClassTests, TranspileClassInstantiation) {
    std::string source = R"(
        let test = new Test();
    )";
    chtholly::Lexer lexer(source);
    auto tokens = lexer.scan_tokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();
    chtholly::Transpiler transpiler;
    std::string output = transpiler.transpile(stmts);
    std::string expected = includes + "const std::unique_ptr<Test> test = std::make_unique<Test>();\n";
    ASSERT_EQ(output, expected);
}
