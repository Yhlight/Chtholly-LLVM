#pragma once

#include "Token.hpp"
#include <string>
#include <vector>
#include <unordered_map>

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> scan_tokens();

private:
    void scan_token();
    char advance();
    void add_token(TokenType type);
    bool match(char expected);
    char peek();
    char peek_next();
    void string();
    void number();
    void identifier();

    std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

    static const std::unordered_map<std::string, TokenType> keywords;
};
