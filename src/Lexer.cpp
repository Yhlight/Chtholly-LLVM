#include "Lexer.hpp"
#include <iostream>
#include <utility>

const std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"let", TokenType::LET},
    {"mut", TokenType::MUT},
    {"func", TokenType::FUNC},
    {"return", TokenType::RETURN},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"while", TokenType::WHILE},
    {"for", TokenType::FOR},
    {"struct", TokenType::STRUCT},
    {"enum", TokenType::ENUM},
    {"public", TokenType::PUBLIC},
    {"private", TokenType::PRIVATE},
    {"impl", TokenType::IMPL},
    {"import", TokenType::IMPORT},
    {"switch", TokenType::SWITCH},
    {"case", TokenType::CASE},
    {"fallthrough", TokenType::FALLTHROUGH},
    {"break", TokenType::BREAK},
    {"continue", TokenType::CONTINUE},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE},
    {"none", TokenType::NONE},
    {"self", TokenType::SELF}
};

Lexer::Lexer(const std::string& source) : source(source) {}

std::vector<Token> Lexer::tokenize() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }

    tokens.emplace_back(TokenType::END_OF_FILE, "", line);
    return std::move(tokens);
}

void Lexer::scanToken() {
    char c = advance();
    switch (c) {
        // Single-character tokens
        case '(': tokens.emplace_back(TokenType::LPAREN, "(", line); break;
        case ')': tokens.emplace_back(TokenType::RPAREN, ")", line); break;
        case '{': tokens.emplace_back(TokenType::LBRACE, "{", line); break;
        case '}': tokens.emplace_back(TokenType::RBRACE, "}", line); break;
        case '[': tokens.emplace_back(TokenType::LBRACKET, "[", line); break;
        case ']': tokens.emplace_back(TokenType::RBRACKET, "]", line); break;
        case ',': tokens.emplace_back(TokenType::COMMA, ",", line); break;
        case '.': tokens.emplace_back(TokenType::DOT, ".", line); break;
        case ';': tokens.emplace_back(TokenType::SEMICOLON, ";", line); break;
        case '?': tokens.emplace_back(TokenType::QUESTION, "?", line); break;
        case '~': tokens.emplace_back(TokenType::BITWISE_NOT, "~", line); break;
        case '^': tokens.emplace_back(TokenType::BITWISE_XOR, "^", line); break;

        // One or two character tokens
        case '-':
            if (match('>')) tokens.emplace_back(TokenType::ARROW, "->", line);
            else if (match('=')) tokens.emplace_back(TokenType::MINUS_ASSIGN, "-=", line);
            else tokens.emplace_back(TokenType::MINUS, "-", line);
            break;
        case '+':
            if (match('=')) tokens.emplace_back(TokenType::PLUS_ASSIGN, "+=", line);
            else tokens.emplace_back(TokenType::PLUS, "+", line);
            break;
        case '*':
            if (match('=')) tokens.emplace_back(TokenType::STAR_ASSIGN, "*=", line);
            else tokens.emplace_back(TokenType::STAR, "*", line);
            break;
        case '%':
            if (match('=')) tokens.emplace_back(TokenType::PERCENT_ASSIGN, "%=", line);
            else tokens.emplace_back(TokenType::PERCENT, "%", line);
            break;
        case '!':
            if (match('=')) tokens.emplace_back(TokenType::NOT_EQUAL, "!=", line);
            else tokens.emplace_back(TokenType::LOGICAL_NOT, "!", line);
            break;
        case '=':
            if (match('=')) tokens.emplace_back(TokenType::EQUAL, "==", line);
            else tokens.emplace_back(TokenType::ASSIGN, "=", line);
            break;
        case '<':
            if (match('=')) tokens.emplace_back(TokenType::LESS_EQUAL, "<=", line);
            else if (match('<')) tokens.emplace_back(TokenType::LEFT_SHIFT, "<<", line);
            else tokens.emplace_back(TokenType::LESS, "<", line);
            break;
        case '>':
            if (match('=')) tokens.emplace_back(TokenType::GREATER_EQUAL, ">=", line);
            else if (match('>')) tokens.emplace_back(TokenType::RIGHT_SHIFT, ">>", line);
            else tokens.emplace_back(TokenType::GREATER, ">", line);
            break;
        case '&':
            if (match('&')) tokens.emplace_back(TokenType::LOGICAL_AND, "&&", line);
            else tokens.emplace_back(TokenType::BITWISE_AND, "&", line);
            break;
        case '|':
            if (match('|')) tokens.emplace_back(TokenType::LOGICAL_OR, "||", line);
            else tokens.emplace_back(TokenType::BITWISE_OR, "|", line);
            break;
        case ':':
            if (match(':')) tokens.emplace_back(TokenType::COLON_COLON, "::", line);
            else tokens.emplace_back(TokenType::COLON, ":", line);
            break;

        case '/':
            if (match('/')) { // Single line comment
                while (peek() != '\n' && !isAtEnd()) advance();
            } else if (match('*')) { // Block comment
                blockComment();
            } else if (match('=')) {
                tokens.emplace_back(TokenType::SLASH_ASSIGN, "/=", line);
            }
            else {
                tokens.emplace_back(TokenType::SLASH, "/", line);
            }
            break;

        // Whitespace
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            line++;
            break;

        // Literals
        case '"': string(); break;
        case '\'':
            // Chtholly uses single quotes for characters
            if (peekNext() == '\'' && !isAtEnd()) {
                char value = advance();
                advance(); // consume closing '
                tokens.emplace_back(TokenType::CHAR, std::string(1, value), line);
            } else {
                 tokens.emplace_back(TokenType::UNKNOWN, std::string(1, c), line);
            }
            break;

        default:
            if (isDigit(c)) {
                number();
            } else if (isAlpha(c)) {
                identifier();
            } else {
                tokens.emplace_back(TokenType::UNKNOWN, std::string(1, c), line);
            }
            break;
    }
}

void Lexer::blockComment() {
    while (!isAtEnd()) {
        if (peek() == '*' && peekNext() == '/') {
            advance(); // consume *
            advance(); // consume /
            return;
        }
        if (peek() == '\n') line++;
        advance();
    }
}

void Lexer::identifier() {
    while (isAlphaNumeric(peek())) advance();

    std::string text = source.substr(start, current - start);
    TokenType type;
    auto it = keywords.find(text);
    if (it == keywords.end()) {
        type = TokenType::IDENTIFIER;
    } else {
        type = it->second;
    }
    tokens.emplace_back(type, text, line);
}

void Lexer::number() {
    bool isDouble = false;
    while (isDigit(peek())) advance();

    if (peek() == '.' && isDigit(peekNext())) {
        isDouble = true;
        advance(); // consume the "."
        while (isDigit(peek())) advance();
    }

    std::string value = source.substr(start, current - start);
    tokens.emplace_back(isDouble ? TokenType::DOUBLE : TokenType::INTEGER, value, line);
}

void Lexer::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }

    if (isAtEnd()) {
        // Unterminated string.
        tokens.emplace_back(TokenType::UNKNOWN, "Unterminated string.", line);
        return;
    }

    advance(); // The closing ".

    // Trim the surrounding quotes.
    std::string value = source.substr(start + 1, current - start - 2);
    tokens.emplace_back(TokenType::STRING, value, line);
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;

    current++;
    return true;
}

char Lexer::peek() {
    if (isAtEnd()) return '\0';
    return source[current];
}

char Lexer::peekNext() {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}

char Lexer::advance() {
    return source[current++];
}

bool Lexer::isAtEnd() {
    return current >= source.length();
}

bool Lexer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

bool Lexer::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}
