#include "Parser.hpp"
#include <stdexcept>
#include <vector>

namespace chtholly {

// Custom exception for parsing errors
struct ParseError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

std::shared_ptr<Expr> Parser::parse() {
    try {
        return expression();
    } catch (const ParseError& error) {
        // In the future, we can add more sophisticated error reporting here.
        return nullptr;
    }
}

// expression -> equality
std::shared_ptr<Expr> Parser::expression() {
    return equality();
}

// equality -> comparison ( ( "!=" | "==" ) comparison )*
std::shared_ptr<Expr> Parser::equality() {
    auto expr = comparison();
    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        Token op = previous();
        auto right = comparison();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

// comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )*
std::shared_ptr<Expr> Parser::comparison() {
    auto expr = term();
    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        Token op = previous();
        auto right = term();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

// term -> factor ( ( "-" | "+" ) factor )*
std::shared_ptr<Expr> Parser::term() {
    auto expr = factor();
    while (match({TokenType::MINUS, TokenType::PLUS})) {
        Token op = previous();
        auto right = factor();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

// factor -> unary ( ( "/" | "*" ) unary )*
std::shared_ptr<Expr> Parser::factor() {
    auto expr = unary();
    while (match({TokenType::SLASH, TokenType::STAR})) {
        Token op = previous();
        auto right = unary();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

// unary -> ( "!" | "-" ) unary | primary
std::shared_ptr<Expr> Parser::unary() {
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();
        auto right = unary();
        return std::make_shared<Unary>(op, right);
    }
    return primary();
}

// primary -> NUMBER | STRING | "true" | "false" | "(" expression ")"
std::shared_ptr<Expr> Parser::primary() {
    if (match({TokenType::FALSE})) return std::make_shared<Literal>(false);
    if (match({TokenType::TRUE})) return std::make_shared<Literal>(true);

    if (match({TokenType::NUMBER_INT, TokenType::NUMBER_FLOAT, TokenType::STRING})) {
        return std::make_shared<Literal>(previous().literal);
    }

    if (match({TokenType::LEFT_PAREN})) {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<Grouping>(expr);
    }

    throw std::runtime_error("Expect expression.");
}

// --- Helper methods ---

bool Parser::match(const std::vector<TokenType>& types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw std::runtime_error(message);
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::END_OF_FILE;
}

Token Parser::peek() const {
    return tokens[current];
}

Token Parser::previous() const {
    return tokens[current - 1];
}

} // namespace chtholly
