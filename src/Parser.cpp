#include "Parser.hpp"
#include <stdexcept>
#include <vector>
#include <iostream>


namespace chtholly {

// Custom exception for parsing errors
struct ParseError : public std::runtime_error {
    using std::runtime_error::runtime_error;
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
        if (match({TokenType::PACKAGE})) return packageStatement();
        if (match({TokenType::IMPORT})) return importStatement();
        if (match({TokenType::CLASS})) return classDeclaration();
        if (match({TokenType::ENUM})) return enumDeclaration();
        if (match({TokenType::FN})) return function("function");
        if (match({TokenType::LET, TokenType::MUT})) return varDeclaration();
        return statement();
    } catch (const ParseError& error) {
        synchronize();
        return nullptr;
    }
}

std::shared_ptr<Stmt> Parser::statement() {
    if (match({TokenType::SWITCH})) return switchStatement();
    if (match({TokenType::FOR})) return forStatement();
    if (match({TokenType::WHILE})) return whileStatement();
    if (match({TokenType::IF})) return ifStatement();
    if (match({TokenType::RETURN})) return returnStatement();
    if (match({TokenType::BREAK})) {
        consume(TokenType::SEMICOLON, "Expect ';' after 'break'.");
        return std::make_shared<BreakStmt>();
    }
    if (match({TokenType::FALLTHROUGH})) {
        consume(TokenType::SEMICOLON, "Expect ';' after 'fallthrough'.");
        return std::make_shared<FallthroughStmt>();
    }
    if (match({TokenType::LEFT_BRACE})) {
        return std::make_shared<BlockStmt>(block());
    }
    return expressionStatement();
}

std::shared_ptr<Stmt> Parser::varDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
    std::shared_ptr<Type> type = nullptr;
    if (match({TokenType::COLON})) {
        type = this->type();
    }
    std::shared_ptr<Expr> initializer = nullptr;
    if (match({TokenType::EQUAL})) {
        initializer = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<VarStmt>(name, type, initializer);
}

std::shared_ptr<Stmt> Parser::expressionStatement() {
    auto expr = expression();
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

std::shared_ptr<Stmt> Parser::function(const std::string& kind) {
    Token name = consume(TokenType::IDENTIFIER, "Expect " + kind + " name.");
    std::vector<Token> type_params;
    if (match({TokenType::LESS})) {
        do {
            type_params.push_back(consume(TokenType::IDENTIFIER, "Expect type parameter name."));
        } while (match({TokenType::COMMA}));
        consume(TokenType::GREATER, "Expect '>' after type parameters.");
    }
    consume(TokenType::LEFT_PAREN, "Expect '(' after " + kind + " name.");
    std::vector<Parameter> parameters;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (parameters.size() >= 255) {
                // error("Can't have more than 255 parameters.");
            }
            Token param_name = consume(TokenType::IDENTIFIER, "Expect parameter name.");
            consume(TokenType::COLON, "Expect ':' after parameter name.");
            auto param_type = type();
            parameters.push_back({param_name, param_type});
        } while (match({TokenType::COMMA}));
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");

    std::shared_ptr<Type> return_type = nullptr;
    if (match({TokenType::COLON})) {
        return_type = type();
    }

    consume(TokenType::LEFT_BRACE, "Expect '{' before " + kind + " body.");
    auto body = std::make_shared<BlockStmt>(block());
    return std::make_shared<FunctionStmt>(name, type_params, parameters, return_type, body);
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

std::shared_ptr<Stmt> Parser::switchStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'switch'.");
    auto expression = this->expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after switch expression.");
    consume(TokenType::LEFT_BRACE, "Expect '{' before switch cases.");

    std::vector<Case> cases;
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        consume(TokenType::CASE, "Expect 'case' in switch statement.");
        auto condition = this->expression();
        consume(TokenType::COLON, "Expect ':' after case value.");
        auto body = statement();
        cases.emplace_back(condition, body);
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after switch cases.");
    return std::make_shared<SwitchStmt>(expression, cases);
}

std::shared_ptr<Stmt> Parser::enumDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect enum name.");
    consume(TokenType::LEFT_BRACE, "Expect '{' before enum body.");
    std::vector<Token> members;
    if (!check(TokenType::RIGHT_BRACE)) {
        do {
            members.push_back(consume(TokenType::IDENTIFIER, "Expect enum member name."));
        } while (match({TokenType::COMMA}));
    }
    consume(TokenType::RIGHT_BRACE, "Expect '}' after enum body.");
    return std::make_shared<EnumStmt>(name, members);
}

std::shared_ptr<Stmt> Parser::classDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect class name.");
    std::vector<Token> type_params;
    if (match({TokenType::LESS})) {
        do {
            type_params.push_back(consume(TokenType::IDENTIFIER, "Expect type parameter name."));
        } while (match({TokenType::COMMA}));
        consume(TokenType::GREATER, "Expect '>' after type parameters.");
    }
    consume(TokenType::LEFT_BRACE, "Expect '{' before class body.");

    std::vector<ClassStmt::ClassMember> members;
    AccessModifier current_access = AccessModifier::PUBLIC; // Default access
    bool is_static = false;

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        if (match({TokenType::PUBLIC})) {
            consume(TokenType::COLON, "Expect ':' after 'public'.");
            current_access = AccessModifier::PUBLIC;
            continue;
        }
        if (match({TokenType::PRIVATE})) {
            consume(TokenType::COLON, "Expect ':' after 'private'.");
            current_access = AccessModifier::PRIVATE;
            continue;
        }

        if (match({TokenType::STATIC})) {
            is_static = true;
        }

        std::shared_ptr<Stmt> member_decl;
        if (match({TokenType::FN})) {
            member_decl = function("method");
        } else if (check(TokenType::IDENTIFIER) && peek().lexeme == name.lexeme) {
            member_decl = constructorOrDestructorDeclaration();
        } else if (check(TokenType::TILDE)) {
            member_decl = constructorOrDestructorDeclaration();
        } else if (match({TokenType::LET, TokenType::MUT})) {
            member_decl = varDeclaration();
        } else {
            throw ParseError("Expect method or field declaration in class.");
        }

        members.push_back({member_decl, current_access, is_static});
        is_static = false; // Reset for the next member
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after class body.");
    return std::make_shared<ClassStmt>(name, type_params, members);
}

std::shared_ptr<Stmt> Parser::importStatement() {
    Token path;
    bool is_stdlib = false;
    if (match({TokenType::STRING})) {
        path = previous();
    } else if (match({TokenType::IDENTIFIER})) {
        path = previous();
        is_stdlib = true;
    } else {
        throw ParseError("Expect module path or identifier.");
    }

    Token alias = {TokenType::IDENTIFIER, "", {}, path.line};
    if (match({TokenType::AS})) {
        alias = consume(TokenType::IDENTIFIER, "Expect alias after 'as'.");
    }
    consume(TokenType::SEMICOLON, "Expect ';' after import statement.");
    return std::make_shared<ImportStmt>(path, alias, is_stdlib);
}

std::shared_ptr<Stmt> Parser::packageStatement() {
    Token name = consume(TokenType::IDENTIFIER, "Expect package name.");
    consume(TokenType::SEMICOLON, "Expect ';' after package name.");
    return std::make_shared<PackageStmt>(name);
}

std::shared_ptr<Stmt> Parser::constructorOrDestructorDeclaration() {
    Token name;
    if (peek().type == TokenType::TILDE) {
        Token tilde = advance();
        Token identifier = consume(TokenType::IDENTIFIER, "Expect class name after '~'.");
        name = Token{TokenType::IDENTIFIER, tilde.lexeme + identifier.lexeme, {}, tilde.line};
    } else {
        name = advance(); // Consume the identifier for constructor
    }

    consume(TokenType::LEFT_PAREN, "Expect '(' after constructor/destructor name.");
    std::vector<Parameter> parameters;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            Token param_name = consume(TokenType::IDENTIFIER, "Expect parameter name.");
            consume(TokenType::COLON, "Expect ':' after parameter name.");
            auto param_type = type();
            parameters.push_back({param_name, param_type});
        } while (match({TokenType::COMMA}));
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");

    consume(TokenType::LEFT_BRACE, "Expect '{' before constructor/destructor body.");
    auto body = std::make_shared<BlockStmt>(block());
    return std::make_shared<FunctionStmt>(name, std::vector<Token>{}, parameters, nullptr, body);
}


std::shared_ptr<Type> Parser::type() {
    if (match({TokenType::AMPERSAND_AMPERSAND})) {
        return std::make_shared<ReferenceType>(type(), ReferenceKind::MOVE);
    }
    if (match({TokenType::AMPERSAND})) {
        return std::make_shared<ReferenceType>(type(), ReferenceKind::MUTABLE);
    }
    if (match({TokenType::STAR})) {
        return std::make_shared<ReferenceType>(type(), ReferenceKind::COPY);
    }


    if (match({TokenType::LEFT_PAREN})) {
        std::vector<std::shared_ptr<Type>> param_types;
        if (!check(TokenType::RIGHT_PAREN)) {
            do {
                param_types.push_back(type());
            } while (match({TokenType::COMMA}));
        }
        consume(TokenType::RIGHT_PAREN, "Expect ')' after function type parameters.");
        consume(TokenType::COLON, "Expect ':' after function type parameters.");
        auto return_type = type();
        return std::make_shared<FunctionType>(param_types, return_type);
    }

    Token token = consume(TokenType::IDENTIFIER, "Expect type name.");
    auto primitive_type = std::make_shared<PrimitiveType>(token.lexeme);

    if (match({TokenType::LEFT_BRACKET})) {
        consume(TokenType::RIGHT_BRACKET, "Expect ']' after type.");
        return std::make_shared<ArrayType>(primitive_type);
    }

    return primitive_type;
}


// expression -> assignment
std::shared_ptr<Expr> Parser::expression() {
    return assignment();
}

// assignment -> ( call "." )? IDENTIFIER "=" assignment | equality
std::shared_ptr<Expr> Parser::assignment() {
    auto expr = equality();

    if (match({TokenType::EQUAL})) {
        Token equals = previous();
        auto value = assignment();

        if (auto var_expr = std::dynamic_pointer_cast<Variable>(expr)) {
            return std::make_shared<Assign>(expr, value);
        } else if (auto get_expr = std::dynamic_pointer_cast<GetExpr>(expr)) {
            return std::make_shared<SetExpr>(get_expr->object, get_expr->name, value);
        } else if (auto subscript_expr = std::dynamic_pointer_cast<SubscriptExpr>(expr)) {
             return std::make_shared<Assign>(expr, value);
        }

        // error(equals, "Invalid assignment target.");
    }

    return expr;
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

// unary -> ( "!" | "-" ) unary | call
std::shared_ptr<Expr> Parser::unary() {
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();
        auto right = unary();
        return std::make_shared<Unary>(op, right);
    }
    return call();
}

// call -> primary ( "(" arguments? ")" | "[" expression "]" | "::" IDENTIFIER | "." IDENTIFIER )*
std::shared_ptr<Expr> Parser::call() {
    auto expr = primary();
    while (true) {
        if (match({TokenType::LEFT_PAREN})) {
            expr = finishCall(expr, {});
        } else if (match({TokenType::LESS})) {
            int backtrack_point = current;
            try {
                std::vector<std::shared_ptr<Type>> type_arguments;
                do {
                    type_arguments.push_back(type());
                } while (match({TokenType::COMMA}));
                consume(TokenType::GREATER, "Expect '>' after type arguments.");
                consume(TokenType::LEFT_PAREN, "Expect '(' after type arguments.");
                expr = finishCall(expr, type_arguments);
            } catch (const ParseError& error) {
                current = backtrack_point - 1;
                break;
            }
        } else if (match({TokenType::LEFT_BRACKET})) {
            auto index = expression();
            consume(TokenType::RIGHT_BRACKET, "Expect ']' after subscript index.");
            expr = std::make_shared<SubscriptExpr>(expr, index);
        } else if (match({TokenType::COLON_COLON})) {
            Token name = consume(TokenType::IDENTIFIER, "Expect identifier after '::'.");
            expr = std::make_shared<ScopeExpr>(expr, name);
        } else if (match({TokenType::DOT})) {
            Token name = consume(TokenType::IDENTIFIER, "Expect property name after '.'.");
            expr = std::make_shared<GetExpr>(expr, name);
        } else {
            break;
        }
    }
    return expr;
}

std::shared_ptr<Expr> Parser::finishCall(std::shared_ptr<Expr> callee, std::vector<std::shared_ptr<Type>> type_arguments) {
    std::vector<std::shared_ptr<Expr>> arguments;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (arguments.size() >= 255) {
                // error("Can't have more than 255 arguments.");
            }
            arguments.push_back(expression());
        } while (match({TokenType::COMMA}));
    }
    Token paren = consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
    return std::make_shared<Call>(callee, paren, arguments, type_arguments);
}

// primary -> NUMBER | STRING | "true" | "false" | "this" | IDENTIFIER | "(" expression ")" | "[" elements? "]" | lambda
std::shared_ptr<Expr> Parser::primary() {
    if (match({TokenType::FALSE})) return std::make_shared<Literal>(false);
    if (match({TokenType::TRUE})) return std::make_shared<Literal>(true);
    if (match({TokenType::THIS})) return std::make_shared<ThisExpr>(previous());

    if (match({TokenType::NUMBER_INT, TokenType::NUMBER_FLOAT, TokenType::STRING})) {
        return std::make_shared<Literal>(previous().literal);
    }

    if (match({TokenType::IDENTIFIER})) {
        return std::make_shared<Variable>(previous());
    }

    if (match({TokenType::TYPE_CAST})) {
        consume(TokenType::LESS, "Expect '<' after 'type_cast'.");
        auto type = this->type();
        consume(TokenType::GREATER, "Expect '>' after type.");
        consume(TokenType::LEFT_PAREN, "Expect '(' after type.");
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<TypeCastExpr>(type, expr);
    }

    if (match({TokenType::LEFT_PAREN})) {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<Grouping>(expr);
    }

    if (check(TokenType::LEFT_BRACKET) && (current + 1 < tokens.size() && tokens[current + 1].type == TokenType::RIGHT_BRACKET) && (current + 2 < tokens.size() && tokens[current + 2].type == TokenType::LEFT_PAREN)) {
        // This is a lambda.
        advance(); // consume '['
        advance(); // consume ']'

        consume(TokenType::LEFT_PAREN, "Expect '(' after lambda '[]'.");
        std::vector<Parameter> parameters;
        if (!check(TokenType::RIGHT_PAREN)) {
            do {
                Token param_name = consume(TokenType::IDENTIFIER, "Expect parameter name.");
                consume(TokenType::COLON, "Expect ':' after parameter name.");
                auto param_type = type();
                parameters.push_back({param_name, param_type});
            } while (match({TokenType::COMMA}));
        }
        consume(TokenType::RIGHT_PAREN, "Expect ')' after lambda parameters.");

        std::shared_ptr<Type> return_type = nullptr;
        if (match({TokenType::COLON})) {
            return_type = type();
        }

        consume(TokenType::LEFT_BRACE, "Expect '{' before lambda body.");
        auto body = std::make_shared<BlockStmt>(block());
        return std::make_shared<LambdaExpr>(parameters, return_type, body);
    }

    if (match({TokenType::LEFT_BRACKET})) {
        // This is an array literal.
        std::vector<std::shared_ptr<Expr>> elements;
        if (!check(TokenType::RIGHT_BRACKET)) {
            do {
                elements.push_back(expression());
            } while (match({TokenType::COMMA}));
        }
        consume(TokenType::RIGHT_BRACKET, "Expect ']' after array literal.");
        return std::make_shared<ArrayLiteral>(elements);
    }

    throw ParseError("Expect expression.");
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
    throw ParseError(message);
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

void Parser::synchronize() {
    advance();
    while (!isAtEnd()) {
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
            case TokenType::SWITCH:
                return;
            default:
                // Do nothing.
                ;
        }
        advance();
    }
}

} // namespace chtholly
