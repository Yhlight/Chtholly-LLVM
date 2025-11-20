#include <gtest/gtest.h>
#include "../src/Transpiler.hpp"
#include "../src/Parser.hpp"
#include "../src/Lexer.hpp"

using namespace chtholly;

TEST(FunctionTest, Declaration) {
    std::string source = "fn add(a, b) { return a + b; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    Transpiler transpiler;
    std::string result = transpiler.transpile(statements);
    std::string expected = "#include <iostream>\n"
                           "#include <string>\n"
                           "#include <vector>\n\n"
                           "auto add(auto a, auto b) {\n"
                           "    return (a + b);\n"
                           "}\n\n"
                           "int main() {\n"
                           "    return 0;\n"
                           "}\n";
    EXPECT_EQ(result, expected);
}

TEST(FunctionTest, Call) {
    std::string source = "add(1, 2);";
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
                           "    add(1, 2);\n"
                           "    return 0;\n"
                           "}\n";
    EXPECT_EQ(result, expected);
}

TEST(FunctionTest, ReturnStatement) {
    std::string source = "fn main() { return 0; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    Transpiler transpiler;
    std::string result = transpiler.transpile(statements);
    std::string expected = "#include <iostream>\n"
                           "#include <string>\n"
                           "#include <vector>\n\n"
                           "auto main() {\n"
                           "    return 0;\n"
                           "}\n\n"
                           "int main() {\n"
                           "    return 0;\n"
                           "}\n";
    EXPECT_EQ(result, expected);
}
