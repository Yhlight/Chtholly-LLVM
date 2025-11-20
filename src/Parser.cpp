#include "Parser.hpp"

namespace chtholly {

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!is_at_end()) {
        statements.push_back(declaration());
    }
    return statements;
}

std::shared_ptr<Stmt> Parser::declaration() {
    try {
        if (match({TokenType::FN})) return function_declaration();
        if (match({TokenType::LET})) return var_declaration(false);
        if (match({TokenType::MUT})) return var_declaration(true);
        return statement();
    } catch (ParseError& error) {
        synchronize();
        return nullptr;
    }
}

std::shared_ptr<Stmt> Parser::function_declaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect function name.");
    consume(TokenType::LEFT_PAREN, "Expect '(' after function name.");

    std::vector<Token> params;
    std::vector<Token> param_types;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            params.push_back(consume(TokenType::IDENTIFIER, "Expect parameter name."));
            consume(TokenType::COLON, "Expect ':' after parameter name.");
            if (match({TokenType::INT, TokenType::I8, TokenType::I16, TokenType::I32, TokenType::I64,
                       TokenType::U8, TokenType::U16, TokenType::U32, TokenType::U64,
                       TokenType::CHAR, TokenType::DOUBLE, TokenType::FLOAT, TokenType::LONG_DOUBLE,
                       TokenType::VOID, TokenType::BOOL, TokenType::STRING})) {
                param_types.push_back(previous());
            } else {
                throw error(peek(), "Expect type annotation.");
            }
        } while (match({TokenType::COMMA}));
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");

    std::optional<Token> return_type;
    if (match({TokenType::COLON})) {
        if (match({TokenType::INT, TokenType::I8, TokenType::I16, TokenType::I32, TokenType::I64,
                   TokenType::U8, TokenType::U16, TokenType::U32, TokenType::U64,
                   TokenType::CHAR, TokenType::DOUBLE, TokenType::FLOAT, TokenType::LONG_DOUBLE,
                   TokenType::VOID, TokenType::BOOL, TokenType::STRING})) {
            return_type = previous();
        } else {
            throw error(peek(), "Expect return type.");
        }
    }

    consume(TokenType::LEFT_BRACE, "Expect '{' before function body.");
    auto body = std::make_shared<BlockStmt>(block());
    return std::make_shared<FunctionStmt>(name, params, param_types, return_type, body);
}


std::shared_ptr<Stmt> Parser::var_declaration(bool is_mutable) {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");

    std::optional<Token> type;
    if (match({TokenType::COLON})) {
        if (match({TokenType::INT, TokenType::I8, TokenType::I16, TokenType::I32, TokenType::I64,
                   TokenType::U8, TokenType::U16, TokenType::U32, TokenType::U64,
                   TokenType::CHAR, TokenType::DOUBLE, TokenType::FLOAT, TokenType::LONG_DOUBLE,
                   TokenType::VOID, TokenType::BOOL, TokenType::STRING})) {
            type = previous();
        } else {
            throw error(peek(), "Expect type annotation.");
        }
    }

    std::shared_ptr<Expr> initializer = nullptr;
    if (match({TokenType::EQUAL})) {
        initializer = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<VarStmt>(name, type, initializer, is_mutable);
}

std::shared_ptr<Stmt> Parser::statement() {
    if (match({TokenType::IF})) return if_statement();
    if (match({TokenType::WHILE})) return while_statement();
    if (match({TokenType::FOR})) return for_statement();
    if (match({TokenType::LEFT_BRACE})) return std::make_shared<BlockStmt>(block());

    auto expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return std::make_shared<ExpressionStmt>(expr);
}

std::shared_ptr<Stmt> Parser::if_statement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

    auto then_branch = statement();
    std::shared_ptr<Stmt> else_branch = nullptr;
    if (match({TokenType::ELSE})) {
        else_branch = statement();
    }

    return std::make_shared<IfStmt>(condition, then_branch, else_branch);
}

std::shared_ptr<Stmt> Parser::while_statement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after while condition.");
    auto body = statement();

    return std::make_shared<WhileStmt>(condition, body);
}

std::shared_ptr<Stmt> Parser::for_statement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");

    std::shared_ptr<Stmt> initializer;
    if (match({TokenType::SEMICOLON})) {
        initializer = nullptr;
    } else if (match({TokenType::LET})) {
        initializer = var_declaration(false);
    } else {
        initializer = std::make_shared<ExpressionStmt>(expression());
    }

    std::shared_ptr<Expr> condition = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        condition = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");

    std::shared_ptr<Expr> increment = nullptr;
    if (!check(TokenType::RIGHT_PAREN)) {
        increment = expression();
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");

    auto body = statement();

    return std::make_shared<ForStmt>(initializer, condition, increment, body);
}

std::vector<std::shared_ptr<Stmt>> Parser::block() {
    std::vector<std::shared_ptr<Stmt>> statements;

    while (!check(TokenType::RIGHT_BRACE) && !is_at_end()) {
        statements.push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}


std::shared_ptr<Expr> Parser::expression() {
    return assignment();
}

std::shared_ptr<Expr> Parser::assignment() {
    auto expr = equality();

    if (match({TokenType::EQUAL})) {
        Token equals = previous();
        auto value = assignment();

        if (auto var = std::dynamic_pointer_cast<Variable>(expr)) {
            return std::make_shared<Assign>(var->name, value);
        }

        error(equals, "Invalid assignment target.");
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

    while (match({TokenType::SLASH, TokenType::STAR})) {
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
            std::vector<std::shared_ptr<Expr>> arguments;
            if (!check(TokenType::RIGHT_PAREN)) {
                do {
                    arguments.push_back(expression());
                } while (match({TokenType::COMMA}));
            }
            Token paren = consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
            expr = std::make_shared<CallExpr>(expr, paren, arguments);
        } else {
            break;
        }
    }

    return expr;
}

std::shared_ptr<Expr> Parser::primary() {
    if (match({TokenType::FALSE})) return std::make_shared<Literal>(false);
    if (match({TokenType::TRUE})) return std::make_shared<Literal>(true);
    if (match({TokenType::IDENTIFIER})) return std::make_shared<Variable>(previous());

    if (match({TokenType::NUMBER, TokenType::STRING})) {
        return std::make_shared<Literal>(previous().literal);
    }

    if (match({TokenType::LEFT_PAREN})) {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<Grouping>(expr);
    }

    throw error(peek(), "Expect expression.");
}


bool Parser::match(const std::vector<TokenType>& types) {
    for (const auto& type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw error(peek(), message);
}

bool Parser::check(TokenType type) const {
    if (is_at_end()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!is_at_end()) current++;
    return previous();
}

bool Parser::is_at_end() const {
    return peek().type == TokenType::EOS;
}

Token Parser::peek() const {
    return tokens[current];
}

Token Parser::previous() const {
    return tokens[current - 1];
}

Parser::ParseError Parser::error(const Token& token, const std::string& message) {
    // In a real compiler, you'd report this error to the user.
    // For now, we'll just throw an exception.
    return ParseError(message);
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
            default:
                break;
        }

        advance();
    }
}

} // namespace chtholly
