#ifndef CHTHOLLY_LEXER_H
#define CHTHOLLY_LEXER_H

#include "Token.h"
#include <string>

namespace Chtholly
{
    class Lexer
    {
    public:
        Lexer(const std::string& input);
        Token NextToken();

    private:
        void SkipWhitespace();
        char PeekChar();
        char ReadChar();
        std::string ReadIdentifier();
        std::string ReadNumber();
        Token ReadString();
        bool IsLetter(char ch);
        bool IsDigit(char ch);

        std::string m_input;
        int m_position = 0;
        int m_readPosition = 0;
        char m_char = 0;
        int m_line = 1;
        int m_column = 0;
    };

} // namespace Chtholly

#endif // CHTHOLLY_LEXER_H
