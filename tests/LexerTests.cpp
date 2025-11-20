#include <gtest/gtest.h>
#include "../src/Lexer.hpp"
#include "../src/Token.hpp"
#include <vector>

using namespace chtholly;

TEST(LexerTest, TestSingleCharacterTokens) {
    std::string source = "(){}[],.-+;*:%";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    std::vector<TokenType> expected_types = {
        TokenType::LEFT_PAREN, TokenType::RIGHT_PAREN, TokenType::LEFT_BRACE, TokenType::RIGHT_BRACE,
        TokenType::LEFT_BRACKET, TokenType::RIGHT_BRACKET, TokenType::COMMA, TokenType::DOT, TokenType::MINUS,
        TokenType::PLUS, TokenType::SEMICOLON, TokenType::STAR, TokenType::COLON, TokenType::PERCENT, TokenType::EOS
    };

    // The lexer will produce an extra EOS token at the end.
    // Let's adjust the expected size to account for that.
    ASSERT_EQ(tokens.size(), expected_types.size());

    for (size_t i = 0; i < expected_types.size(); ++i) {
        EXPECT_EQ(tokens[i].type, expected_types[i]);
    }
}

TEST(LexerTest, TestKeywords) {
    std::string source = "let mut fn if else";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    std::vector<TokenType> expected_types = {
        TokenType::LET, TokenType::MUT, TokenType::FN, TokenType::IF, TokenType::ELSE, TokenType::EOS
    };

    ASSERT_EQ(tokens.size(), expected_types.size());

    for (size_t i = 0; i < expected_types.size(); ++i) {
        EXPECT_EQ(tokens[i].type, expected_types[i]);
    }
}

TEST(LexerTest, TestNumberLiterals) {
    std::string source = "123 45.67";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].type, TokenType::NUMBER);
    EXPECT_EQ(std::any_cast<double>(tokens[0].literal), 123.0);
    EXPECT_EQ(tokens[1].type, TokenType::NUMBER);
    EXPECT_EQ(std::any_cast<double>(tokens[1].literal), 45.67);
    EXPECT_EQ(tokens[2].type, TokenType::EOS);
}

TEST(LexerTest, TestStringLiterals) {
    std::string source = "\"hello world\"";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::STRING);
    EXPECT_EQ(std::any_cast<std::string>(tokens[0].literal), "hello world");
    EXPECT_EQ(tokens[1].type, TokenType::EOS);
}
