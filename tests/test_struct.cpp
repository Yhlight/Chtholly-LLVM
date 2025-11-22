#include "gtest/gtest.h"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/ASTPrinter.hpp"
#include "../src/Transpiler.hpp"
#include "test_helper.hpp"

TEST(StructTest, BasicStruct) {
    std::string source = R"(
        struct Point {
            let x: int = 0;
            let y: int = 0;
        }

        fn main() {
            let p = Point();
            return p.x;
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(
        #include <string>
        #include <vector>

        struct Point {
            const int x = 0;
            const int y = 0;
        };

        int main(int argc, char* argv[]) {
            const auto p = Point();
            return p.x;
        }
    )";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(StructTest, StructWithMutableMembers) {
    std::string source = R"(
        struct Point {
            mut x: int = 0;
            mut y: int = 0;
        }

        fn main() {
            mut p = Point();
            p.x = 10;
            return p.x;
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(
        #include <string>
        #include <vector>

        struct Point {
            int x = 0;
            int y = 0;
        };

        int main(int argc, char* argv[]) {
            auto p = Point();
            p.x = 10;
            return p.x;
        }
    )";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(StructTest, StructWithStaticMembers) {
    std::string source = R"(
        struct Math {
            static let PI: double = 3.14;
        }

        fn main() {
            return Math::PI;
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(
        #include <string>
        #include <vector>

        struct Math {
            inline static const double PI = 3.14;
        };

        int main(int argc, char* argv[]) {
            return Math::PI;
        }
    )";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(StructTest, StructWithStaticMethod) {
    std::string source = R"(
        struct Math {
            static fn add(a: int, b: int): int {
                return a + b;
            }
        }

        fn main() {
            return Math::add(1, 2);
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(
        #include <string>
        #include <vector>

        struct Math {
            static int add(int a, int b) {
                return a + b;
            }
        };

        int main(int argc, char* argv[]) {
            return Math::add(1, 2);
        }
    )";
    ASSERT_EQ(normalize(result), normalize(expected));
}
