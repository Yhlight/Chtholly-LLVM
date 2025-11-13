#include <gtest/gtest.h>
#include "lexer/Token.h"
#include "lexer/Lexer.h"
#include <vector>

TEST(TokenTest, TokenCreation) {
    Chtholly::Token token{Chtholly::TokenType::Int, "123", 1, 5};
    ASSERT_EQ(token.type, Chtholly::TokenType::Int);
    ASSERT_EQ(token.literal, "123");
    ASSERT_EQ(token.line, 1);
    ASSERT_EQ(token.column, 5);
}

TEST(LexerTest, NextTokenSimple) {
    std::string input = "=+(){},;";
    Chtholly::Lexer lexer(input);

    std::vector<Chtholly::Token> expected_tokens = {
        {Chtholly::TokenType::Assign, "=", 1, 1},
        {Chtholly::TokenType::Plus, "+", 1, 2},
        {Chtholly::TokenType::LParen, "(", 1, 3},
        {Chtholly::TokenType::RParen, ")", 1, 4},
        {Chtholly::TokenType::LBrace, "{", 1, 5},
        {Chtholly::TokenType::RBrace, "}", 1, 6},
        {Chtholly::TokenType::Comma, ",", 1, 7},
        {Chtholly::TokenType::Semicolon, ";", 1, 8},
        {Chtholly::TokenType::Eof, "", 1, 9}
    };

    for (const auto& expected_token : expected_tokens) {
        Chtholly::Token actual_token = lexer.NextToken();
        EXPECT_EQ(actual_token.type, expected_token.type);
        EXPECT_EQ(actual_token.literal, expected_token.literal);
        EXPECT_EQ(actual_token.line, expected_token.line);
        EXPECT_EQ(actual_token.column, expected_token.column);
    }
}

TEST(LexerTest, NextTokenComplex) {
    std::string input = R"(
        let five = 5;
        let ten = 10;
        let add = func(x, y) {
            x + y;
        };
        let result = add(five, ten);
    )";
    Chtholly::Lexer lexer(input);

    std::vector<Chtholly::Token> expected_tokens = {
        {Chtholly::TokenType::Let, "let", 2, 9},
        {Chtholly::TokenType::Identifier, "five", 2, 13},
        {Chtholly::TokenType::Assign, "=", 2, 18},
        {Chtholly::TokenType::Int, "5", 2, 20},
        {Chtholly::TokenType::Semicolon, ";", 2, 21},
        {Chtholly::TokenType::Let, "let", 3, 9},
        {Chtholly::TokenType::Identifier, "ten", 3, 13},
        {Chtholly::TokenType::Assign, "=", 3, 17},
        {Chtholly::TokenType::Int, "10", 3, 19},
        {Chtholly::TokenType::Semicolon, ";", 3, 21},
        {Chtholly::TokenType::Let, "let", 4, 9},
        {Chtholly::TokenType::Identifier, "add", 4, 13},
        {Chtholly::TokenType::Assign, "=", 4, 17},
        {Chtholly::TokenType::Function, "func", 4, 19},
        {Chtholly::TokenType::LParen, "(", 4, 23},
        {Chtholly::TokenType::Identifier, "x", 4, 24},
        {Chtholly::TokenType::Comma, ",", 4, 25},
        {Chtholly::TokenType::Identifier, "y", 4, 27},
        {Chtholly::TokenType::RParen, ")", 4, 28},
        {Chtholly::TokenType::LBrace, "{", 4, 30},
        {Chtholly::TokenType::Identifier, "x", 5, 13},
        {Chtholly::TokenType::Plus, "+", 5, 15},
        {Chtholly::TokenType::Identifier, "y", 5, 17},
        {Chtholly::TokenType::Semicolon, ";", 5, 18},
        {Chtholly::TokenType::RBrace, "}", 6, 9},
        {Chtholly::TokenType::Semicolon, ";", 6, 10},
        {Chtholly::TokenType::Let, "let", 7, 9},
        {Chtholly::TokenType::Identifier, "result", 7, 13},
        {Chtholly::TokenType::Assign, "=", 7, 20},
        {Chtholly::TokenType::Identifier, "add", 7, 22},
        {Chtholly::TokenType::LParen, "(", 7, 25},
        {Chtholly::TokenType::Identifier, "five", 7, 26},
        {Chtholly::TokenType::Comma, ",", 7, 30},
        {Chtholly::TokenType::Identifier, "ten", 7, 32},
        {Chtholly::TokenType::RParen, ")", 7, 35},
        {Chtholly::TokenType::Semicolon, ";", 7, 36},
        {Chtholly::TokenType::Eof, "", 8, 5}
    };

    for (const auto& expected_token : expected_tokens) {
        Chtholly::Token actual_token = lexer.NextToken();
        EXPECT_EQ(actual_token.type, expected_token.type);
        EXPECT_EQ(actual_token.literal, expected_token.literal);
        EXPECT_EQ(actual_token.line, expected_token.line);
        EXPECT_EQ(actual_token.column, expected_token.column);
    }
}
