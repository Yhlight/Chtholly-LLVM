#include "Lexer.hpp"
#include "Chtholly.hpp"
#include <iostream>
#include <map>

namespace chtholly {

static std::map<std::string, TokenType> keywords = {
    {"let", TokenType::LET},
    {"mut", TokenType::MUT},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"switch", TokenType::SWITCH},
    {"case", TokenType::CASE},
    {"fallthrough", TokenType::FALLTHROUGH},
    {"while", TokenType::WHILE},
    {"for", TokenType::FOR},
    {"do", TokenType::DO},
    {"break", TokenType::BREAK},
    {"fn", TokenType::FN},
    {"return", TokenType::RETURN},
    {"class", TokenType::CLASS},
    {"this", TokenType::THIS},
    {"static", TokenType::STATIC},
    {"public", TokenType::PUBLIC},
    {"private", TokenType::PRIVATE},
    {"enum", TokenType::ENUM},
    {"import", TokenType::IMPORT},
    {"as", TokenType::AS},
    {"package", TokenType::PACKAGE},
    {"type_cast", TokenType::TYPE_CAST},
};

Lexer::Lexer(const std::string& source) : source(source) {}

std::vector<Token> Lexer::scanTokens() {
    while (current < source.length()) {
        start = current;
        scanToken();
    }

    tokens.push_back({TokenType::END_OF_FILE, "", nullptr, line});
    return tokens;
}

void Lexer::scanToken() {
    char c = advance();
    switch (c) {
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case '[': addToken(TokenType::LEFT_BRACKET); break;
        case ']': addToken(TokenType::RIGHT_BRACKET); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '-': addToken(match('=') ? TokenType::MINUS_EQUAL : TokenType::MINUS); break;
        case '+': addToken(match('=') ? TokenType::PLUS_EQUAL : TokenType::PLUS); break;
        case '*': addToken(match('=') ? TokenType::STAR_EQUAL : TokenType::STAR); break;
        case '%': addToken(match('=') ? TokenType::PERCENT_EQUAL : TokenType::PERCENT); break;
        case ':': addToken(TokenType::COLON); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '~': addToken(TokenType::TILDE); break;
        case '!': addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
        case '=': addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
        case '<': addToken(match('=') ? TokenType::LESS_EQUAL : (match('<') ? TokenType::LESS_LESS : TokenType::LESS)); break;
        case '>': addToken(match('=') ? TokenType::GREATER_EQUAL : (match('>') ? TokenType::GREATER_GREATER : TokenType::GREATER)); break;
        case '&': addToken(match('&') ? TokenType::AMPERSAND_AMPERSAND : TokenType::AMPERSAND); break;
        case '|': addToken(match('|') ? TokenType::PIPE_PIPE : TokenType::PIPE); break;
        case '^': addToken(TokenType::CARET); break;
        case '/':
            if (match('/')) {
                while (peek() != '\n' && current < source.length()) advance();
            } else if (match('*')) {
                while (peek() != '*' && peekNext() != '/' && current < source.length()) {
                    if (peek() == '\n') line++;
                    advance();
                }
                if (current < source.length()) {
                    advance(); // Consume the '*'
                    advance(); // Consume the '/'
                } else {
                    Chtholly::error(line, "Unterminated multi-line comment.");
                }
            } else {
                addToken(match('=') ? TokenType::SLASH_EQUAL : TokenType::SLASH);
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
        default:
            if (isdigit(c)) {
                number();
            } else if (isalpha(c) || c == '_') {
                identifier();
            } else {
                Chtholly::error(line, "Unexpected character.");
            }
            break;
    }
}

char Lexer::advance() {
    return source[current++];
}

void Lexer::addToken(TokenType type) {
    addToken(type, nullptr);
}

void Lexer::addToken(TokenType type, const std::any& literal) {
    std::string text = source.substr(start, current - start);
    tokens.push_back({type, text, literal, line});
}

bool Lexer::match(char expected) {
    if (current >= source.length()) return false;
    if (source[current] != expected) return false;
    current++;
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

void Lexer::string() {
    while (peek() != '"' && current < source.length()) {
        if (peek() == '\n') line++;
        advance();
    }

    if (current >= source.length()) {
        Chtholly::error(line, "Unterminated string.");
        return;
    }

    advance(); // The closing ".

    std::string value = source.substr(start + 1, current - start - 2);
    addToken(TokenType::STRING, value);
}

void Lexer::number() {
    bool isFloat = false;
    while (isdigit(peek())) advance();

    if (peek() == '.' && isdigit(peekNext())) {
        isFloat = true;
        advance();
        while (isdigit(peek())) advance();
    }

    std::string numStr = source.substr(start, current - start);
    if (isFloat) {
        addToken(TokenType::NUMBER, std::stod(numStr));
    } else {
        addToken(TokenType::NUMBER, std::stoi(numStr));
    }
}

void Lexer::identifier() {
    while (isalnum(peek()) || peek() == '_') advance();

    std::string text = source.substr(start, current - start);
    TokenType type = TokenType::IDENTIFIER;
    if (keywords.count(text)) {
        type = keywords[text];
    }
    addToken(type);
}

} // namespace chtholly
