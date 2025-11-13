#ifndef CHTHOLLY_LEXER_HPP
#define CHTHOLLY_LEXER_HPP

#include "Token.hpp"
#include <string>
#include <vector>
#include <unordered_map>

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    void scanToken();
    char advance();
    bool match(char expected);
    char peek();
    char peekNext();

    void string();
    void number();
    void identifier();
    void blockComment();

    bool isAtEnd();
    bool isDigit(char c);
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);

    const std::string& source;
    std::vector<Token> tokens;
    static const std::unordered_map<std::string, TokenType> keywords;

    int start = 0;
    int current = 0;
    int line = 1;
};

#endif // CHTHOLLY_LEXER_HPP
