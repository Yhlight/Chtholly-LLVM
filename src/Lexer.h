#ifndef CHTHOLLY_LEXER_H
#define CHTHOLLY_LEXER_H

#include "Token.h"
#include <string>
#include <vector>

namespace Chtholly {

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> scanTokens();

private:
    Token scanToken();
    char advance();
    bool match(char expected);
    char peek();
    char peekNext();
    void string();
    void number();
    void identifier();
    void lineComment();
    void blockComment();

    const std::string& source;
    std::string text;
    int start = 0;
    int current = 0;
    int line = 1;
    int column = 1;
    std::vector<Token> tokens;
};

} // namespace Chtholly

#endif // CHTHOLLY_LEXER_H
