#include <gtest/gtest.h>
#include "../src/Transpiler.hpp"
#include "../src/Parser.hpp"
#include "../src/Lexer.hpp"

using namespace chtholly;

TEST(ControlFlowTest, IfStatement) {
    std::string source = "if (1) 1;";
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
                           "    if (1) 1;\n"
                           "    return 0;\n"
                           "}\n";
    EXPECT_EQ(result, expected);
}

TEST(ControlFlowTest, IfElseStatement) {
    std::string source = "if (1) 1; else 2;";
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
                           "    if (1) 1; else 2;\n"
                           "    return 0;\n"
                           "}\n";
    EXPECT_EQ(result, expected);
}

TEST(ControlFlowTest, WhileStatement) {
    std::string source = "while (1) 1;";
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
                           "    while (1) 1;\n"
                           "    return 0;\n"
                           "}\n";
    EXPECT_EQ(result, expected);
}

TEST(ControlFlowTest, ForStatement) {
    std::string source = "for (let i = 0; i < 10; i = i + 1) 1;";
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
                           "    for (auto const i = 0; (i < 10); i = (i + 1)) 1;\n"
                           "    return 0;\n"
                           "}\n";
    EXPECT_EQ(result, expected);
}
