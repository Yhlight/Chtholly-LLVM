#include <gtest/gtest.h>
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/Transpiler.hpp"
#include <string>
#include <vector>
#include <regex>
#include "test_helper.hpp"

TEST(ClassTest, SimpleClass) {
    std::string source = R"(
        class Point {
            public:
            let x: int = 0;
            let y: int = 0;
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();
    chtholly::Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(
        #include <iostream>
        #include <string>
        #include <vector>

        class Point {
        public:
            int x = 0;
            int y = 0;
        };
    )";

    EXPECT_EQ(normalize(result), normalize(expected));
}

TEST(ClassTest, ClassWithMethods) {
    std::string source = R"(
        class Greeter {
            public:
            fn sayHello() {
                // body
            }
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();
    chtholly::Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(
        #include <iostream>
        #include <string>
        #include <vector>

        class Greeter {
        public:
            auto sayHello() {
            }
        };
    )";

    EXPECT_EQ(normalize(result), normalize(expected));
}

TEST(ClassTest, ClassWithPrivateMembers) {
    std::string source = R"(
        class Account {
            private:
            let balance: double = 0.0;
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();
    chtholly::Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(
        #include <iostream>
        #include <string>
        #include <vector>

        class Account {
        private:
            double balance = 0;
        };
    )";

    EXPECT_EQ(normalize(result), normalize(expected));
}

TEST(ClassTest, MemberAccess) {
    std::string source = R"(
        class Point {
            public:
            let x: int = 0;
        }
        let p = Point();
        p.x = 10;
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();
    chtholly::Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(
        #include <iostream>
        #include <string>
        #include <vector>

        class Point {
        public:
            int x = 0;
        };
        auto p = Point();
        p.x = 10;
    )";

    EXPECT_EQ(normalize(result), normalize(expected));
}
