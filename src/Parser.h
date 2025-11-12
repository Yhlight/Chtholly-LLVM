#ifndef CHTHOLLY_PARSER_H
#define CHTHOLLY_PARSER_H

#include "Expr.h"
#include "Stmt.h"
#include "Token.h"
#include <vector>

namespace Chtholly {

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::vector<std::shared_ptr<Stmt>> parse();

private:
    std::shared_ptr<Stmt> declaration();
    std::shared_ptr<Stmt> varDeclaration();
    std::shared_ptr<Stmt> statement();
    std::shared_ptr<Stmt> expressionStatement();

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
    bool isAtEnd();
    Token peek();
    Token previous();

    const std::vector<Token>& tokens;
    int current = 0;
};

} // namespace Chtholly

#endif // CHTHOLLY_PARSER_H
