#include "Lexer.hpp"
#include <unordered_map>

namespace chtholly {

static const std::unordered_map<std::string, TokenType> keywords = {
    {"let", TokenType::LET},
    {"mut", TokenType::MUT},
    {"fn", TokenType::FN},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"while", TokenType::WHILE},
    {"for", TokenType::FOR},
    {"return", TokenType::RETURN},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE},
    {"class", TokenType::CLASS},
    {"this", TokenType::THIS},
    {"static", TokenType::STATIC},
    {"import", TokenType::IMPORT},
    {"as", TokenType::AS},
    {"package", TokenType::PACKAGE},
    {"enum", TokenType::ENUM},
    {"switch", TokenType::SWITCH},
    {"case", TokenType::CASE},
    {"break", TokenType::BREAK},
    {"fallthrough", TokenType::FALLTHROUGH},
    {"do", TokenType::DO},
    {"private", TokenType::PRIVATE},
    {"public", TokenType::PUBLIC},
    {"type_cast", TokenType::TYPE_CAST},
    {"int", TokenType::INT},
    {"i8", TokenType::I8},
    {"i16", TokenType::I16},
    {"i32", TokenType::I32},
    {"i64", TokenType::I64},
    {"u8", TokenType::U8},
    {"u16", TokenType::U16},
    {"u32", TokenType::U32},
    {"u64", TokenType::U64},
    {"char", TokenType::CHAR},
    {"double", TokenType::DOUBLE},
    {"float", TokenType::FLOAT},
    {"long", TokenType::LONG_DOUBLE},
    {"void", TokenType::VOID},
    {"bool", TokenType::BOOL}
};

Lexer::Lexer(const std::string& source) : source(source) {}

std::vector<Token> Lexer::scan_tokens() {
    while (current < source.length()) {
        start = current;
        scan_token();
    }

    tokens.emplace_back(TokenType::EOS, "", nullptr, line);
    return tokens;
}

void Lexer::scan_token() {
    char c = advance();
    switch (c) {
        case '(': add_token(TokenType::LEFT_PAREN); break;
        case ')': add_token(TokenType::RIGHT_PAREN); break;
        case '{': add_token(TokenType::LEFT_BRACE); break;
        case '}': add_token(TokenType::RIGHT_BRACE); break;
        case '[': add_token(TokenType::LEFT_BRACKET); break;
        case ']': add_token(TokenType::RIGHT_BRACKET); break;
        case ',': add_token(TokenType::COMMA); break;
        case '.': add_token(TokenType::DOT); break;
        case '-': add_token(match('=') ? TokenType::MINUS_EQUAL : (match('-') ? TokenType::MINUS_MINUS : TokenType::MINUS)); break;
        case '+': add_token(match('=') ? TokenType::PLUS_EQUAL : (match('+') ? TokenType::PLUS_PLUS : TokenType::PLUS)); break;
        case ';': add_token(TokenType::SEMICOLON); break;
        case '*': add_token(match('=') ? TokenType::STAR_EQUAL : TokenType::STAR); break;
        case '%': add_token(match('=') ? TokenType::PERCENT_EQUAL : TokenType::PERCENT); break;
        case ':': add_token(TokenType::COLON); break;
        case '!': add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
        case '=': add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
        case '<': add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
        case '>': add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
        case '&': add_token(match('&') ? TokenType::AMPERSAND_AMPERSAND : TokenType::AMPERSAND); break;
        case '|': add_token(match('|') ? TokenType::PIPE_PIPE : TokenType::PIPE); break;
        case '/':
            if (match('/')) {
                // A comment goes until the end of the line.
                while (peek() != '\n' && current < source.length()) advance();
            } else if (match('*')) {
                // A block comment goes until */ is found.
                while (peek() != '*' || peek_next() != '/') {
                    if (peek() == '\n') line++;
                    advance();
                }
                advance(); // consume the '*'
                advance(); // consume the '/'
            }
            else {
                add_token(match('=') ? TokenType::SLASH_EQUAL : TokenType::SLASH);
            }
            break;

        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace.
            break;

        case '\n':
            line++;
            break;

        case '"': string(); break;
        case '\'':
            // char literal
            if (peek() != '\'' && peek_next() == '\'') {
                char value = advance();
                advance(); // consume the closing '
                add_token(TokenType::CHARACTER, value);
            } else {
                // error
            }
            break;

        default:
            if (isdigit(c)) {
                number();
            } else if (isalpha(c) || c == '_') {
                identifier();
            } else {
                // error
            }
            break;
    }
}

char Lexer::advance() {
    return source[current++];
}

void Lexer::add_token(TokenType type, const std::any& literal) {
    std::string text = source.substr(start, current - start);
    tokens.emplace_back(type, text, literal, line);
}

bool Lexer::match(char expected) {
    if (current >= source.length()) return false;
    if (source[current] != expected) return false;

    current++;
    return true;
}

char Lexer::peek() const {
    if (current >= source.length()) return '\0';
    return source[current];
}

char Lexer::peek_next() const {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}

void Lexer::string() {
    while (peek() != '"' && current < source.length()) {
        if (peek() == '\n') line++;
        advance();
    }

    if (current >= source.length()) {
        // Unterminated string.
        return;
    }

    // The closing ".
    advance();

    // Trim the surrounding quotes.
    std::string value = source.substr(start + 1, current - start - 2);
    add_token(TokenType::STRING, value);
}

void Lexer::number() {
    while (isdigit(peek())) advance();

    // Look for a fractional part.
    if (peek() == '.' && isdigit(peek_next())) {
        // Consume the "."
        advance();

        while (isdigit(peek())) advance();
    }

    add_token(TokenType::NUMBER, std::stod(source.substr(start, current - start)));
}

void Lexer::identifier() {
    while (isalnum(peek()) || peek() == '_') advance();

    std::string text = source.substr(start, current - start);
    TokenType type = TokenType::IDENTIFIER;
    if (keywords.count(text)) {
        type = keywords.at(text);
    }
    add_token(type);
}

} // namespace chtholly
