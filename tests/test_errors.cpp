#include <gtest/gtest.h>
#include "../src/Chtholly.hpp"
#include "../src/Parser.hpp"
#include "../src/Lexer.hpp"
#include <iostream>
#include <streambuf>

using namespace chtholly;

TEST(ErrorTest, ReportError) {
    // Redirect cerr to a string buffer
    std::streambuf* oldCerr = std::cerr.rdbuf();
    std::stringstream newCerr;
    std::cerr.rdbuf(newCerr.rdbuf());

    std::string source = "let x = 10"; // Missing semicolon
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    parser.parse();

    // Restore cerr
    std::cerr.rdbuf(oldCerr);

    // Check if anything was written to the error stream
    EXPECT_FALSE(newCerr.str().empty());
}
