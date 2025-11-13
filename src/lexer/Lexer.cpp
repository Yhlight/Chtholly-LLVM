#include "Lexer.h"
#include "Token.h"

namespace Chtholly
{
    Lexer::Lexer(const std::string& input) : m_input(input)
    {
        ReadChar();
    }

    Token Lexer::NextToken()
    {
        SkipWhitespace();

        Token token;
        token.line = m_line;
        token.column = m_column;

        switch (m_char)
        {
        case '=':
            token = {TokenType::Assign, "=", m_line, m_column};
            break;
        case '+':
            token = {TokenType::Plus, "+", m_line, m_column};
            break;
        case '(':
            token = {TokenType::LParen, "(", m_line, m_column};
            break;
        case ')':
            token = {TokenType::RParen, ")", m_line, m_column};
            break;
        case '{':
            token = {TokenType::LBrace, "{", m_line, m_column};
            break;
        case '}':
            token = {TokenType::RBrace, "}", m_line, m_column};
            break;
        case ',':
            token = {TokenType::Comma, ",", m_line, m_column};
            break;
        case ';':
            token = {TokenType::Semicolon, ";", m_line, m_column};
            break;
        case '"':
            return ReadString();
        case 0:
            token = {TokenType::Eof, "", m_line, m_column};
            break;
        default:
            if (IsLetter(m_char))
            {
                std::string literal = ReadIdentifier();
                token.type = LookupIdentifier(literal);
                token.literal = literal;
                return token;
            }
            else if (IsDigit(m_char))
            {
                token.literal = ReadNumber();
                token.type = TokenType::Int; // For now, assume all numbers are ints
                return token;
            }
            else
            {
                token = {TokenType::Illegal, std::string(1, m_char), m_line, m_column};
            }
        }

        ReadChar();
        return token;
    }

    void Lexer::SkipWhitespace()
    {
        while (m_char == ' ' || m_char == '\t' || m_char == '\n' || m_char == '\r')
        {
            ReadChar();
        }
    }

    char Lexer::PeekChar()
    {
        if (m_readPosition >= m_input.length())
        {
            return 0;
        }
        return m_input[m_readPosition];
    }

    char Lexer::ReadChar()
    {
        if (m_readPosition >= m_input.length())
        {
            m_char = 0;
        }
        else
        {
            m_char = m_input[m_readPosition];
        }
        m_position = m_readPosition;
        m_readPosition++;
        if (m_char == '\n')
        {
            m_line++;
            m_column = 0;
        }
        else
        {
            m_column++;
        }
        return m_char;
    }

    std::string Lexer::ReadIdentifier()
    {
        int startPosition = m_position;
        while (IsLetter(m_char) || IsDigit(m_char))
        {
            ReadChar();
        }
        return m_input.substr(startPosition, m_position - startPosition);
    }

    std::string Lexer::ReadNumber()
    {
        int startPosition = m_position;
        while (IsDigit(m_char))
        {
            ReadChar();
        }
        return m_input.substr(startPosition, m_position - startPosition);
    }

    Token Lexer::ReadString()
    {
        int startPosition = m_position + 1;
        do
        {
            ReadChar();
        } while (m_char != '"' && m_char != 0);
        std::string literal = m_input.substr(startPosition, m_position - startPosition);
        ReadChar(); // consume the closing quote
        return {TokenType::String, literal, m_line, m_column - (int)literal.length() - 2};
    }

    bool Lexer::IsLetter(char ch)
    {
        return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == '_';
    }

    bool Lexer::IsDigit(char ch)
    {
        return '0' <= ch && ch <= '9';
    }

} // namespace Chtholly
