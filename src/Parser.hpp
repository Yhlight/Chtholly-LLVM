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
    std::shared_ptr<Expr> parse();

private:
    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> primary();

    bool match(const std::vector<TokenType>& types);
    Token consume(TokenType type, const std::string& message);
    bool check(TokenType type) const;
    Token advance();
    bool isAtEnd() const;
    Token peek() const;
    Token previous() const;

    const std::vector<Token>& tokens;
    int current = 0;
};

} // namespace chtholly

#endif // CHTHOLLY_PARSER_HPP
