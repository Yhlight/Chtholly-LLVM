#include "gtest/gtest.h"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/ASTPrinter.hpp"
#include "../src/Transpiler.hpp"
#include "test_helper.hpp"

TEST(StdlibTest, Iostream) {
    std::string source = R"(
        import iostream;
        fn main() {
            iostream::print("Hello, World!");
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
#include <iostream>

namespace iostream {
    template<typename T>
    void print(const T& msg) {
        std::cout << msg;
    }

    template<typename T>
    void println(const T& msg) {
        std::cout << msg << std::endl;
    }
}

int main(int argc, char* argv[]) {
    iostream::print("Hello, World!");
}
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(StdlibTest, Math) {
    std::string source = R"(
        import math;
        import iostream;
        fn main() {
            iostream::println(math::sqrt(16));
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
#include <iostream>
#include <cmath>

namespace math {
    template<typename T>
    T sqrt(T val) { return std::sqrt(val); }

    template<typename T>
    T sin(T val) { return std::sin(val); }

    template<typename T>
    T cos(T val) { return std::cos(val); }

    template<typename T>
    T tan(T val) { return std::tan(val); }

    template<typename T, typename U>
    auto pow(T base, U exp) -> decltype(std::pow(base, exp)) { return std::pow(base, exp); }
}

namespace iostream {
    template<typename T>
    void print(const T& msg) {
        std::cout << msg;
    }

    template<typename T>
    void println(const T& msg) {
        std::cout << msg << std::endl;
    }
}

int main(int argc, char* argv[]) {
    iostream::println(math::sqrt(16));
}
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}

TEST(StdlibTest, String) {
    std::string source = R"(
        import string;
        import iostream;
        fn main() {
            iostream::println(string::length("hello"));
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
#include <iostream>
#include <string>

namespace string {
    size_t length(const std::string& s) { return s.length(); }
    std::string substr(const std::string& s, size_t pos, size_t len = std::string::npos) { return s.substr(pos, len); }
    size_t find(const std::string& s, const std::string& to_find, size_t pos = 0) { return s.find(to_find, pos); }
}

namespace iostream {
    template<typename T>
    void print(const T& msg) {
        std::cout << msg;
    }

    template<typename T>
    void println(const T& msg) {
        std::cout << msg << std::endl;
    }
}

int main(int argc, char* argv[]) {
    iostream::println(string::length("hello"));
}
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}
