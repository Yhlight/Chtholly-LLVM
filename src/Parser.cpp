#include "Parser.hpp"
#include <iostream>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!is_at_end()) {
        auto decl = declaration();
        if (decl) {
            statements.push_back(decl);
        }
    }
    return statements;
}

std::shared_ptr<Stmt> Parser::declaration() {
    try {
        if (match({TokenType::LET, TokenType::MUT})) return var_declaration();
        return statement();
    } catch (const std::runtime_error& error) {
        std::cerr << "[line " << peek().line << "] Error: " << error.what() << std::endl;
        had_error = true;
        synchronize();
        return nullptr;
    }
}

std::shared_ptr<Stmt> Parser::var_declaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
    std::shared_ptr<Expr> initializer = nullptr;
    if (match({TokenType::EQUAL})) {
        initializer = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<VarStmt>(name, initializer);
}

std::shared_ptr<Stmt> Parser::statement() {
    return expression_statement();
}

std::shared_ptr<Stmt> Parser::expression_statement() {
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return std::make_shared<ExpressionStmt>(expr);
}

std::shared_ptr<Expr> Parser::expression() {
    return assignment();
}

std::shared_ptr<Expr> Parser::assignment() {
    std::shared_ptr<Expr> expr = equality();
    if (match({TokenType::EQUAL})) {
        Token equals = previous();
        std::shared_ptr<Expr> value = assignment();
        if (auto var = std::dynamic_pointer_cast<Variable>(expr)) {
            return std::make_shared<Assign>(var->name, value);
        }
        throw std::runtime_error("Invalid assignment target.");
    }
    return expr;
}

std::shared_ptr<Expr> Parser::equality() {
    std::shared_ptr<Expr> expr = comparison();
    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        Token op = previous();
        std::shared_ptr<Expr> right = comparison();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::comparison() {
    std::shared_ptr<Expr> expr = term();
    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        Token op = previous();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::term() {
    std::shared_ptr<Expr> expr = factor();
    while (match({TokenType::MINUS, TokenType::PLUS})) {
        Token op = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::factor() {
    std::shared_ptr<Expr> expr = unary();
    while (match({TokenType::SLASH, TokenType::STAR})) {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::unary() {
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        return std::make_shared<Unary>(op, right);
    }
    return primary();
}

std::shared_ptr<Expr> Parser::primary() {
    if (match({TokenType::FALSE})) return std::make_shared<Literal>(false);
    if (match({TokenType::TRUE})) return std::make_shared<Literal>(true);

    if (match({TokenType::INTEGER})) {
        return std::make_shared<Literal>(std::stoi(previous().lexeme));
    }
    if (match({TokenType::DOUBLE})) {
        return std::make_shared<Literal>(std::stod(previous().lexeme));
    }
    if (match({TokenType::STRING})) {
        std::string value = previous().lexeme;
        return std::make_shared<Literal>(value.substr(1, value.length() - 2));
    }

    if (match({TokenType::IDENTIFIER})) {
        return std::make_shared<Variable>(previous());
    }
    if (match({TokenType::LEFT_PAREN})) {
        std::shared_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<Grouping>(expr);
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
    if (is_at_end()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!is_at_end()) current++;
    return previous();
}

bool Parser::is_at_end() {
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
    while (!is_at_end()) {
        if (previous().type == TokenType::SEMICOLON) return;
        switch (peek().type) {
            case TokenType::CLASS:
            case TokenType::FN:
            case TokenType::LET:
            case TokenType::MUT:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::RETURN:
                return;
        }
        advance();
    }
}
