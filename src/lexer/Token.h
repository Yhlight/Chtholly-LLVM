#ifndef CHTHOLLY_TOKEN_H
#define CHTHOLLY_TOKEN_H

#include <string>
#include <unordered_map>

namespace Chtholly
{
    enum class TokenType
    {
        // Special Tokens
        Eof,
        Illegal,

        // Literals
        Identifier,
        Int,
        Double,
        Char,
        String,

        // Operators
        Assign,
        Plus,
        Minus,
        Bang,
        Asterisk,
        Slash,
        Percent,

        Equal,
        NotEqual,
        LessThan,
        GreaterThan,
        LessThanEqual,
        GreaterThanEqual,

        // Delimiters
        Comma,
        Semicolon,
        Colon,
        LParen,
        RParen,
        LBrace,
        RBrace,
        LBracket,
        RBracket,

        // Keywords
        Function,
        Let,
        Mut,
        Return,
        True,
        False,
        If,
        Else,
        While,
        For,
        Struct,
        Enum,
        Import,
        Public,
        Private,
        None
    };

    struct Token
    {
        TokenType type;
        std::string literal;
        int line = 0;
        int column = 0;
    };

    TokenType LookupIdentifier(const std::string& identifier);

} // namespace Chtholly

#endif // CHTHOLLY_TOKEN_H
