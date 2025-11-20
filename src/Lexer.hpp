#ifndef CHTHOLLY_LEXER_HPP
#define CHTHOLLY_LEXER_HPP

#include <string>
#include <vector>

namespace chtholly {

enum class TokenType {
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS,
    SEMICOLON, SLASH, STAR,

    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals.
    IDENTIFIER, STRING, NUMBER,

    // Keywords.
    AND, CLASS, ELSE, FALSE,
    FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS,
    TRUE, VAR, WHILE,

    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string lexeme;
    // std::any literal;
    int line;
};

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> scanTokens();

private:
    std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

    void scanToken();
    char advance();
    void addToken(TokenType type);
    bool match(char expected);
    char peek();
    void string();
    void number();
    void identifier();
};

} // namespace chtholly

#endif // CHTHOLLY_LEXER_HPP
