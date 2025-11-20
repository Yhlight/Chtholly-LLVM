#ifndef CHTHOLLY_AST_HPP
#define CHTHOLLY_AST_HPP

#include "Lexer.hpp"
#include <any>
#include <memory>
#include <vector>

namespace chtholly {

struct Expr;
struct Binary;
struct Grouping;
struct Literal;
struct Unary;

struct ExprVisitor {
    virtual std::any visitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
    virtual std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
    virtual std::any visitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
    virtual std::any visitUnaryExpr(std::shared_ptr<Unary> expr) = 0;
};

struct Expr {
    virtual std::any accept(ExprVisitor& visitor) = 0;
};

struct Binary : Expr, public std::enable_shared_from_this<Binary> {
    Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
        : left(left), op(op), right(right) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitBinaryExpr(shared_from_this());
    }

    const std::shared_ptr<Expr> left;
    const Token op;
    const std::shared_ptr<Expr> right;
};

struct Grouping : Expr, public std::enable_shared_from_this<Grouping> {
    Grouping(std::shared_ptr<Expr> expression) : expression(expression) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitGroupingExpr(shared_from_this());
    }

    const std::shared_ptr<Expr> expression;
};

struct Literal : Expr, public std::enable_shared_from_this<Literal> {
    Literal(std::any value) : value(value) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitLiteralExpr(shared_from_this());
    }

    const std::any value;
};

struct Unary : Expr, public std::enable_shared_from_this<Unary> {
    Unary(Token op, std::shared_ptr<Expr> right) : op(op), right(right) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitUnaryExpr(shared_from_this());
    }

    const Token op;
    const std::shared_ptr<Expr> right;
};

} // namespace chtholly

#endif // CHTHOLLY_AST_HPP
