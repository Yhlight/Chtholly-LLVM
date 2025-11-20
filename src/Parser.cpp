#include "Parser.hpp"
#include "Chtholly.hpp"
#include <iostream>

namespace chtholly {

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }
    return statements;
}

std::shared_ptr<Expr> Parser::expression() {
    return assignment();
}

std::shared_ptr<Expr> Parser::assignment() {
    auto expr = logical_or();

    if (match({TokenType::EQUAL})) {
        Token equals = previous();
        auto value = assignment();

        if (auto var = std::dynamic_pointer_cast<Variable>(expr)) {
            Token name = var->name;
            return std::make_shared<Assign>(name, value);
        }

        Chtholly::error(equals, "Invalid assignment target.");
    }

    return expr;
}

std::shared_ptr<Expr> Parser::logical_or() {
    auto expr = logical_and();

    while (match({TokenType::PIPE_PIPE})) {
        Token op = previous();
        auto right = logical_and();
        expr = std::make_shared<Binary>(expr, op, right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::logical_and() {
    auto expr = equality();

    while (match({TokenType::AMPERSAND_AMPERSAND})) {
        Token op = previous();
        auto right = equality();
        expr = std::make_shared<Binary>(expr, op, right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::equality() {
    auto expr = comparison();

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        Token op = previous();
        auto right = comparison();
        expr = std::make_shared<Binary>(expr, op, right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::comparison() {
    auto expr = term();

    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        Token op = previous();
        auto right = term();
        expr = std::make_shared<Binary>(expr, op, right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::term() {
    auto expr = factor();

    while (match({TokenType::MINUS, TokenType::PLUS})) {
        Token op = previous();
        auto right = factor();
        expr = std::make_shared<Binary>(expr, op, right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::factor() {
    auto expr = unary();

    while (match({TokenType::SLASH, TokenType::STAR, TokenType::PERCENT})) {
        Token op = previous();
        auto right = unary();
        expr = std::make_shared<Binary>(expr, op, right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::unary() {
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();
        auto right = unary();
        return std::make_shared<Unary>(op, right);
    }

    return call();
}

std::shared_ptr<Expr> Parser::call() {
    auto expr = primary();

    while (true) {
        if (match({TokenType::LEFT_PAREN})) {
            expr = finishCall(expr);
        } else {
            break;
        }
    }

    return expr;
}

std::shared_ptr<Expr> Parser::finishCall(std::shared_ptr<Expr> callee) {
    std::vector<std::shared_ptr<Expr>> arguments;
    if (peek().type != TokenType::RIGHT_PAREN) {
        do {
            arguments.push_back(expression());
        } while (match({TokenType::COMMA}));
    }

    Token paren = consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
    return std::make_shared<CallExpr>(callee, paren, arguments);
}

std::shared_ptr<Expr> Parser::primary() {
    if (match({TokenType::NUMBER, TokenType::STRING})) {
        return std::make_shared<Literal>(previous().literal);
    }

    if (match({TokenType::IDENTIFIER})) {
        return std::make_shared<Variable>(previous());
    }

    if (match({TokenType::LEFT_PAREN})) {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<Grouping>(expr);
    }

    throw std::runtime_error("Expect expression.");
}

std::shared_ptr<Stmt> Parser::statement() {
    if (match({TokenType::IF})) return ifStatement();
    if (match({TokenType::WHILE})) return whileStatement();
    if (match({TokenType::FOR})) return forStatement();
    if (match({TokenType::RETURN})) return returnStatement();
    if (match({TokenType::LEFT_BRACE})) return std::make_shared<BlockStmt>(block());
    return expressionStatement();
}

std::shared_ptr<Stmt> Parser::declaration() {
    try {
        if (match({TokenType::FN})) return function("function");
        if (match({TokenType::LET, TokenType::MUT})) {
            return varDeclaration();
        }
        return statement();
    } catch (...) {
        synchronize();
        return nullptr;
    }
}

std::shared_ptr<FunctionStmt> Parser::function(const std::string& kind) {
    Token name = consume(TokenType::IDENTIFIER, "Expect " + kind + " name.");
    consume(TokenType::LEFT_PAREN, "Expect '(' after " + kind + " name.");
    std::vector<Token> parameters;
    if (peek().type != TokenType::RIGHT_PAREN) {
        do {
            parameters.push_back(consume(TokenType::IDENTIFIER, "Expect parameter name."));
        } while (match({TokenType::COMMA}));
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");
    consume(TokenType::LEFT_BRACE, "Expect '{' before " + kind + " body.");
    auto body = block();
    return std::make_shared<FunctionStmt>(name, parameters, body);
}

std::shared_ptr<Stmt> Parser::varDeclaration() {
    Token keyword = previous();
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
    std::shared_ptr<Expr> initializer = nullptr;
    if (match({TokenType::EQUAL})) {
        initializer = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<VarStmt>(keyword, name, initializer);
}

std::shared_ptr<Stmt> Parser::expressionStatement() {
    auto expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return std::make_shared<ExpressionStmt>(expr);
}

std::shared_ptr<Stmt> Parser::ifStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

    auto thenBranch = statement();
    std::shared_ptr<Stmt> elseBranch = nullptr;
    if (match({TokenType::ELSE})) {
        elseBranch = statement();
    }

    return std::make_shared<IfStmt>(condition, thenBranch, elseBranch);
}

std::shared_ptr<Stmt> Parser::whileStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after while condition.");
    auto body = statement();

    return std::make_shared<WhileStmt>(condition, body);
}

std::shared_ptr<Stmt> Parser::forStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");

    std::shared_ptr<Stmt> initializer;
    if (match({TokenType::SEMICOLON})) {
        initializer = nullptr;
    } else if (match({TokenType::LET, TokenType::MUT})) {
        initializer = varDeclaration();
    } else {
        initializer = expressionStatement();
    }

    std::shared_ptr<Expr> condition = nullptr;
    if (!match({TokenType::SEMICOLON})) {
        condition = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");

    std::shared_ptr<Expr> increment = nullptr;
    if (!match({TokenType::RIGHT_PAREN})) {
        increment = expression();
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");

    auto body = statement();

    return std::make_shared<ForStmt>(initializer, condition, increment, body);
}

std::shared_ptr<Stmt> Parser::returnStatement() {
    Token keyword = previous();
    std::shared_ptr<Expr> value = nullptr;
    if (peek().type != TokenType::SEMICOLON) {
        value = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after return value.");
    return std::make_shared<ReturnStmt>(keyword, value);
}

std::vector<std::shared_ptr<Stmt>> Parser::block() {
    std::vector<std::shared_ptr<Stmt>> statements;

    while (peek().type != TokenType::RIGHT_BRACE && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

bool Parser::match(const std::vector<TokenType>& types) {
    for (TokenType type : types) {
        if (!isAtEnd() && peek().type == type) {
            advance();
            return true;
        }
    }
    return false;
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

Token Parser::consume(TokenType type, const std::string& message) {
    if (!isAtEnd() && peek().type == type) return advance();
    Chtholly::error(peek(), message);
    throw std::runtime_error(message);
}

void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) return;

        switch (peek().type) {
            case TokenType::CLASS:
            case TokenType::FN:
            case TokenType::LET:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::RETURN:
                return;
            default:
                break;
        }

        advance();
    }
}

} // namespace chtholly
