#include <gtest/gtest.h>
#include "../src/Lexer.hpp"

TEST(LexerTest, LetStatement) {
    std::string source = "let a = 10;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 6);
    EXPECT_EQ(tokens[0].type, TokenType::LET);
    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[1].lexeme, "a");
    EXPECT_EQ(tokens[2].type, TokenType::EQUAL);
    EXPECT_EQ(tokens[3].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[3].lexeme, "10");
    EXPECT_EQ(tokens[4].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[5].type, TokenType::END_OF_FILE);
}

TEST(LexerTest, BlockCommentAndColon) {
    std::string source = "/* let a = 10; */ let b: int = 20;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 8);
    EXPECT_EQ(tokens[0].type, TokenType::LET);
    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[1].lexeme, "b");
    EXPECT_EQ(tokens[2].type, TokenType::COLON);
    EXPECT_EQ(tokens[3].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[3].lexeme, "int");
    EXPECT_EQ(tokens[4].type, TokenType::EQUAL);
    EXPECT_EQ(tokens[5].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[5].lexeme, "20");
    EXPECT_EQ(tokens[6].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[7].type, TokenType::END_OF_FILE);
}

TEST(LexerTest, SlashEqualAndLiterals) {
    std::string source = "let a = 10.5; a /= 2.5; let b = \"hello\";";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 15);
    EXPECT_EQ(tokens[0].type, TokenType::LET);
    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[1].lexeme, "a");
    EXPECT_EQ(tokens[2].type, TokenType::EQUAL);
    EXPECT_EQ(tokens[3].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[3].lexeme, "10.5");
    EXPECT_EQ(std::any_cast<double>(tokens[3].literal), 10.5);
    EXPECT_EQ(tokens[4].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[5].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[5].lexeme, "a");
    EXPECT_EQ(tokens[6].type, TokenType::SLASH_EQUAL);
    EXPECT_EQ(tokens[7].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[7].lexeme, "2.5");
    EXPECT_EQ(std::any_cast<double>(tokens[7].literal), 2.5);
    EXPECT_EQ(tokens[8].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[9].type, TokenType::LET);
    EXPECT_EQ(tokens[10].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[10].lexeme, "b");
    EXPECT_EQ(tokens[11].type, TokenType::EQUAL);
    EXPECT_EQ(tokens[12].type, TokenType::STRING);
    EXPECT_EQ(tokens[12].lexeme, "\"hello\"");
    EXPECT_EQ(std::any_cast<std::string>(tokens[12].literal), "hello");
    EXPECT_EQ(tokens[13].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[14].type, TokenType::END_OF_FILE);
}
