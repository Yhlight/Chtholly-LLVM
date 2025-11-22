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
    Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <string>
        #include <vector>

        int main(int argc, char* argv[]) {
            return 0;
        }
    )";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(TranspilerTest, ArrayAt) {
    std::string source = R"(
        fn main() {
            let a: int[] = [1, 2, 3];
            a.at(0);
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("tests/main.cns");
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(
        #include <string>
        #include <vector>

        int main(int argc, char* argv[]) {
            const std::vector<int> a = {1, 2, 3};
            a.at(0);
        }
    )";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(TranspilerTest, DoWhileStatement) {
    std::string source = R"(
        fn main() {
            mut i = 0;
            do {
                i = i + 1;
            } while (i < 10);
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("tests/main.cns");
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(
        #include <string>
        #include <vector>

        int main(int argc, char* argv[]) {
            auto i = 0;
            do {
                i = i + 1;
            } while (i < 10);
        }
    )";
    EXPECT_EQ(normalize(result), normalize(expected));
}

TEST(TranspilerTest, StaticArrayDeclaration) {
    std::string source = "fn main() { let a: int[4] = [1, 2, 3, 4]; return a[0]; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <string>
        #include <vector>
        #include <array>

        int main(int argc, char* argv[]) {
            const std::array<int, 4> a = {1, 2, 3, 4};
            return a[0];
        }
    )";
    EXPECT_EQ(normalize(result), normalize(expected));
}

TEST(TranspilerTest, SwitchStatementWithArbitraryExpression) {
    std::string source = R"(
        fn main() {
            let x = 2;
            let y = 1;
            switch (x) {
                case y + 1: {
                    return 1;
                }
                case y + 2: {
                    return 2;
                }
            }
            return 0;
        }
    )";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <string>
        #include <vector>

        int main(int argc, char* argv[]) {
            const auto x = 2;
            const auto y = 1;
            if (x == y + 1) {
                return 1;
            } else if (x == y + 2) {
                return 2;
            }
            return 0;
        }
    )";
    EXPECT_EQ(normalize(result), normalize(expected));
}

TEST(TranspilerTest, GenericFunctionWithDefaultType) {
    std::string source = "fn add<T = double>(a: T, b: T): T { return a + b; } fn main() { add(1.0, 2.0); }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <string>
        #include <vector>

        template <typename T = double>
        T add(const T& a, const T& b) {
            return a + b;
        }

        int main(int argc, char* argv[]) {
            add(1.0, 2.0);
        }
    )";
    EXPECT_EQ(normalize(result), normalize(expected));
}

TEST(TranspilerTest, SwitchStatementWithExpression) {
    std::string source = R"(
        fn main() {
            let x = 2;
            let y = 2;
            switch (x) {
                case 1: {
                    return 1;
                }
                case y: {
                    return 2;
                }
            }
            return 0;
        }
    )";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <string>
        #include <vector>

        int main(int argc, char* argv[]) {
            const auto x = 2;
            const auto y = 2;
            if (x == 1) {
                return 1;
            } else if (x == y) {
                return 2;
            }
            return 0;
        }
    )";
    EXPECT_EQ(normalize(result), normalize(expected));
}

TEST(TranspilerTest, LambdaExpression) {
    std::string source = "fn main() { let add = [](a: int, b: int): int { return a + b; }; return add(1, 2); }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <string>
        #include <vector>
        #include <functional>

        int main(int argc, char* argv[]) {
            const auto add = [](int a, int b) -> int {
                return a + b;
            };
            return add(1, 2);
        }
    )";
    EXPECT_EQ(normalize(result), normalize(expected));
}

TEST(TranspilerTest, FunctionType) {
    std::string source = R"(
        fn add(a: int, b: int): int {
            return a + b;
        }

        fn main() {
            let my_func: (int, int): int = add;
            return my_func(5, 10);
        }
    )";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <string>
        #include <vector>
        #include <functional>

        int add(int a, int b) {
            return a + b;
        }

        int main(int argc, char* argv[]) {
            const std::function<int(int, int)> my_func = add;
            return my_func(5, 10);
        }
    )";
    EXPECT_EQ(normalize(result), normalize(expected));
}

TEST(TranspilerTest, EnumDeclaration) {
    std::string source = R"(
        enum Color {
            RED,
            GREEN,
            BLUE
        }

        fn main() {
            let c: Color = Color::RED;
            return 0;
        }
    )";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <string>
        #include <vector>

        enum class Color {
            RED,
            GREEN,
            BLUE
        };

        int main(int argc, char* argv[]) {
            const Color c = Color::RED;
            return 0;
        }
    )";
    EXPECT_EQ(normalize(result), normalize(expected));
}

TEST(TranspilerTest, ArrayDeclaration) {
    std::string source = "fn main() { mut a: int[] = [1, 2, 3]; a[0] = 10; return a[0]; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <string>
        #include <vector>

        int main(int argc, char* argv[]) {
            std::vector<int> a = {1, 2, 3};
            a[0] = 10;
            return a[0];
        }
    )";
    EXPECT_EQ(normalize(result), normalize(expected));
}

TEST(TranspilerTest, SwitchStatement) {
    std::string source = R"(
        fn main() {
            let x = 2;
            switch (x) {
                case 1: {
                    return 1;
                }
                case 2: {
                    return 2;
                    break;
                }
                case 3: {
                    fallthrough;
                }
                case 4: {
                    return 4;
                }
            }
            return 0;
        }
    )";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <string>
        #include <vector>

        int main(int argc, char* argv[]) {
            const auto x = 2;
            if (x == 1) {
                return 1;
            } else if (x == 2) {
                return 2;
                break;
            } else if (x == 3) {
                [[fallthrough]];
            } else if (x == 4) {
                return 4;
            }
            return 0;
        }
    )";
    EXPECT_EQ(normalize(result), normalize(expected));
}

TEST(TranspilerTest, ForStatement) {
    std::string source = "fn main() { for (let i = 0; i < 10; i = i + 1) {} return 0; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <string>
        #include <vector>

        int main(int argc, char* argv[]) {
            for (auto i = 0; i < 10; i = i + 1) {
            }
            return 0;
        }
    )";
    EXPECT_EQ(normalize(result), normalize(expected));
}

TEST(TranspilerTest, WhileStatement) {
    std::string source = "fn main() { mut i = 0; while (i < 10) { i = i + 1; } return i; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <string>
        #include <vector>

        int main(int argc, char* argv[]) {
            auto i = 0;
            while (i < 10) {
                i = i + 1;
            }
            return i;
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
    Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
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
    Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <string>
        #include <vector>

        int main(int argc, char* argv[]) {
            const auto x = 10;
            return x;
        }
    )";
    EXPECT_EQ(normalize(result), normalize(expected));
}

TEST(TranspilerTest, FunctionCall) {
    std::string source = R"(
        fn add(a: int, b: int): int {
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
    Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);
    std::string expected = R"(
        #include <string>
        #include <vector>

        int add(int a, int b) {
            return a + b;
        }

        int main(int argc, char* argv[]) {
            const auto result = add(5, 10);
            return result;
        }
    )";
    EXPECT_EQ(normalize(result), normalize(expected));
}
