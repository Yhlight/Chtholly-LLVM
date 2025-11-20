#include <gtest/gtest.h>
#include "../src/Transpiler.hpp"
#include "../src/Parser.hpp"
#include "../src/Lexer.hpp"

using namespace chtholly;

TEST(OperatorTest, Modulo) {
    std::string source = "10 % 3;";
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
                           "    (10 % 3);\n"
                           "    return 0;\n"
                           "}\n";
    EXPECT_EQ(result, expected);
}

TEST(OperatorTest, LogicalAnd) {
    std::string source = "1 && 0;";
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
                           "    (1 && 0);\n"
                           "    return 0;\n"
                           "}\n";
    EXPECT_EQ(result, expected);
}

TEST(OperatorTest, LogicalOr) {
    std::string source = "1 || 0;";
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
                           "    (1 || 0);\n"
                           "    return 0;\n"
                           "}\n";
    EXPECT_EQ(result, expected);
}

TEST(OperatorTest, Precedence) {
    std::string source = "1 || 0 && 1;";
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
                           "    (1 || (0 && 1));\n"
                           "    return 0;\n"
                           "}\n";
    EXPECT_EQ(result, expected);
}
