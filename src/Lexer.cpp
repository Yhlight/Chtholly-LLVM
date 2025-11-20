#include "Lexer.hpp"
#include <unordered_map>

static std::unordered_map<std::string, TokenType> keywords = {
    {"let", TokenType::LET},
    {"mut", TokenType::MUT},
    {"fn", TokenType::FN},
    {"class", TokenType::CLASS},
    {"public", TokenType::PUBLIC},
    {"private", TokenType::PRIVATE},
    {"static", TokenType::STATIC},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"switch", TokenType::SWITCH},
    {"case", TokenType::CASE},
    {"break", TokenType::BREAK},
    {"fallthrough", TokenType::FALLTHROUGH},
    {"while", TokenType::WHILE},
    {"for", TokenType::FOR},
    {"do", TokenType::DO},
    {"return", TokenType::RETURN},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE},
    {"this", TokenType::THIS},
    {"enum", TokenType::ENUM},
    {"import", TokenType::IMPORT},
    {"as", TokenType::AS},
    {"package", TokenType::PACKAGE}
};

Lexer::Lexer(const std::string& source) : source(source) {}

std::vector<Token> Lexer::scanTokens() {
    while (current < source.length()) {
        start = current;
        scanToken();
    }

    tokens.push_back({TokenType::END_OF_FILE, "", line});
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
        case '-':
            if (match('=')) {
                addToken(TokenType::MINUS_EQUAL);
            } else if (match('-')) {
                addToken(TokenType::MINUS_MINUS);
            } else if (match('>')) {
                addToken(TokenType::ARROW);
            } else {
                addToken(TokenType::MINUS);
            }
            break;
        case '+':
            if (match('=')) {
                addToken(TokenType::PLUS_EQUAL);
            } else if (match('+')) {
                addToken(TokenType::PLUS_PLUS);
            } else {
                addToken(TokenType::PLUS);
            }
            break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '*':
            if (match('=')) {
                addToken(TokenType::STAR_EQUAL);
            } else {
                addToken(TokenType::STAR);
            }
            break;
        case '%':
             if (match('=')) {
                addToken(TokenType::PERCENT_EQUAL);
            } else {
                addToken(TokenType::PERCENT);
            }
            break;
        case '~': addToken(TokenType::TILDE); break;
        case '!':
            addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
             if (match('=')) {
                addToken(TokenType::LESS_EQUAL);
            } else if (match('<')) {
                addToken(TokenType::LESS_LESS);
            } else {
                addToken(TokenType::LESS);
            }
            break;
        case '>':
            if (match('=')) {
                addToken(TokenType::GREATER_EQUAL);
            } else if (match('>')) {
                addToken(TokenType::GREATER_GREATER);
            } else {
                addToken(TokenType::GREATER);
            }
            break;
        case '/':
            if (match('/')) {
                // A comment goes until the end of the line.
                while (peek() != '\n' && current < source.length()) advance();
            } else if (match('*')) {
                // A block comment goes until */
                while (!(peek() == '*' && peekNext() == '/') && current < source.length()) {
                    if (peek() == '\n') line++;
                    advance();
                }
                if (current < source.length()) {
                    advance(); // consume *
                    advance(); // consume /
                }
            } else if (match('=')) {
                addToken(TokenType::SLASH_EQUAL);
            }
            else {
                addToken(TokenType::SLASH);
            }
            break;
        case ':':
            addToken(match(':') ? TokenType::COLON_COLON : TokenType::COLON);
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
            // for character
            advance(); // consume the character
            if (advance() != '\'') {
                // error
            }
            addToken(TokenType::CHARACTER);
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
    addToken(TokenType::STRING, value);
}

void Lexer::number() {
    while (isdigit(peek())) advance();

    // Look for a fractional part.
    if (peek() == '.' && isdigit(peekNext())) {
        // Consume the "."
        advance();

        while (isdigit(peek())) advance();
    }

    addToken(TokenType::NUMBER, std::stod(source.substr(start, current - start)));
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

char Lexer::peek() {
    if (current >= source.length()) return '\0';
    return source[current];
}

char Lexer::peekNext() {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}
