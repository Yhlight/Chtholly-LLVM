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
    if (match({TokenType::STRUCT})) {
        return structDeclaration();
    }
    if (match({TokenType::FUNC})) {
        return function("function");
    }
    if (match({TokenType::LET, TokenType::MUT})) {
        return varDeclaration();
    }
    return statement();
}

std::shared_ptr<Stmt> Parser::varDeclaration() {
    bool isMutable = previous().type == TokenType::MUT;
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");

    std::shared_ptr<Expr> initializer = nullptr;
    if (match({TokenType::ASSIGN})) {
        initializer = expression();
    }

    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<VarStmt>(name, initializer, isMutable);
}

std::shared_ptr<Stmt> Parser::statement() {
    if (match({TokenType::IF})) {
        return ifStatement();
    }
    if (match({TokenType::WHILE})) {
        return whileStatement();
    }
    if (match({TokenType::FOR})) {
        return forStatement();
    }
    if (match({TokenType::RETURN})) {
        return returnStatement();
    }
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

std::shared_ptr<Stmt> Parser::ifStatement() {
    consume(TokenType::LPAREN, "Expect '(' after 'if'.");
    std::shared_ptr<Expr> condition = expression();
    consume(TokenType::RPAREN, "Expect ')' after if condition.");

    std::shared_ptr<Stmt> thenBranch = statement();
    std::shared_ptr<Stmt> elseBranch = nullptr;
    if (match({TokenType::ELSE})) {
        elseBranch = statement();
    }

    return std::make_shared<IfStmt>(condition, thenBranch, elseBranch);
}

std::shared_ptr<Stmt> Parser::whileStatement() {
    consume(TokenType::LPAREN, "Expect '(' after 'while'.");
    std::shared_ptr<Expr> condition = expression();
    consume(TokenType::RPAREN, "Expect ')' after condition.");
    std::shared_ptr<Stmt> body = statement();

    return std::make_shared<WhileStmt>(condition, body);
}

std::shared_ptr<Stmt> Parser::function(const std::string& kind) {
    Token name = consume(TokenType::IDENTIFIER, "Expect " + kind + " name.");
    consume(TokenType::LPAREN, "Expect '(' after " + kind + " name.");
    std::vector<FunctionStmt::Parameter> parameters;
    if (!check(TokenType::RPAREN)) {
        do {
            if (parameters.size() >= 255) {
                throw std::runtime_error("Can't have more than 255 parameters.");
            }
            Token paramName = consume(TokenType::IDENTIFIER, "Expect parameter name.");
            consume(TokenType::COLON, "Expect ':' after parameter name for type annotation.");
            Token paramType = consume(TokenType::IDENTIFIER, "Expect parameter type.");
            parameters.push_back({paramName, paramType});
        } while (match({TokenType::COMMA}));
    }
    consume(TokenType::RPAREN, "Expect ')' after parameters.");

    Token returnType(TokenType::IDENTIFIER, "void", -1); // Default return type
    if (match({TokenType::ARROW})) {
        returnType = consume(TokenType::IDENTIFIER, "Expect return type after '->'.");
    }

    consume(TokenType::LBRACE, "Expect '{' before " + kind + " body.");
    std::vector<std::shared_ptr<Stmt>> body = block();
    return std::make_shared<FunctionStmt>(name, parameters, body, returnType);
}

std::shared_ptr<Stmt> Parser::structDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect struct name.");
    consume(TokenType::LBRACE, "Expect '{' before struct body.");
    std::vector<std::shared_ptr<VarStmt>> fields;
    while (!check(TokenType::RBRACE) && !isAtEnd()) {
        bool isMutable = match({TokenType::MUT});
        if (!isMutable) consume(TokenType::LET, "Expect 'let' or 'mut' for field declaration.");

        Token fieldName = consume(TokenType::IDENTIFIER, "Expect field name.");
        consume(TokenType::COLON, "Expect ':' after field name.");
        Token fieldType = consume(TokenType::IDENTIFIER, "Expect field type.");
        consume(TokenType::SEMICOLON, "Expect ';' after field declaration.");
        fields.push_back(std::make_shared<VarStmt>(fieldName, nullptr, isMutable, fieldType));
    }
    consume(TokenType::RBRACE, "Expect '}' after struct body.");
    return std::make_shared<StructStmt>(name, fields);
}

std::shared_ptr<Stmt> Parser::forStatement() {
    consume(TokenType::LPAREN, "Expect '(' after 'for'.");
    consume(TokenType::LET, "Expect 'let' for loop variable declaration.");
    Token variable = consume(TokenType::IDENTIFIER, "Expect loop variable name.");
    consume(TokenType::COLON, "Expect ':' after loop variable.");
    std::shared_ptr<Expr> collection = expression();
    consume(TokenType::RPAREN, "Expect ')' after for clauses.");
    std::shared_ptr<Stmt> body = statement();

    return std::make_shared<ForStmt>(variable, collection, body);
}

std::shared_ptr<Stmt> Parser::returnStatement() {
    Token keyword = previous();
    std::shared_ptr<Expr> value = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        value = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after return value.");
    return std::make_shared<ReturnStmt>(keyword, value);
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
        if (check(TokenType::LBRACE)) {
            Token name = previous();
            consume(TokenType::LBRACE, "Expect '{' after struct name for initialization.");
            std::vector<std::pair<Token, std::shared_ptr<Expr>>> fields;
            if (!check(TokenType::RBRACE)) {
                do {
                    Token fieldName = consume(TokenType::IDENTIFIER, "Expect field name.");
                    consume(TokenType::COLON, "Expect ':' after field name.");
                    std::shared_ptr<Expr> value = expression();
                    fields.push_back({fieldName, value});
                } while (match({TokenType::COMMA}));
            }
            consume(TokenType::RBRACE, "Expect '}' after struct fields.");
            return std::make_shared<StructInitExpr>(name, fields);
        }
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
        } else if (match({TokenType::DOT})) {
            Token name = consume(TokenType::IDENTIFIER, "Expect property name after '.'.");
            expr = std::make_shared<GetExpr>(expr, name);
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
