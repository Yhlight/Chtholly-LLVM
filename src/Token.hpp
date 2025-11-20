#pragma once

#include <string>
#include <unordered_map>

enum class TokenType {
    // Punctuators
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE,
    LEFT_BRACKET, RIGHT_BRACKET,
    COMMA, DOT, COLON, SEMICOLON,
    ARROW,

    // Operators
    PLUS, MINUS, STAR, SLASH, PERCENT,
    EQUAL, EQUAL_EQUAL,
    BANG, BANG_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    AMPERSAND, AMPERSAND_AMPERSAND,
    PIPE, PIPE_PIPE,
    CARET, TILDE,
    LEFT_SHIFT, RIGHT_SHIFT,
    PLUS_EQUAL, MINUS_EQUAL, STAR_EQUAL, SLASH_EQUAL, PERCENT_EQUAL,
    PLUS_PLUS, MINUS_MINUS,

    // Literals
    IDENTIFIER, STRING, INTEGER, DOUBLE, CHAR,

    // Keywords
    FN, LET, MUT,
    IF, ELSE,
    WHILE, FOR,
    RETURN,
    CLASS, PRIVATE, PUBLIC, STATIC, THIS,
    SWITCH, CASE, BREAK, FALLTHROUGH, DO,
    ENUM,
    IMPORT, AS, PACKAGE,
    TRUE, FALSE,

    // Special
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
};

// Helper to convert TokenType to string for printing/debugging
static std::string token_type_to_string(TokenType type) {
    static const std::unordered_map<TokenType, std::string> type_map = {
        {TokenType::LEFT_PAREN, "LEFT_PAREN"}, {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
        {TokenType::LEFT_BRACE, "LEFT_BRACE"}, {TokenType::RIGHT_BRACE, "RIGHT_BRACE"},
        {TokenType::LEFT_BRACKET, "LEFT_BRACKET"}, {TokenType::RIGHT_BRACKET, "RIGHT_BRACKET"},
        {TokenType::COMMA, "COMMA"}, {TokenType::DOT, "DOT"}, {TokenType::COLON, "COLON"}, {TokenType::SEMICOLON, "SEMICOLON"},
        {TokenType::ARROW, "ARROW"},
        {TokenType::PLUS, "PLUS"}, {TokenType::MINUS, "MINUS"}, {TokenType::STAR, "STAR"}, {TokenType::SLASH, "SLASH"}, {TokenType::PERCENT, "PERCENT"},
        {TokenType::EQUAL, "EQUAL"}, {TokenType::EQUAL_EQUAL, "EQUAL_EQUAL"},
        {TokenType::BANG, "BANG"}, {TokenType::BANG_EQUAL, "BANG_EQUAL"},
        {TokenType::GREATER, "GREATER"}, {TokenType::GREATER_EQUAL, "GREATER_EQUAL"},
        {TokenType::LESS, "LESS"}, {TokenType::LESS_EQUAL, "LESS_EQUAL"},
        {TokenType::AMPERSAND, "AMPERSAND"}, {TokenType::AMPERSAND_AMPERSAND, "AMPERSAND_AMPERSAND"},
        {TokenType::PIPE, "PIPE"}, {TokenType::PIPE_PIPE, "PIPE_PIPE"},
        {TokenType::CARET, "CARET"}, {TokenType::TILDE, "TILDE"},
        {TokenType::LEFT_SHIFT, "LEFT_SHIFT"}, {TokenType::RIGHT_SHIFT, "RIGHT_SHIFT"},
        {TokenType::PLUS_EQUAL, "PLUS_EQUAL"}, {TokenType::MINUS_EQUAL, "MINUS_EQUAL"}, {TokenType::STAR_EQUAL, "STAR_EQUAL"}, {TokenType::SLASH_EQUAL, "SLASH_EQUAL"}, {TokenType::PERCENT_EQUAL, "PERCENT_EQUAL"},
        {TokenType::PLUS_PLUS, "PLUS_PLUS"}, {TokenType::MINUS_MINUS, "MINUS_MINUS"},
        {TokenType::IDENTIFIER, "IDENTIFIER"}, {TokenType::STRING, "STRING"}, {TokenType::INTEGER, "INTEGER"}, {TokenType::DOUBLE, "DOUBLE"}, {TokenType::CHAR, "CHAR"},
        {TokenType::FN, "FN"}, {TokenType::LET, "LET"}, {TokenType::MUT, "MUT"},
        {TokenType::IF, "IF"}, {TokenType::ELSE, "ELSE"},
        {TokenType::WHILE, "WHILE"}, {TokenType::FOR, "FOR"},
        {TokenType::RETURN, "RETURN"},
        {TokenType::CLASS, "CLASS"}, {TokenType::PRIVATE, "PRIVATE"}, {TokenType::PUBLIC, "PUBLIC"}, {TokenType::STATIC, "STATIC"}, {TokenType::THIS, "THIS"},
        {TokenType::SWITCH, "SWITCH"}, {TokenType::CASE, "CASE"}, {TokenType::BREAK, "BREAK"}, {TokenType::FALLTHROUGH, "FALLTHROUGH"}, {TokenType::DO, "DO"},
        {TokenType::ENUM, "ENUM"},
        {TokenType::IMPORT, "IMPORT"}, {TokenType::AS, "AS"}, {TokenType::PACKAGE, "PACKAGE"},
        {TokenType::TRUE, "TRUE"}, {TokenType::FALSE, "FALSE"},
        {TokenType::END_OF_FILE, "EOF"}
    };
    auto it = type_map.find(type);
    if (it != type_map.end()) {
        return it->second;
    }
    return "UNKNOWN";
}
