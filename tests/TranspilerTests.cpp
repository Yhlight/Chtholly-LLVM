#include <gtest/gtest.h>
#include "../src/Transpiler.hpp"
#include "../src/Parser.hpp"
#include "../src/Lexer.hpp"

TEST(TranspilerTest, LetStatement) {
    std::string source = "let a = 10;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::unique_ptr<Stmt>> statements = parser.parse();
    Transpiler transpiler;
    std::string result = transpiler.transpile(statements);
    EXPECT_EQ(result, "const auto a = 10.000000;\n");
}
