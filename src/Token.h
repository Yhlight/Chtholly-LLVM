#ifndef CHTHOLLY_TOKEN_H
#define CHTHOLLY_TOKEN_H

#include <string>
#include <variant>

namespace Chtholly {

enum class TokenType {
    // Keywords
    Let, Mut, Func, Return, If, Else, While, For, Break, Continue, Struct, Enum,
    Import, Public, Private, Self, Switch, Case, Fallthrough, Impl, Trait,
    TypeCast,

    // Built-in Types
    Int, Int8, Int16, Int32, Int64, Uint8, Uint16, Uint32, Uint64, Char,
    Double, Float, LongDouble, Void, Bool, String, Array, Function, Option,
    None,

    // Operators
    Plus, Minus, Asterisk, Slash, Percent,
    EqualEqual, BangEqual, Greater, Less, GreaterEqual, LessEqual,
    AmpersandAmpersand, PipePipe, Bang,
    Ampersand, Pipe, Caret, Tilde, LessLess, GreaterGreater,
    Equal, PlusEqual, MinusEqual, AsteriskEqual, SlashEqual, PercentEqual,
    Arrow, ColonColon, Colon, Semicolon, Comma, Dot,
    LeftParen, RightParen, LeftBrace, RightBrace, LeftBracket, RightBracket,

    // Literals
    IntegerLiteral, DoubleLiteral, CharLiteral, StringLiteral,

    // Identifier
    Identifier,

    // Comments
    LineComment, BlockComment,

    // End of File
    Eof
};

struct Token {
    TokenType type;
    std::string lexeme;
    std::variant<std::monostate, int64_t, double, char, std::string> literal;
    int line = 0;
    int column = 0;
};

} // namespace Chtholly

#endif // CHTHOLLY_TOKEN_H
