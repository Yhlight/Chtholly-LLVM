#include <gtest/gtest.h>
#include "../src/Transpiler.hpp"
#include "../src/Parser.hpp"
#include "../src/Lexer.hpp"

using namespace chtholly;

TEST(VariableTest, LetDeclaration) {
    std::string source = "let x = 10;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    Transpiler transpiler;
    std::string result = transpiler.transpile(statements);
    std::string expected = "#include <iostream>\n"
                           "#include <string>\n"
                           "#include <vector>\n\n"
                           "int main() {\n"
                           "    auto const x = 10;\n"
                           "    return 0;\n"
                           "}\n";
    EXPECT_EQ(result, expected);
}

TEST(VariableTest, MutDeclaration) {
    std::string source = "mut y = 20;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    Transpiler transpiler;
    std::string result = transpiler.transpile(statements);
    std::string expected = "#include <iostream>\n"
                           "#include <string>\n"
                           "#include <vector>\n\n"
                           "int main() {\n"
                           "    auto y = 20;\n"
                           "    return 0;\n"
                           "}\n";
    EXPECT_EQ(result, expected);
}
