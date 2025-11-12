#include "Parser.h"
#include <stdexcept>

namespace Chtholly {

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }
    return statements;
}

std::shared_ptr<Stmt> Parser::declaration() {
    if (match({TokenType::Let, TokenType::Mut})) {
        return varDeclaration();
    }
    return statement();
}

std::shared_ptr<Stmt> Parser::varDeclaration() {
    Token name = consume(TokenType::Identifier, "Expect variable name.");
    std::shared_ptr<Expr> initializer = nullptr;
    if (match({TokenType::Equal})) {
        initializer = expression();
    }
    consume(TokenType::Semicolon, "Expect ';' after variable declaration.");
    return std::make_shared<Var>(name, initializer);
}

std::shared_ptr<Stmt> Parser::statement() {
    return expressionStatement();
}

std::shared_ptr<Stmt> Parser::expressionStatement() {
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::Semicolon, "Expect ';' after expression.");
    return std::make_shared<Expression>(expr);
}

std::shared_ptr<Expr> Parser::expression() {
    return assignment();
}

std::shared_ptr<Expr> Parser::assignment() {
    std::shared_ptr<Expr> expr = equality();

    if (match({TokenType::Equal})) {
        Token equals = previous();
        std::shared_ptr<Expr> value = assignment();

        if (auto var = std::dynamic_pointer_cast<Variable>(expr)) {
            Token name = var->name;
            return std::make_shared<Assign>(name, value);
        }

        throw std::runtime_error("Invalid assignment target.");
    }

    return expr;
}

std::shared_ptr<Expr> Parser::equality() {
    std::shared_ptr<Expr> expr = comparison();
    while (match({TokenType::BangEqual, TokenType::EqualEqual})) {
        Token op = previous();
        std::shared_ptr<Expr> right = comparison();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::comparison() {
    std::shared_ptr<Expr> expr = term();
    while (match({TokenType::Greater, TokenType::GreaterEqual, TokenType::Less, TokenType::LessEqual})) {
        Token op = previous();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::term() {
    std::shared_ptr<Expr> expr = factor();
    while (match({TokenType::Minus, TokenType::Plus})) {
        Token op = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::factor() {
    std::shared_ptr<Expr> expr = unary();
    while (match({TokenType::Slash, TokenType::Asterisk, TokenType::Percent})) {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::unary() {
    if (match({TokenType::Bang, TokenType::Minus})) {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        return std::make_shared<Unary>(op, right);
    }
    return primary();
}

std::shared_ptr<Expr> Parser::primary() {
    if (match({TokenType::IntegerLiteral, TokenType::DoubleLiteral, TokenType::StringLiteral, TokenType::CharLiteral})) {
        return std::make_shared<Literal>(previous().literal);
    }

    if (match({TokenType::Identifier})) {
        return std::make_shared<Variable>(previous());
    }

    if (match({TokenType::LeftParen})) {
        std::shared_ptr<Expr> expr = expression();
        consume(TokenType::RightParen, "Expect ')' after expression.");
        return expr;
    }

    throw std::runtime_error("Expect expression.");
}

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

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::Eof;
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

} // namespace Chtholly
