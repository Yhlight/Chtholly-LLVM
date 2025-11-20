#include <gtest/gtest.h>
#include "../src/Lexer.hpp"

using namespace chtholly;

TEST(LexerTest, SingleCharacterTokens) {
    std::string source = "(){}[],.-+*;:";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    std::vector<TokenType> expected = {
        TokenType::LEFT_PAREN, TokenType::RIGHT_PAREN,
        TokenType::LEFT_BRACE, TokenType::RIGHT_BRACE,
        TokenType::LEFT_BRACKET, TokenType::RIGHT_BRACKET,
        TokenType::COMMA, TokenType::DOT, TokenType::MINUS,
        TokenType::PLUS, TokenType::STAR, TokenType::SEMICOLON,
        TokenType::COLON, TokenType::END_OF_FILE
    };

    ASSERT_EQ(tokens.size(), expected.size());
    for (size_t i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i].type, expected[i]);
    }
}

TEST(LexerTest, MultiCharacterTokens) {
    std::string source = "!= == <= >= += -= *= /= %= && || << >>";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    std::vector<TokenType> expected = {
        TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL,
        TokenType::LESS_EQUAL, TokenType::GREATER_EQUAL,
        TokenType::PLUS_EQUAL, TokenType::MINUS_EQUAL,
        TokenType::STAR_EQUAL, TokenType::SLASH_EQUAL,
        TokenType::PERCENT_EQUAL,
        TokenType::AMPERSAND_AMPERSAND, TokenType::PIPE_PIPE,
        TokenType::LESS_LESS, TokenType::GREATER_GREATER,
        TokenType::END_OF_FILE
    };

    ASSERT_EQ(tokens.size(), expected.size());
    for (size_t i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i].type, expected[i]);
    }
}

TEST(LexerTest, Comments) {
    std::string source = "// this is a comment\nlet x = 10; /* multi-line \n comment */";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    std::vector<TokenType> expected = {
        TokenType::LET, TokenType::IDENTIFIER, TokenType::EQUAL,
        TokenType::NUMBER, TokenType::SEMICOLON, TokenType::END_OF_FILE
    };

    ASSERT_EQ(tokens.size(), expected.size());
    for (size_t i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i].type, expected[i]);
    }
}

TEST(LexerTest, StringLiteral) {
    std::string source = "\"hello world\"";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::STRING);
    EXPECT_EQ(std::any_cast<std::string>(tokens[0].literal), "hello world");
}

TEST(LexerTest, NumberLiteral) {
    std::string source = "123.45";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::NUMBER);
    EXPECT_EQ(std::any_cast<double>(tokens[0].literal), 123.45);
}

TEST(LexerTest, Keywords) {
    std::string source = "let mut if else switch case fn return class this";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    std::vector<TokenType> expected = {
        TokenType::LET, TokenType::MUT, TokenType::IF, TokenType::ELSE,
        TokenType::SWITCH, TokenType::CASE, TokenType::FN, TokenType::RETURN,
        TokenType::CLASS, TokenType::THIS, TokenType::END_OF_FILE
    };

    ASSERT_EQ(tokens.size(), expected.size());
    for (size_t i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i].type, expected[i]);
    }
}
