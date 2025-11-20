#include <gtest/gtest.h>
#include "../src/Parser.hpp"
#include "../src/Lexer.hpp"
#include "../src/Transpiler.hpp"

using namespace chtholly;

std::string normalize(const std::string& s) {
    std::string result;
    for (char c : s) {
        if (c != ' ' && c != '\n' && c != '\t' && c != '\r') {
            result += c;
        }
    }
    return result;
}

TEST(TranspilerTest, SimpleMain) {
    std::string source = "fn main() { return 0; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <iostream>
        #include <string>
        #include <vector>

        int main(int argc, char* argv[]) {
            return 0;
        }
    )";
    EXPECT_EQ(normalize(result), normalize(expected));
}

TEST(TranspilerTest, IfStatement) {
    std::string source = "fn main() { if (true) { return 1; } else { return 0; } }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <iostream>
        #include <string>
        #include <vector>

        int main(int argc, char* argv[]) {
            if (true) {
                return 1;
            } else {
                return 0;
            }
        }
    )";
    EXPECT_EQ(normalize(result), normalize(expected));
}

TEST(TranspilerTest, VariableDeclaration) {
    std::string source = "fn main() { let x = 10; return x; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <iostream>
        #include <string>
        #include <vector>

        int main(int argc, char* argv[]) {
            auto x = 10;
            return x;
        }
    )";
    EXPECT_EQ(normalize(result), normalize(expected));
}

TEST(TranspilerTest, FunctionCall) {
    std::string source = R"(
        fn add(a, b) {
            return a + b;
        }

        fn main() {
            let result = add(5, 10);
            return result;
        }
    )";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <iostream>
        #include <string>
        #include <vector>

        auto add(auto a, auto b) {
            return a + b;
        }

        int main(int argc, char* argv[]) {
            auto result = add(5, 10);
            return result;
        }
    )";
    EXPECT_EQ(normalize(result), normalize(expected));
}
