#include "Token.h"

namespace Chtholly
{
    static std::unordered_map<std::string, TokenType> keywords = {
        {"func", TokenType::Function},
        {"let", TokenType::Let},
        {"mut", TokenType::Mut},
        {"return", TokenType::Return},
        {"true", TokenType::True},
        {"false", TokenType::False},
        {"if", TokenType::If},
        {"else", TokenType::Else},
        {"while", TokenType::While},
        {"for", TokenType::For},
        {"struct", TokenType::Struct},
        {"enum", TokenType::Enum},
        {"import", TokenType::Import},
        {"public", TokenType::Public},
        {"private", TokenType::Private},
        {"none", TokenType::None},
    };

    TokenType LookupIdentifier(const std::string& identifier)
    {
        if (keywords.count(identifier))
        {
            return keywords.at(identifier);
        }
        return TokenType::Identifier;
    }

} // namespace Chtholly
