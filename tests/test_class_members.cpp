#include "gtest/gtest.h"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/ASTPrinter.hpp"
#include "../src/Transpiler.hpp"
#include "test_helper.hpp"
#include <fstream>
#include <streambuf>

TEST(ClassMembersTest, LetAndMut) {
    std::ifstream t("tests/class_members.cns");
    std::string source((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("tests/class_members.cns");
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(
#include <string>
#include <vector>

class Test {
public:
const int immutable_member = 10;
int mutable_member = 20;
};
int main(int argc, char* argv[]) {
auto t = Test();
t.mutable_member = 30;
return t.immutable_member;
}
)";
    ASSERT_EQ(normalize(result), normalize(expected));
}
