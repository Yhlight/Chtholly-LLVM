#include <gtest/gtest.h>
#include "../src/Lexer.hpp"
#include "../src/Token.hpp"

TEST(LexerTest, TokenizesBasicOperators) {
    std::string source = "+ - * / %";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    ASSERT_EQ(tokens.size(), 6);
    EXPECT_EQ(tokens[0].type, TokenType::PLUS);
    EXPECT_EQ(tokens[1].type, TokenType::MINUS);
    EXPECT_EQ(tokens[2].type, TokenType::STAR);
    EXPECT_EQ(tokens[3].type, TokenType::SLASH);
    EXPECT_EQ(tokens[4].type, TokenType::PERCENT);
    EXPECT_EQ(tokens[5].type, TokenType::END_OF_FILE);
}

TEST(LexerTest, TokenizesKeywordsAndIdentifiers) {
    std::string source = "let x = 10; fn main() {}";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    ASSERT_EQ(tokens.size(), 12);
    EXPECT_EQ(tokens[0].type, TokenType::LET);
    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[2].type, TokenType::EQUAL);
    EXPECT_EQ(tokens[3].type, TokenType::INTEGER);
    EXPECT_EQ(tokens[4].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[5].type, TokenType::FN);
    EXPECT_EQ(tokens[6].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[7].type, TokenType::LEFT_PAREN);
    EXPECT_EQ(tokens[8].type, TokenType::RIGHT_PAREN);
    EXPECT_EQ(tokens[9].type, TokenType::LEFT_BRACE);
    EXPECT_EQ(tokens[10].type, TokenType::RIGHT_BRACE);
    EXPECT_EQ(tokens[11].type, TokenType::END_OF_FILE);
}

TEST(LexerTest, TokenizesStringLiterals) {
    std::string source = "\"hello world\"";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::STRING);
    EXPECT_EQ(tokens[0].lexeme, "\"hello world\"");
    EXPECT_EQ(tokens[1].type, TokenType::END_OF_FILE);
}

TEST(LexerTest, HandlesComments) {
    std::string source = "// this is a comment\nlet x = 10; /* another comment */";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    ASSERT_EQ(tokens.size(), 6);
    EXPECT_EQ(tokens[0].type, TokenType::LET);
    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[2].type, TokenType::EQUAL);
    EXPECT_EQ(tokens[3].type, TokenType::INTEGER);
    EXPECT_EQ(tokens[4].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[5].type, TokenType::END_OF_FILE);
}

TEST(LexerTest, HandlesMultiLineCommentWithAsterisk) {
    std::string source = "/* this is a multi-line comment with an asterisk * inside */";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();

    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::END_OF_FILE);
}
