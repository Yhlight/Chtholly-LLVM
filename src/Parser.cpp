#include "Parser.hpp"
#include <stdexcept>
#include <utility>

// Simple error class for parsing
class ParseError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        try {
            statements.push_back(declaration());
        } catch (const ParseError& error) {
            // In case of an error, synchronize to the next statement
            synchronize();
        }
    }
    return statements;
}

// Grammar Implementation

std::shared_ptr<Stmt> Parser::declaration() {
    if (match({TokenType::LET, TokenType::MUT})) {
        return varDeclaration();
    }
    return statement();
}

std::shared_ptr<Stmt> Parser::varDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");

    consume(TokenType::ASSIGN, "Expect '=' after variable name for initializer.");
    std::shared_ptr<Expr> initializer = expression();

    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<VarStmt>(name, initializer);
}

std::shared_ptr<Stmt> Parser::statement() {
    if (match({TokenType::LBRACE})) {
        return std::make_shared<BlockStmt>(block());
    }

    return expressionStatement();
}

std::vector<std::shared_ptr<Stmt>> Parser::block() {
    std::vector<std::shared_ptr<Stmt>> statements;

    while (!check(TokenType::RBRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(TokenType::RBRACE, "Expect '}' after block.");
    return statements;
}

std::shared_ptr<Stmt> Parser::expressionStatement() {
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return std::make_shared<ExpressionStmt>(expr);
}

std::shared_ptr<Expr> Parser::expression() {
    return assignment();
}

std::shared_ptr<Expr> Parser::assignment() {
    std::shared_ptr<Expr> expr = equality();

    if (match({TokenType::ASSIGN})) {
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

    while (match({TokenType::NOT_EQUAL, TokenType::EQUAL})) {
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
    if (match({TokenType::LOGICAL_NOT, TokenType::MINUS})) {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        return std::make_shared<Unary>(op, right);
    }

    return call();
}

std::shared_ptr<Expr> Parser::primary() {
    if (match({TokenType::FALSE})) return std::make_shared<Literal>(false);
    if (match({TokenType::TRUE})) return std::make_shared<Literal>(true);
    if (match({TokenType::NONE})) return std::make_shared<Literal>(nullptr);

    if (match({TokenType::INTEGER, TokenType::DOUBLE})) {
        if (previous().type == TokenType::INTEGER)
            return std::make_shared<Literal>(std::stoi(previous().lexeme));
        else
            return std::make_shared<Literal>(std::stod(previous().lexeme));
    }

    if (match({TokenType::STRING})) {
        return std::make_shared<Literal>(previous().lexeme);
    }

    if (match({TokenType::IDENTIFIER})) {
        return std::make_shared<Variable>(previous());
    }

    if (match({TokenType::LPAREN})) {
        std::shared_ptr<Expr> expr = expression();
        consume(TokenType::RPAREN, "Expect ')' after expression.");
        return std::make_shared<Grouping>(expr);
    }

    throw std::runtime_error("Expect expression.");
}

std::shared_ptr<Expr> Parser::call() {
    std::shared_ptr<Expr> expr = primary();

    while (true) {
        if (match({TokenType::LPAREN})) {
            expr = finishCall(expr);
        } else {
            break;
        }
    }

    return expr;
}

std::shared_ptr<Expr> Parser::finishCall(std::shared_ptr<Expr> callee) {
    std::vector<std::shared_ptr<Expr>> arguments;
    if (!check(TokenType::RPAREN)) {
        do {
            if (arguments.size() >= 255) {
                throw std::runtime_error("Can't have more than 255 arguments.");
            }
            arguments.push_back(expression());
        } while (match({TokenType::COMMA}));
    }

    Token paren = consume(TokenType::RPAREN, "Expect ')' after arguments.");

    return std::make_shared<Call>(callee, paren, arguments);
}


// Helper Methods

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
    return tokens.at(current);
}

Token Parser::previous() {
    return tokens.at(current - 1);
}

void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) return;

        switch (peek().type) {
            case TokenType::FUNC:
            case TokenType::LET:
            case TokenType::MUT:
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
