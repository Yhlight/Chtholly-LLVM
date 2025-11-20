#include <gtest/gtest.h>
#include "../src/Transpiler.hpp"
#include "../src/Parser.hpp"
#include "../src/Lexer.hpp"

using namespace chtholly;

TEST(ArrayTest, Literal) {
    std::string source = "let x = [1, 2, 3];";
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
                           "    auto const x = std::vector<auto>({1, 2, 3});\n"
                           "    return 0;\n"
                           "}\n";
    EXPECT_EQ(result, expected);
}

TEST(ArrayTest, Subscript) {
    std::string source = "x[0];";
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
                           "    x[0];\n"
                           "    return 0;\n"
                           "}\n";
    EXPECT_EQ(result, expected);
}

TEST(ArrayTest, Assignment) {
    std::string source = "x[0] = 1;";
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
                           "    x[0] = 1;\n"
                           "    return 0;\n"
                           "}\n";
    EXPECT_EQ(result, expected);
}
