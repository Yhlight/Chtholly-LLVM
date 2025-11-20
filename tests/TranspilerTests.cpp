#include <gtest/gtest.h>
#include "Transpiler.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"

TEST(TranspilerTest, TranspilesSimpleExpression) {
    std::string source = "let x = 1 + 2;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    Transpiler transpiler;
    std::string result = transpiler.transpile(statements);

    std::string expected =
        "#include <iostream>\n"
        "#include <any>\n"
        "#include <vector>\n\n"
        "int main() {\n"
        "    const auto x = 1.000000 + 2.000000;\n"
        "    return 0;\n"
        "}\n";

    EXPECT_EQ(result, expected);
}

TEST(TranspilerTest, TranspilesCompoundAssignment) {
    std::string source = "mut z = 1; z += 2;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    Transpiler transpiler;
    std::string result = transpiler.transpile(statements);

    std::string expected =
        "#include <iostream>\n"
        "#include <any>\n"
        "#include <vector>\n\n"
        "int main() {\n"
        "    auto z = 1.000000;\n"
        "    z += 2.000000;\n"
        "    return 0;\n"
        "}\n";

    EXPECT_EQ(result, expected);
}

TEST(TranspilerTest, TranspilesVariableWithType) {
    std::string source = "let x: int = 10;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    Transpiler transpiler;
    std::string result = transpiler.transpile(statements);

    std::string expected =
        "#include <iostream>\n"
        "#include <any>\n"
        "#include <vector>\n\n"
        "int main() {\n"
        "    const int x = 10.000000;\n"
        "    return 0;\n"
        "}\n";

    EXPECT_EQ(result, expected);
}

TEST(TranspilerTest, TranspilesMutableVariable) {
    std::string source = "mut y = \"hello\";";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    Transpiler transpiler;
    std::string result = transpiler.transpile(statements);

    std::string expected =
        "#include <iostream>\n"
        "#include <any>\n"
        "#include <vector>\n\n"
        "int main() {\n"
        "    auto y = \"hello\";\n"
        "    return 0;\n"
        "}\n";

    EXPECT_EQ(result, expected);
}

TEST(TranspilerTest, TranspilesAssignment) {
    std::string source = "mut z = 1; z = 2;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    Transpiler transpiler;
    std::string result = transpiler.transpile(statements);

    std::string expected =
        "#include <iostream>\n"
        "#include <any>\n"
        "#include <vector>\n\n"
        "int main() {\n"
        "    auto z = 1.000000;\n"
        "    z = 2.000000;\n"
        "    return 0;\n"
        "}\n";

    EXPECT_EQ(result, expected);
}
