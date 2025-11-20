#ifndef CHTHOLLY_LEXER_HPP
#define CHTHOLLY_LEXER_HPP

#include "Token.hpp"
#include <string>
#include <vector>
#include <unordered_map>

namespace chtholly {

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> scanTokens();

private:
    void scanToken();
    char advance();
    void addToken(TokenType type, const std::any& literal = {});
    bool match(char expected);
    char peek() const;
    char peekNext() const;

    void string();
    void number();
    void identifier();

    bool isAtEnd() const;
    bool isDigit(char c) const;
    bool isAlpha(char c) const;
    bool isAlphaNumeric(char c) const;

    const std::string source;
    std::vector<Token> tokens;
    static const std::unordered_map<std::string, TokenType> keywords;

    int start = 0;
    int current = 0;
    int line = 1;
};

} // namespace chtholly

#endif // CHTHOLLY_LEXER_HPP
