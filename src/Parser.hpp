#pragma once

#include "Token.hpp"
#include "AST.hpp"
#include <vector>
#include <memory>

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::vector<std::shared_ptr<Stmt>> parse();
    bool had_error = false;

private:
    std::shared_ptr<Stmt> declaration();
    std::shared_ptr<Stmt> var_declaration();
    std::shared_ptr<Stmt> statement();
    std::shared_ptr<Stmt> expression_statement();
    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> assignment();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> primary();

    bool match(const std::vector<TokenType>& types);
    Token consume(TokenType type, const std::string& message);
    bool check(TokenType type);
    Token advance();
    bool is_at_end();
    Token peek();
    Token previous();
    void synchronize();

    std::vector<Token> tokens;
    int current = 0;
};
