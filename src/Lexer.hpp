#ifndef CHTHOLLY_LEXER_HPP
#define CHTHOLLY_LEXER_HPP

#include <any>
#include <string>
#include <vector>

namespace chtholly {

enum class TokenType {
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    LEFT_BRACKET, RIGHT_BRACKET,
    COMMA, DOT, MINUS, PLUS, SLASH, STAR,
    COLON, SEMICOLON, TILDE,

    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    PLUS_EQUAL, MINUS_EQUAL, STAR_EQUAL, SLASH_EQUAL,
    PERCENT, PERCENT_EQUAL,
    AMPERSAND, AMPERSAND_AMPERSAND,
    PIPE, PIPE_PIPE,
    CARET,
    LESS_LESS, GREATER_GREATER,

    // Literals.
    IDENTIFIER, STRING, NUMBER,

    // Keywords.
    LET, MUT,
    IF, ELSE,
    SWITCH, CASE, FALLTHROUGH,
    WHILE, FOR, DO, BREAK,
    FN, RETURN,
    CLASS, THIS, STATIC,
    PUBLIC, PRIVATE,
    ENUM,
    IMPORT, AS, PACKAGE,
    TYPE_CAST,

    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;
};

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> scanTokens();

private:
    void scanToken();
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, const std::any& literal);
    bool match(char expected);
    char peek();
    char peekNext();
    void string();
    void number();
    void identifier();

    std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;
};

} // namespace chtholly

#endif // CHTHOLLY_LEXER_HPP
