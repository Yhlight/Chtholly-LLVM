#include <gtest/gtest.h>
#include "../src/Lexer.hpp"
#include "../src/Token.hpp"

using namespace chtholly;

TEST(LexerTest, SingleTokens) {
    std::string source = "(){}[],.-+*;/%!:~=&|";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    std::vector<TokenType> expected = {
        TokenType::LEFT_PAREN, TokenType::RIGHT_PAREN, TokenType::LEFT_BRACE, TokenType::RIGHT_BRACE,
        TokenType::LEFT_BRACKET, TokenType::RIGHT_BRACKET, TokenType::COMMA, TokenType::DOT,
        TokenType::MINUS, TokenType::PLUS, TokenType::STAR, TokenType::SEMICOLON, TokenType::SLASH, TokenType::PERCENT,
        TokenType::BANG, TokenType::COLON, TokenType::TILDE,
        TokenType::EQUAL, TokenType::AMPERSAND, TokenType::PIPE,
        TokenType::END_OF_FILE
    };

    ASSERT_EQ(tokens.size(), expected.size());
    for (size_t i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i].type, expected[i]);
    }
}

TEST(LexerTest, DoubleTokens) {
    std::string source = "!= == <= >= && || ++ -- += -= *= /= %=";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    std::vector<TokenType> expected = {
        TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL, TokenType::LESS_EQUAL, TokenType::GREATER_EQUAL,
        TokenType::AMPERSAND_AMPERSAND, TokenType::PIPE_PIPE, TokenType::PLUS_PLUS, TokenType::MINUS_MINUS,
        TokenType::PLUS_EQUAL, TokenType::MINUS_EQUAL, TokenType::STAR_EQUAL, TokenType::SLASH_EQUAL,
        TokenType::PERCENT_EQUAL, TokenType::END_OF_FILE
    };

    ASSERT_EQ(tokens.size(), expected.size());
    for (size_t i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i].type, expected[i]);
    }
}

TEST(LexerTest, Literals) {
    std::string source = "\"hello\" 123 45.67 'a'";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 5);
    EXPECT_EQ(tokens[0].type, TokenType::STRING);
    EXPECT_EQ(std::any_cast<std::string>(tokens[0].literal), "hello");
    EXPECT_EQ(tokens[1].type, TokenType::NUMBER_INT);
    EXPECT_EQ(std::any_cast<int>(tokens[1].literal), 123);
    EXPECT_EQ(tokens[2].type, TokenType::NUMBER_FLOAT);
    EXPECT_EQ(std::any_cast<double>(tokens[2].literal), 45.67);
    EXPECT_EQ(tokens[3].type, TokenType::CHAR);
    EXPECT_EQ(std::any_cast<char>(tokens[3].literal), 'a');
    EXPECT_EQ(tokens[4].type, TokenType::END_OF_FILE);
}

TEST(LexerTest, KeywordsAndIdentifiers) {
    std::string source = "let mut var = fn myFunc";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    std::vector<TokenType> expected = {
        TokenType::LET, TokenType::MUT, TokenType::IDENTIFIER, TokenType::EQUAL,
        TokenType::FN, TokenType::IDENTIFIER, TokenType::END_OF_FILE
    };

    ASSERT_EQ(tokens.size(), expected.size());
    for (size_t i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i].type, expected[i]);
    }
    EXPECT_EQ(tokens[2].lexeme, "var");
    EXPECT_EQ(tokens[5].lexeme, "myFunc");
}

TEST(LexerTest, Comments) {
    std::string source = R"(
        // This is a comment.
        let x = 10; // Another comment.
        /* This is a
         * block comment.
         */
        let y = 20;
    )";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    std::vector<TokenType> expected = {
        TokenType::LET, TokenType::IDENTIFIER, TokenType::EQUAL, TokenType::NUMBER_INT, TokenType::SEMICOLON,
        TokenType::LET, TokenType::IDENTIFIER, TokenType::EQUAL, TokenType::NUMBER_INT, TokenType::SEMICOLON,
        TokenType::END_OF_FILE
    };

    ASSERT_EQ(tokens.size(), expected.size());
    for (size_t i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i].type, expected[i]);
    }
}
