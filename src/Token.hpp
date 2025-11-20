#ifndef CHTHOLLY_TOKEN_HPP
#define CHTHOLLY_TOKEN_HPP

#include <string>
#include <any>

namespace chtholly {

enum class TokenType {
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    LEFT_BRACKET, RIGHT_BRACKET, COMMA, DOT, MINUS, PLUS,
    SEMICOLON, SLASH, STAR, PERCENT, COLON,

    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    AMPERSAND, PIPE, AMPERSAND_AMPERSAND, PIPE_PIPE,
    PLUS_PLUS, MINUS_MINUS,
    PLUS_EQUAL, MINUS_EQUAL, STAR_EQUAL, SLASH_EQUAL, PERCENT_EQUAL,


    // Literals.
    IDENTIFIER, STRING, NUMBER, CHARACTER,

    // Keywords.
    LET, MUT, FN, IF, ELSE, WHILE, FOR, RETURN, TRUE, FALSE,
    CLASS, THIS, STATIC, IMPORT, AS, PACKAGE, ENUM, SWITCH,
    CASE, BREAK, FALLTHROUGH, DO, PRIVATE, PUBLIC, TYPE_CAST,

    // Type Keywords
    INT, I8, I16, I32, I64, U8, U16, U32, U64,
    CHAR, DOUBLE, FLOAT, LONG_DOUBLE, VOID, BOOL,

    // Special Tokens
    EOS // End of source
};

struct Token {
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;

    Token(TokenType type, std::string lexeme, std::any literal, int line)
        : type(type), lexeme(std::move(lexeme)), literal(std::move(literal)), line(line) {}

    std::string toString() const {
        return std::to_string(static_cast<int>(type)) + " " + lexeme;
    }
};

} // namespace chtholly

#endif //CHTHOLLY_TOKEN_HPP
