#ifndef CHTHOLLY_LEXER_HPP
#define CHTHOLLY_LEXER_HPP

#include "Token.hpp"
#include <vector>
#include <string>

namespace chtholly {

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> scan_tokens();

private:
    void scan_token();
    char advance();
    void add_token(TokenType type, const std::any& literal = {});
    bool match(char expected);
    char peek() const;
    char peek_next() const;
    void string();
    void number();
    void identifier();

    const std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;
};

} // namespace chtholly

#endif //CHTHOLLY_LEXER_HPP
