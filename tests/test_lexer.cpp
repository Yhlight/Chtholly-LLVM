#include "../src/Lexer.h"
#include <cassert>
#include <iostream>
#include <vector>

void test_addition() {
    assert(1 + 1 == 2);
}

void test_simple_declaration() {
    std::string source = "let x = 10;";
    Chtholly::Lexer lexer(source);
    std::vector<Chtholly::Token> tokens = lexer.scanTokens();

    assert(tokens.size() == 6);
    assert(tokens[0].type == Chtholly::TokenType::Let);
    assert(tokens[1].type == Chtholly::TokenType::Identifier);
    assert(tokens[1].lexeme == "x");
    assert(tokens[2].type == Chtholly::TokenType::Equal);
    assert(tokens[3].type == Chtholly::TokenType::IntegerLiteral);
    assert(std::get<int64_t>(tokens[3].literal) == 10);
    assert(tokens[4].type == Chtholly::TokenType::Semicolon);
}

void test_various_tokens() {
    std::string source = R"(
        let a = 10;
        mut b = 20.5;
        let c = 'x';
        let d = "hello";
        // this is a comment
        let e = a + b;
        /*
         * block comment
         */
    )";
    Chtholly::Lexer lexer(source);
    std::vector<Chtholly::Token> tokens = lexer.scanTokens();

    std::vector<Chtholly::TokenType> expected_types = {
        Chtholly::TokenType::Let, Chtholly::TokenType::Identifier, Chtholly::TokenType::Equal, Chtholly::TokenType::IntegerLiteral, Chtholly::TokenType::Semicolon,
        Chtholly::TokenType::Mut, Chtholly::TokenType::Identifier, Chtholly::TokenType::Equal, Chtholly::TokenType::DoubleLiteral, Chtholly::TokenType::Semicolon,
        Chtholly::TokenType::Let, Chtholly::TokenType::Identifier, Chtholly::TokenType::Equal, Chtholly::TokenType::CharLiteral, Chtholly::TokenType::Semicolon,
        Chtholly::TokenType::Let, Chtholly::TokenType::Identifier, Chtholly::TokenType::Equal, Chtholly::TokenType::StringLiteral, Chtholly::TokenType::Semicolon,
        Chtholly::TokenType::Let, Chtholly::TokenType::Identifier, Chtholly::TokenType::Equal, Chtholly::TokenType::Identifier, Chtholly::TokenType::Plus, Chtholly::TokenType::Identifier, Chtholly::TokenType::Semicolon,
        Chtholly::TokenType::Eof
    };

    assert(tokens.size() == expected_types.size());
    for (size_t i = 0; i < tokens.size(); ++i) {
        assert(tokens[i].type == expected_types[i]);
    }

    assert(std::get<int64_t>(tokens[3].literal) == 10);
    assert(std::get<double>(tokens[8].literal) == 20.5);
    assert(std::get<char>(tokens[13].literal) == 'x');
    assert(std::get<std::string>(tokens[18].literal) == "hello");
}

void test_all() {
    test_addition();
    test_simple_declaration();
    test_various_tokens();
}

int main() {
    test_all();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
