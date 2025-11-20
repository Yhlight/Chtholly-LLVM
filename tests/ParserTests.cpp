#include <gtest/gtest.h>
#include "../src/Parser.hpp"
#include "../src/Lexer.hpp"
#include "../src/ASTPrinter.hpp"

TEST(ParserTest, LetStatement) {
    std::string source = "let a = 10;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::unique_ptr<Stmt>> statements = parser.parse();
    ASTPrinter printer;
    std::string result = printer.print(statements);
    EXPECT_EQ(result, "(var a 10.000000)");
}
