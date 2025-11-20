#ifndef CHTHOLLY_PARSER_HPP
#define CHTHOLLY_PARSER_HPP

#include "AST.hpp"
#include "Token.hpp"
#include <vector>
#include <memory>

namespace chtholly {

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::vector<std::shared_ptr<Stmt>> parse();

private:
    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> assignment();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> call();
    std::shared_ptr<Expr> primary();

    std::shared_ptr<Expr> finishCall(std::shared_ptr<Expr> callee);
    std::shared_ptr<Stmt> declaration();
    std::shared_ptr<Stmt> statement();
    std::shared_ptr<Stmt> varDeclaration();
    std::shared_ptr<Stmt> expressionStatement();
    std::vector<std::shared_ptr<Stmt>> block();
    std::shared_ptr<Stmt> function(const std::string& kind);
    std::shared_ptr<Stmt> returnStatement();
    std::shared_ptr<Stmt> ifStatement();
    std::shared_ptr<Stmt> whileStatement();
    std::shared_ptr<Stmt> forStatement();
    std::shared_ptr<Stmt> switchStatement();
    std::shared_ptr<Type> type();


    bool match(const std::vector<TokenType>& types);
    Token consume(TokenType type, const std::string& message);
    bool check(TokenType type) const;
    Token advance();
    bool isAtEnd() const;
    Token peek() const;
    Token previous() const;
    void synchronize();


    const std::vector<Token>& tokens;
    int current = 0;
};

} // namespace chtholly

#endif // CHTHOLLY_PARSER_HPP
