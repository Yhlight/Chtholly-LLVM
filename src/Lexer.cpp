#include "Lexer.hpp"
#include <cctype>

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
    {"enum", TokenType::ENUM},
    {"import", TokenType::IMPORT},
    {"as", TokenType::AS},
    {"package", TokenType::PACKAGE},
    {"switch", TokenType::SWITCH},
    {"case", TokenType::CASE},
    {"fallthrough", TokenType::FALLTHROUGH},
    {"do", TokenType::DO},
    {"static", TokenType::STATIC},
    {"public", TokenType::PUBLIC},
    {"private", TokenType::PRIVATE},
    {"this", TokenType::THIS},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE}
};

Lexer::Lexer(const std::string& source) : source(source) {}

std::vector<Token> Lexer::scanTokens() {
    while (current < source.length()) {
        start = current;
        scanToken();
    }
    tokens.push_back({TokenType::END_OF_FILE, "", {}, line});
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
            if (match('-')) addToken(TokenType::MINUS_MINUS);
            else if (match('=')) addToken(TokenType::MINUS_EQUAL);
            else addToken(TokenType::MINUS);
            break;
        case '+':
            if (match('+')) addToken(TokenType::PLUS_PLUS);
            else if (match('=')) addToken(TokenType::PLUS_EQUAL);
            else addToken(TokenType::PLUS);
            break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '*':
            if (match('=')) addToken(TokenType::STAR_EQUAL);
            else addToken(TokenType::STAR);
            break;
        case '%':
            if (match('=')) addToken(TokenType::PERCENT_EQUAL);
            else addToken(TokenType::PERCENT);
            break;
        case '!':
            addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
             if (match('<')) addToken(TokenType::LESS_LESS);
             else addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
             break;
        case '>':
            if (match('>')) addToken(TokenType::GREATER_GREATER);
            else addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        case '/':
            if (match('/')) {
                while (peek() != '\n' && current < source.length()) advance();
            } else if (match('*')) {
                blockComment();
            }
            else if (match('=')) {
                addToken(TokenType::SLASH_EQUAL);
            }
            else {
                addToken(TokenType::SLASH);
            }
            break;
        case '&':
            if (match('&')) addToken(TokenType::AMPERSAND_AMPERSAND);
            else addToken(TokenType::AMPERSAND);
            break;
        case '|':
            if (match('|')) addToken(TokenType::PIPE_PIPE);
            else addToken(TokenType::PIPE);
            break;
        case '^': addToken(TokenType::CARET); break;
        case '~': addToken(TokenType::TILDE); break;
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
        default:
            if (isdigit(c)) {
                number();
            } else if (isalpha(c) || c == '_') {
                identifier();
            } else {
                // Handle error
            }
            break;
    }
}

char Lexer::advance() {
    return source[current++];
}

void Lexer::addToken(TokenType type, std::any literal) {
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
        // Unterminated string error
        return;
    }

    advance(); // The closing ".

    std::string value = source.substr(start + 1, current - start - 2);
    addToken(TokenType::STRING, value);
}

void Lexer::number() {
    while (isdigit(peek())) advance();

    if (peek() == '.' && isdigit(peekNext())) {
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
        type = keywords.at(text);
    }
    addToken(type);
}

void Lexer::blockComment() {
    while (!(peek() == '*' && peekNext() == '/') && current < source.length()) {
        if (peek() == '\n') line++;
        advance();
    }
    if (current >= source.length()) {
        // Unterminated block comment error
        return;
    }
    advance(); // *
    advance(); // /
}
