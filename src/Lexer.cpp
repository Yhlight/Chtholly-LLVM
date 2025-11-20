#include "Lexer.hpp"
#include <iostream>

const std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"fn", TokenType::FN},
    {"let", TokenType::LET},
    {"mut", TokenType::MUT},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"while", TokenType::WHILE},
    {"for", TokenType::FOR},
    {"return", TokenType::RETURN},
    {"class", TokenType::CLASS},
    {"private", TokenType::PRIVATE},
    {"public", TokenType::PUBLIC},
    {"static", TokenType::STATIC},
    {"this", TokenType::THIS},
    {"switch", TokenType::SWITCH},
    {"case", TokenType::CASE},
    {"break", TokenType::BREAK},
    {"fallthrough", TokenType::FALLTHROUGH},
    {"do", TokenType::DO},
    {"enum", TokenType::ENUM},
    {"import", TokenType::IMPORT},
    {"as", TokenType::AS},
    {"package", TokenType::PACKAGE},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE}
};

Lexer::Lexer(const std::string& source) : source(source) {}

std::vector<Token> Lexer::scan_tokens() {
    while (current < source.length()) {
        start = current;
        scan_token();
    }
    tokens.push_back({TokenType::END_OF_FILE, "", line});
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
        case ';': add_token(TokenType::SEMICOLON); break;
        case ':': add_token(TokenType::COLON); break;
        case '+':
            if (match('=')) add_token(TokenType::PLUS_EQUAL);
            else if (match('+')) add_token(TokenType::PLUS_PLUS);
            else add_token(TokenType::PLUS);
            break;
        case '-':
            if (match('=')) add_token(TokenType::MINUS_EQUAL);
            else if (match('-')) add_token(TokenType::MINUS_MINUS);
            else if (match('>')) add_token(TokenType::ARROW);
            else add_token(TokenType::MINUS);
            break;
        case '*':
            if (match('=')) add_token(TokenType::STAR_EQUAL);
            else add_token(TokenType::STAR);
            break;
        case '/':
            if (match('=')) add_token(TokenType::SLASH_EQUAL);
            else if (match('/')) {
                while (peek() != '\n' && current < source.length()) advance();
            } else if (match('*')) {
                while (peek() != '*' || peek_next() != '/') {
                    if (current >= source.length()) {
                        std::cerr << "Unterminated multi-line comment at line " << line << std::endl;
                        return;
                    }
                    if (peek() == '\n') line++;
                    advance();
                }
                advance(); // Consume the '*'
                advance(); // Consume the '/'
            } else {
                add_token(TokenType::SLASH);
            }
            break;
        case '%':
            if (match('=')) add_token(TokenType::PERCENT_EQUAL);
            else add_token(TokenType::PERCENT);
            break;
        case '!': add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
        case '=': add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
        case '<': add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
        case '>': add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
        case '&': add_token(match('&') ? TokenType::AMPERSAND_AMPERSAND : TokenType::AMPERSAND); break;
        case '|': add_token(match('|') ? TokenType::PIPE_PIPE : TokenType::PIPE); break;
        case '^': add_token(TokenType::CARET); break;
        case '~': add_token(TokenType::TILDE); break;
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            line++;
            break;
        case '"': string(); break;
        case '\'':
            advance();
            add_token(TokenType::CHAR);
            if (peek() == '\'') advance();
            break;
        default:
            if (isdigit(c)) {
                number();
            } else if (isalpha(c) || c == '_') {
                identifier();
            } else {
                std::cerr << "Unexpected character at line " << line << std::endl;
            }
            break;
    }
}

char Lexer::advance() {
    return source[current++];
}

void Lexer::add_token(TokenType type) {
    std::string text = source.substr(start, current - start);
    tokens.push_back({type, text, line});
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

char Lexer::peek_next() {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}

void Lexer::string() {
    while (peek() != '"' && current < source.length()) {
        if (peek() == '\n') line++;
        advance();
    }
    if (current >= source.length()) {
        std::cerr << "Unterminated string at line " << line << std::endl;
        return;
    }
    advance();
    add_token(TokenType::STRING);
}

void Lexer::number() {
    while (isdigit(peek())) advance();
    if (peek() == '.' && isdigit(peek_next())) {
        advance();
        while (isdigit(peek())) advance();
        add_token(TokenType::DOUBLE);
    } else {
        add_token(TokenType::INTEGER);
    }
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
