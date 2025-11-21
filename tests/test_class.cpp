#include "gtest/gtest.h"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/ASTPrinter.hpp"
#include "../src/Transpiler.hpp"
#include "test_helper.hpp"

TEST(ClassTest, BasicClass) {
    std::string source = R"(
        class Test {
            public:
            let a: int = 1;
            private:
            let b: int = 2;
            fn hello() {
                // Do nothing
            }
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(#include <iostream>
#include <string>
#include <vector>

class Test {
public:
int a = 1;
private:
int b = 2;
auto hello() {
}
};
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(ClassTest, PrivateMember) {
    std::string source = R"(
        class Test {
            private:
            let a: int = 1;
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(#include <iostream>
#include <string>
#include <vector>

class Test {
private:
int a = 1;
};
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(ClassTest, Constructor) {
    std::string source = R"(
        class Test {
            public:
            let a: int;
            Test(val: int) {
                this.a = val;
            }
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(#include <iostream>
#include <string>
#include <vector>

class Test {
public:
int a;
Test(int val) {
this->a = val;
}
};
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(ClassTest, StaticMember) {
    std::string source = R"(
        class Test {
            public:
            static let a: int = 1;
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(#include <iostream>
#include <string>
#include <vector>

class Test {
public:
static int a = 1;
};
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(ClassTest, Destructor) {
    std::string source = R"(
        class Test {
            public:
            ~Test() {
                // Do nothing
            }
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(#include <iostream>
#include <string>
#include <vector>

class Test {
public:
~Test() {
}
};
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(ClassTest, ClassInstantiation) {
    std::string source = R"(
        class Test {}
        fn main() {
            let t = Test();
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(#include <iostream>
#include <string>
#include <vector>

class Test {
};
int main(int argc, char* argv[]) {
auto t = Test();
}
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(ClassTest, MemberAccess) {
    std::string source = R"(
        class Test {
            public:
            let a: int = 1;
        }
        fn main() {
            let t = Test();
            t.a = 2;
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler;
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(#include <iostream>
#include <string>
#include <vector>

class Test {
public:
int a = 1;
};
int main(int argc, char* argv[]) {
auto t = Test();
t.a = 2;
}
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}
