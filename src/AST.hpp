#ifndef CHTHOLLY_AST_HPP
#define CHTHOLLY_AST_HPP

#include "Token.hpp"
#include <any>
#include <memory>
#include <vector>

namespace chtholly {

// Forward declarations
struct Binary;
struct Grouping;
struct Literal;
struct Unary;

// Visitor interface
template <typename R>
struct ExprVisitor {
    virtual R visit(const Binary& expr) = 0;
    virtual R visit(const Grouping& expr) = 0;
    virtual R visit(const Literal& expr) = 0;
    virtual R visit(const Unary& expr) = 0;
    virtual ~ExprVisitor() = default;
};

// Base class for all expression nodes
struct Expr {
    virtual std::any accept(ExprVisitor<std::any>& visitor) const = 0;
};

// Concrete expression classes
struct Binary : Expr {
    Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

    std::any accept(ExprVisitor<std::any>& visitor) const override {
        return visitor.visit(*this);
    }

    const std::shared_ptr<Expr> left;
    const Token op;
    const std::shared_ptr<Expr> right;
};

struct Grouping : Expr {
    Grouping(std::shared_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    std::any accept(ExprVisitor<std::any>& visitor) const override {
        return visitor.visit(*this);
    }

    const std::shared_ptr<Expr> expression;
};

struct Literal : Expr {
    Literal(std::any value) : value(std::move(value)) {}

    std::any accept(ExprVisitor<std::any>& visitor) const override {
        return visitor.visit(*this);
    }

    const std::any value;
};

struct Unary : Expr {
    Unary(Token op, std::shared_ptr<Expr> right)
        : op(std::move(op)), right(std::move(right)) {}

    std::any accept(ExprVisitor<std::any>& visitor) const override {
        return visitor.visit(*this);
    }

    const Token op;
    const std::shared_ptr<Expr> right;
};

} // namespace chtholly

#endif // CHTHOLLY_AST_HPP
