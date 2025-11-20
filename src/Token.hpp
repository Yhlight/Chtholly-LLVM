#ifndef CHTHOLLY_TOKEN_HPP
#define CHTHOLLY_TOKEN_HPP

#include <string>
#include <any>

enum class TokenType {
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    LEFT_BRACKET, RIGHT_BRACKET,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
    PERCENT, AMPERSAND, PIPE, CARET, TILDE,

    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    PLUS_PLUS, PLUS_EQUAL,
    MINUS_MINUS, MINUS_EQUAL,
    STAR_EQUAL, SLASH_EQUAL, PERCENT_EQUAL,
    AMPERSAND_AMPERSAND, PIPE_PIPE,
    LESS_LESS, GREATER_GREATER,
    COLON, COLON_COLON,

    // Literals.
    IDENTIFIER, STRING, NUMBER, CHAR,

    // Keywords.
    FN, LET, MUT, IF, ELSE, WHILE, FOR, RETURN,
    CLASS, ENUM, IMPORT, AS, PACKAGE, SWITCH, CASE,
    FALLTHROUGH, DO, STATIC, PUBLIC, PRIVATE, THIS,
    TRUE, FALSE,

    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;
};

#endif // CHTHOLLY_TOKEN_HPP
