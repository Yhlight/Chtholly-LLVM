#include "Parser.hpp"
#include <stdexcept>

class ParseError : public std::runtime_error {
public:
    ParseError() : std::runtime_error("Parse error") {}
};

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }
    return statements;
}

std::shared_ptr<Stmt> Parser::declaration() {
    try {
        if (match({TokenType::LET})) return varDeclaration(false);
        if (match({TokenType::MUT})) return varDeclaration(true);
        return statement();
    } catch (ParseError& error) {
        synchronize();
        return nullptr;
    }
}

std::shared_ptr<Stmt> Parser::varDeclaration(bool isMutable) {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
    std::optional<Token> type;
    if (match({TokenType::COLON})) {
        type = consume(TokenType::IDENTIFIER, "Expect type specifier.");
    }
    std::shared_ptr<Expr> initializer = nullptr;
    if (match({TokenType::EQUAL})) {
        initializer = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<VarStmt>(name, type, initializer, isMutable);
}

std::shared_ptr<Stmt> Parser::statement() {
    return expressionStatement();
}

std::shared_ptr<Stmt> Parser::expressionStatement() {
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return std::make_shared<ExpressionStmt>(expr);
}

std::vector<std::shared_ptr<Stmt>> Parser::block() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }
    consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

std::shared_ptr<Expr> Parser::expression() {
    return assignment();
}

std::shared_ptr<Expr> Parser::assignment() {
    std::shared_ptr<Expr> expr = logic_or();
    if (match({TokenType::EQUAL, TokenType::PLUS_EQUAL, TokenType::MINUS_EQUAL, TokenType::STAR_EQUAL, TokenType::SLASH_EQUAL, TokenType::PERCENT_EQUAL})) {
        Token op = previous();
        std::shared_ptr<Expr> value = assignment();
        if (auto varExpr = std::dynamic_pointer_cast<VariableExpr>(expr)) {
            return std::make_shared<AssignExpr>(expr, op, value);
        }
        throw std::runtime_error("Invalid assignment target.");
    }
    return expr;
}

std::shared_ptr<Expr> Parser::logic_or() {
    std::shared_ptr<Expr> expr = logic_and();
    while (match({TokenType::PIPE_PIPE})) {
        Token op = previous();
        std::shared_ptr<Expr> right = logic_and();
        expr = std::make_shared<LogicalExpr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::logic_and() {
    std::shared_ptr<Expr> expr = equality();
    while (match({TokenType::AMPERSAND_AMPERSAND})) {
        Token op = previous();
        std::shared_ptr<Expr> right = equality();
        expr = std::make_shared<LogicalExpr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::equality() {
    std::shared_ptr<Expr> expr = comparison();
    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        Token op = previous();
        std::shared_ptr<Expr> right = comparison();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::comparison() {
    std::shared_ptr<Expr> expr = term();
    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        Token op = previous();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::term() {
    std::shared_ptr<Expr> expr = factor();
    while (match({TokenType::MINUS, TokenType::PLUS})) {
        Token op = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::factor() {
    std::shared_ptr<Expr> expr = unary();
    while (match({TokenType::SLASH, TokenType::STAR})) {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::unary() {
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        return std::make_shared<UnaryExpr>(op, right);
    }
    return primary();
}

std::shared_ptr<Expr> Parser::primary() {
    if (match({TokenType::FALSE})) return std::make_shared<LiteralExpr>(false);
    if (match({TokenType::TRUE})) return std::make_shared<LiteralExpr>(true);

    if (match({TokenType::NUMBER, TokenType::STRING})) {
        return std::make_shared<LiteralExpr>(previous().literal);
    }

    if (match({TokenType::IDENTIFIER})) {
        return std::make_shared<VariableExpr>(previous());
    }

    if (match({TokenType::LEFT_PAREN})) {
        std::shared_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<GroupingExpr>(expr);
    }

    throw ParseError();
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
    return peek().type == TokenType::END_OF_FILE;
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

void Parser::synchronize() {
    advance();
    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) return;
        switch (peek().type) {
            case TokenType::LET:
            case TokenType::MUT:
                return;
        }
        advance();
    }
}
