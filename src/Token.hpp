#ifndef CHTHOLLY_TOKEN_HPP
#define CHTHOLLY_TOKEN_HPP

#include <string>
#include <any>

namespace chtholly {

enum class TokenType {
    // Single-character tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET,
    COMMA, DOT, MINUS, PLUS, SLASH, STAR, PERCENT,
    SEMICOLON, COLON, TILDE, COLON_COLON,

    // One or two character tokens
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    AMPERSAND, AMPERSAND_AMPERSAND,
    PIPE, PIPE_PIPE,
    PLUS_PLUS, MINUS_MINUS,
    PLUS_EQUAL, MINUS_EQUAL, STAR_EQUAL, SLASH_EQUAL, PERCENT_EQUAL,

    // Literals
    IDENTIFIER, STRING, NUMBER_INT, NUMBER_FLOAT, CHAR,

    // Keywords
    FN, LET, MUT, IF, ELSE, TRUE, FALSE, FOR, WHILE, DO,
    SWITCH, CASE, BREAK, FALLTHROUGH,
    CLASS, THIS, SUPER, RETURN,
    PRIVATE, PUBLIC, STATIC,
    ENUM, STRUCT,
    IMPORT, AS, PACKAGE,
    TYPE_CAST,

    // Generics
    TEMPLATE,

    // End of file
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;

    std::string toString() const {
        return "Type: " + std::to_string(static_cast<int>(type)) +
               ", Lexeme: '" + lexeme + "'";
    }
};

} // namespace chtholly

#endif // CHTHOLLY_TOKEN_HPP
