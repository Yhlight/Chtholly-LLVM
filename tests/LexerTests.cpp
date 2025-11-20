#include <gtest/gtest.h>
#include "Lexer.hpp"
#include "Token.hpp"

TEST(LexerTest, TokenizesSingleCharacterTokens) {
    std::string source = "(){}[],.-+;*/%&|^~:";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 20);
    EXPECT_EQ(tokens[0].type, TokenType::LEFT_PAREN);
    EXPECT_EQ(tokens[1].type, TokenType::RIGHT_PAREN);
    EXPECT_EQ(tokens[2].type, TokenType::LEFT_BRACE);
    EXPECT_EQ(tokens[3].type, TokenType::RIGHT_BRACE);
    EXPECT_EQ(tokens[4].type, TokenType::LEFT_BRACKET);
    EXPECT_EQ(tokens[5].type, TokenType::RIGHT_BRACKET);
    EXPECT_EQ(tokens[6].type, TokenType::COMMA);
    EXPECT_EQ(tokens[7].type, TokenType::DOT);
    EXPECT_EQ(tokens[8].type, TokenType::MINUS);
    EXPECT_EQ(tokens[9].type, TokenType::PLUS);
    EXPECT_EQ(tokens[10].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[11].type, TokenType::STAR);
    EXPECT_EQ(tokens[12].type, TokenType::SLASH);
    EXPECT_EQ(tokens[13].type, TokenType::PERCENT);
    EXPECT_EQ(tokens[14].type, TokenType::AMPERSAND);
    EXPECT_EQ(tokens[15].type, TokenType::PIPE);
    EXPECT_EQ(tokens[16].type, TokenType::CARET);
    EXPECT_EQ(tokens[17].type, TokenType::TILDE);
    EXPECT_EQ(tokens[18].type, TokenType::COLON);
    EXPECT_EQ(tokens[19].type, TokenType::END_OF_FILE);
}

TEST(LexerTest, TokenizesTwoCharacterTokens) {
    std::string source = "!= == <= >= ++ -- += -= *= /= %= && || << >> ::";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 17);
    EXPECT_EQ(tokens[0].type, TokenType::BANG_EQUAL);
    EXPECT_EQ(tokens[1].type, TokenType::EQUAL_EQUAL);
    EXPECT_EQ(tokens[2].type, TokenType::LESS_EQUAL);
    EXPECT_EQ(tokens[3].type, TokenType::GREATER_EQUAL);
    EXPECT_EQ(tokens[4].type, TokenType::PLUS_PLUS);
    EXPECT_EQ(tokens[5].type, TokenType::MINUS_MINUS);
    EXPECT_EQ(tokens[6].type, TokenType::PLUS_EQUAL);
    EXPECT_EQ(tokens[7].type, TokenType::MINUS_EQUAL);
    EXPECT_EQ(tokens[8].type, TokenType::STAR_EQUAL);
    EXPECT_EQ(tokens[9].type, TokenType::SLASH_EQUAL);
    EXPECT_EQ(tokens[10].type, TokenType::PERCENT_EQUAL);
    EXPECT_EQ(tokens[11].type, TokenType::AMPERSAND_AMPERSAND);
    EXPECT_EQ(tokens[12].type, TokenType::PIPE_PIPE);
    EXPECT_EQ(tokens[13].type, TokenType::LESS_LESS);
    EXPECT_EQ(tokens[14].type, TokenType::GREATER_GREATER);
    EXPECT_EQ(tokens[15].type, TokenType::COLON_COLON);
    EXPECT_EQ(tokens[16].type, TokenType::END_OF_FILE);
}

TEST(LexerTest, TokenizesLiterals) {
    std::string source = "\"hello\" 123 45.67 my_var";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 5);
    EXPECT_EQ(tokens[0].type, TokenType::STRING);
    EXPECT_EQ(std::any_cast<std::string>(tokens[0].literal), "hello");
    EXPECT_EQ(tokens[1].type, TokenType::NUMBER);
    EXPECT_EQ(std::any_cast<double>(tokens[1].literal), 123);
    EXPECT_EQ(tokens[2].type, TokenType::NUMBER);
    EXPECT_EQ(std::any_cast<double>(tokens[2].literal), 45.67);
    EXPECT_EQ(tokens[3].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[3].lexeme, "my_var");
}

TEST(LexerTest, TokenizesKeywords) {
    std::string source = "fn let mut if else while for return";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 9);
    EXPECT_EQ(tokens[0].type, TokenType::FN);
    EXPECT_EQ(tokens[1].type, TokenType::LET);
    EXPECT_EQ(tokens[2].type, TokenType::MUT);
    EXPECT_EQ(tokens[3].type, TokenType::IF);
    EXPECT_EQ(tokens[4].type, TokenType::ELSE);
    EXPECT_EQ(tokens[5].type, TokenType::WHILE);
    EXPECT_EQ(tokens[6].type, TokenType::FOR);
    EXPECT_EQ(tokens[7].type, TokenType::RETURN);
}

TEST(LexerTest, IgnoresComments) {
    std::string source = "// this is a comment\nlet x = 10; /* block comment */";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 6);
    EXPECT_EQ(tokens[0].type, TokenType::LET);
    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[2].type, TokenType::EQUAL);
    EXPECT_EQ(tokens[3].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[4].type, TokenType::SEMICOLON);
}
