#ifndef CHTHOLLY_TOKEN_HPP
#define CHTHOLLY_TOKEN_HPP

#include <string>
#include <vector>

enum class TokenType {
    // Keywords
    LET, MUT, FUNC, RETURN, IF, ELSE, WHILE, FOR, STRUCT, ENUM, PUBLIC, PRIVATE,
    IMPL, IMPORT, SWITCH, CASE, FALLTHROUGH, BREAK, CONTINUE, TRUE, FALSE, NONE, SELF,

    // Literals
    IDENTIFIER, INTEGER, DOUBLE, STRING, CHAR,

    // Operators
    PLUS, MINUS, STAR, SLASH, PERCENT,
    ASSIGN, EQUAL, NOT_EQUAL, GREATER, LESS, GREATER_EQUAL, LESS_EQUAL,
    LOGICAL_AND, LOGICAL_OR, LOGICAL_NOT,
    BITWISE_AND, BITWISE_OR, BITWISE_XOR, BITWISE_NOT,
    LEFT_SHIFT, RIGHT_SHIFT,
    PLUS_ASSIGN, MINUS_ASSIGN, STAR_ASSIGN, SLASH_ASSIGN, PERCENT_ASSIGN,

    // Punctuation
    ARROW, COLON_COLON, DOT, COMMA, COLON, SEMICOLON,
    LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET,
    QUESTION,

    // End of File
    END_OF_FILE,

    // Unknown
    UNKNOWN
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;

    Token(TokenType type, const std::string& lexeme, int line)
        : type(type), lexeme(lexeme), line(line) {}
};

#endif // CHTHOLLY_TOKEN_HPP
