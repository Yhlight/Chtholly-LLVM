#include "Lexer.h"
#include <unordered_map>

namespace Chtholly {

static std::unordered_map<std::string, TokenType> keywords;

static void initializeKeywords() {
    keywords["let"] = TokenType::Let;
    keywords["mut"] = TokenType::Mut;
    keywords["func"] = TokenType::Func;
    keywords["return"] = TokenType::Return;
    keywords["if"] = TokenType::If;
    keywords["else"] = TokenType::Else;
    keywords["while"] = TokenType::While;
    keywords["for"] = TokenType::For;
    keywords["break"] = TokenType::Break;
    keywords["continue"] = TokenType::Continue;
    keywords["struct"] = TokenType::Struct;
    keywords["enum"] = TokenType::Enum;
    keywords["import"] = TokenType::Import;
    keywords["public"] = TokenType::Public;
    keywords["private"] = TokenType::Private;
    keywords["self"] = TokenType::Self;
    keywords["switch"] = TokenType::Switch;
    keywords["case"] = TokenType::Case;
    keywords["fallthrough"] = TokenType::Fallthrough;
    keywords["impl"] = TokenType::Impl;
    keywords["trait"] = TokenType::Trait;
    keywords["type_cast"] = TokenType::TypeCast;
    keywords["int"] = TokenType::Int;
    keywords["int8"] = TokenType::Int8;
    keywords["int16"] = TokenType::Int16;
    keywords["int32"] = TokenType::Int32;
    keywords["int64"] = TokenType::Int64;
    keywords["uint8"] = TokenType::Uint8;
    keywords["uint16"] = TokenType::Uint16;
    keywords["uint32"] = TokenType::Uint32;
    keywords["uint64"] = TokenType::Uint64;
    keywords["char"] = TokenType::Char;
    keywords["double"] = TokenType::Double;
    keywords["float"] = TokenType::Float;
    keywords["long"] = TokenType::LongDouble;
    keywords["void"] = TokenType::Void;
    keywords["bool"] = TokenType::Bool;
    keywords["string"] = TokenType::String;
    keywords["array"] = TokenType::Array;
    keywords["function"] = TokenType::Function;
    keywords["option"] = TokenType::Option;
    keywords["none"] = TokenType::None;
}

Lexer::Lexer(const std::string& source) : source(source) {
    initializeKeywords();
}

std::vector<Token> Lexer::scanTokens() {
    while (current < source.length()) {
        start = current;
        scanToken();
    }
    tokens.push_back({TokenType::Eof, "", std::monostate{}, line, column});
    return tokens;
}

Token Lexer::scanToken() {
    char c = advance();
    switch (c) {
        // Single-character tokens
        case '(': tokens.push_back({TokenType::LeftParen, "(", std::monostate{}, line, column}); break;
        case ')': tokens.push_back({TokenType::RightParen, ")", std::monostate{}, line, column}); break;
        case '{': tokens.push_back({TokenType::LeftBrace, "{", std::monostate{}, line, column}); break;
        case '}': tokens.push_back({TokenType::RightBrace, "}", std::monostate{}, line, column}); break;
        case '[': tokens.push_back({TokenType::LeftBracket, "[", std::monostate{}, line, column}); break;
        case ']': tokens.push_back({TokenType::RightBracket, "]", std::monostate{}, line, column}); break;
        case ',': tokens.push_back({TokenType::Comma, ",", std::monostate{}, line, column}); break;
        case '.': tokens.push_back({TokenType::Dot, ".", std::monostate{}, line, column}); break;
        case ';': tokens.push_back({TokenType::Semicolon, ";", std::monostate{}, line, column}); break;
        case '*': tokens.push_back({TokenType::Asterisk, "*", std::monostate{}, line, column}); break;
        case '%': tokens.push_back({TokenType::Percent, "%", std::monostate{}, line, column}); break;
        case '^': tokens.push_back({TokenType::Caret, "^", std::monostate{}, line, column}); break;
        case '~': tokens.push_back({TokenType::Tilde, "~", std::monostate{}, line, column}); break;

        // One or two character tokens
        case '!':
            tokens.push_back({match('=') ? TokenType::BangEqual : TokenType::Bang,
                              source.substr(start, current - start), std::monostate{}, line, column});
            break;
        case '=':
            tokens.push_back({match('=') ? TokenType::EqualEqual : TokenType::Equal,
                              source.substr(start, current - start), std::monostate{}, line, column});
            break;
        case '<':
            if (match('=')) {
                tokens.push_back({TokenType::LessEqual, "<=", std::monostate{}, line, column});
            } else if (match('<')) {
                tokens.push_back({TokenType::LessLess, "<<", std::monostate{}, line, column});
            } else {
                tokens.push_back({TokenType::Less, "<", std::monostate{}, line, column});
            }
            break;
        case '>':
            if (match('=')) {
                tokens.push_back({TokenType::GreaterEqual, ">=", std::monostate{}, line, column});
            } else if (match('>')) {
                tokens.push_back({TokenType::GreaterGreater, ">>", std::monostate{}, line, column});
            } else {
                tokens.push_back({TokenType::Greater, ">", std::monostate{}, line, column});
            }
            break;
        case '+':
            tokens.push_back({match('=') ? TokenType::PlusEqual : TokenType::Plus,
                              source.substr(start, current - start), std::monostate{}, line, column});
            break;
        case '-':
            if (match('=')) {
                tokens.push_back({TokenType::MinusEqual, "-=", std::monostate{}, line, column});
            } else if (match('>')) {
                tokens.push_back({TokenType::Arrow, "->", std::monostate{}, line, column});
            } else {
                tokens.push_back({TokenType::Minus, "-", std::monostate{}, line, column});
            }
            break;
        case ':':
            tokens.push_back({match(':') ? TokenType::ColonColon : TokenType::Colon,
                              source.substr(start, current - start), std::monostate{}, line, column});
            break;

        // Comments, slash, and block comments
        case '/':
            if (match('/')) {
                lineComment();
            } else if (match('*')) {
                blockComment();
            } else {
                tokens.push_back({match('=') ? TokenType::SlashEqual : TokenType::Slash,
                                  source.substr(start, current - start), std::monostate{}, line, column});
            }
            break;

        // Whitespace
        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace
            break;

        case '\n':
            line++;
            column = 1;
            break;

        case '"': string(); break;
        case '\'':
            // For single character literals
            if (peek() != '\'' && peekNext() == '\'') {
                char value = advance();
                advance(); // consume closing quote
                tokens.push_back({TokenType::CharLiteral, source.substr(start, current - start), value, line, column});
            }
            break;

        default:
            if (isdigit(c)) {
                number();
            } else if (isalpha(c) || c == '_') {
                identifier();
            } else {
                // For now, we will add error handling in the parser
            }
            break;
    }

    return Token();
}

char Lexer::advance() {
    column++;
    return source[current++];
}

bool Lexer::match(char expected) {
    if (current >= source.length()) return false;
    if (source[current] != expected) return false;
    current++;
    column++;
    return true;
}

char Lexer::peek() {
    if (current >= source.length()) return '\0';
    return source[current];
}

char Lexer::peekNext() {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}

void Lexer::lineComment() {
    while (peek() != '\n' && current < source.length()) {
        advance();
    }
}

void Lexer::blockComment() {
    while (!(peek() == '*' && peekNext() == '/') && current < source.length()) {
        if (peek() == '\n') {
            line++;
            column = 1;
        }
        advance();
    }
    if (current < source.length()) {
        advance(); // consume '*'
        advance(); // consume '/'
    }
}

void Lexer::string() {
    while (peek() != '"' && current < source.length()) {
        if (peek() == '\n') {
            line++;
            column = 1;
        }
        advance();
    }

    if (current >= source.length()) {
        // Unterminated string.
        return;
    }

    advance(); // The closing ".

    // Trim the surrounding quotes.
    std::string value = source.substr(start + 1, current - start - 2);
    tokens.push_back({TokenType::StringLiteral, source.substr(start, current - start), value, line, column});
}

void Lexer::number() {
    while (isdigit(peek())) advance();

    // Look for a fractional part.
    if (peek() == '.' && isdigit(peekNext())) {
        // Consume the ".".
        advance();

        while (isdigit(peek())) advance();
    }

    std::string lexeme = source.substr(start, current - start);
    if (lexeme.find('.') != std::string::npos) {
        tokens.push_back({TokenType::DoubleLiteral, lexeme, std::stod(lexeme), line, column});
    } else {
        tokens.push_back({TokenType::IntegerLiteral, lexeme, std::stoll(lexeme), line, column});
    }
}

void Lexer::identifier() {
    while (isalnum(peek()) || peek() == '_') advance();

    std::string text = source.substr(start, current - start);
    TokenType type = keywords.count(text) ? keywords.at(text) : TokenType::Identifier;
    tokens.push_back({type, text, std::monostate{}, line, column});
}

} // namespace Chtholly
