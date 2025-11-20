#ifndef CHTHOLLY_PARSER_HPP
#define CHTHOLLY_PARSER_HPP

#include "Token.hpp"
#include "AST.hpp"
#include <vector>
#include <memory>
#include <stdexcept>

namespace chtholly {

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::vector<std::shared_ptr<Stmt>> parse();

private:
    std::shared_ptr<Stmt> declaration();
    std::shared_ptr<Stmt> statement();
    std::shared_ptr<Stmt> var_declaration(bool is_mutable);
    std::shared_ptr<Stmt> function_declaration();
    std::shared_ptr<Stmt> if_statement();
    std::shared_ptr<Stmt> while_statement();
    std::shared_ptr<Stmt> for_statement();
    std::vector<std::shared_ptr<Stmt>> block();
    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> assignment();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> call();
    std::shared_ptr<Expr> primary();

    bool match(const std::vector<TokenType>& types);
    Token consume(TokenType type, const std::string& message);
    bool check(TokenType type) const;
    Token advance();
    bool is_at_end() const;
    Token peek() const;
    Token previous() const;

    // Error handling
    struct ParseError : std::runtime_error {
        using std::runtime_error::runtime_error;
    };
    ParseError error(const Token& token, const std::string& message);
    void synchronize();

    const std::vector<Token>& tokens;
    int current = 0;
};

} // namespace chtholly

#endif //CHTHOLLY_PARSER_HPP
