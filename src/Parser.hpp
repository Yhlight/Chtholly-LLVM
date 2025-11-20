#ifndef CHTHOLLY_PARSER_HPP
#define CHTHOLLY_PARSER_HPP

#include "AST.hpp"
#include "Lexer.hpp"
#include <vector>

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
    Token advance();
    bool isAtEnd();
    Token peek();
    Token previous();
    Token consume(TokenType type, const std::string& message);
    void synchronize();

    std::vector<Token> tokens;
    int current = 0;
};

} // namespace chtholly

#endif // CHTHOLLY_PARSER_HPP
