#pragma once

#include <string>
#include <any>

enum class TokenType {
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    LEFT_BRACKET, RIGHT_BRACKET, COMMA, DOT, MINUS, PLUS,
    SEMICOLON, SLASH, STAR, PERCENT, TILDE, COLON,

    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    PLUS_EQUAL, MINUS_EQUAL, STAR_EQUAL, SLASH_EQUAL, PERCENT_EQUAL,
    AMPERSAND, AMPERSAND_AMPERSAND, PIPE, PIPE_PIPE,
    CARET, LESS_LESS, GREATER_GREATER,
    PLUS_PLUS, MINUS_MINUS,
    ARROW, COLON_COLON,

    // Literals.
    IDENTIFIER, STRING, NUMBER, CHARACTER,

    // Keywords.
    LET, MUT, FN, CLASS, PUBLIC, PRIVATE, STATIC,
    IF, ELSE, SWITCH, CASE, BREAK, FALLTHROUGH,
    WHILE, FOR, DO, RETURN, TRUE, FALSE, THIS,
    ENUM, IMPORT, AS, PACKAGE,

    // End of file.
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;
};
