#pragma once

#include "Token.hpp"
#include <string>
#include <vector>

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> scanTokens();

private:
    void scanToken();
    char advance();
    void addToken(TokenType type, const std::any& literal = {});
    bool match(char expected);
    void string();
    void number();
    void identifier();
    char peek();
    char peekNext();

    const std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;
};
