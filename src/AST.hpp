#pragma once

#include "Token.hpp"
#include <any>
#include <memory>
#include <vector>

// Forward declarations for mutual recursion
struct Expr;
struct Stmt;
struct Binary;
struct Grouping;
struct Literal;
struct Unary;
struct ExpressionStmt;

// Visitor interface for expressions
class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;
    virtual std::any visit(std::shared_ptr<Binary> expr) = 0;
    virtual std::any visit(std::shared_ptr<Grouping> expr) = 0;
    virtual std::any visit(std::shared_ptr<Literal> expr) = 0;
    virtual std::any visit(std::shared_ptr<Unary> expr) = 0;
};

// Base class for all expression nodes
struct Expr {
    virtual ~Expr() = default;
    virtual std::any accept(ExprVisitor& visitor) = 0;
};

// --- Concrete Expression Nodes ---

struct Binary : Expr, std::enable_shared_from_this<Binary> {
    Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::shared_ptr<Expr> left;
    const Token op;
    const std::shared_ptr<Expr> right;
};

struct Grouping : Expr, std::enable_shared_from_this<Grouping> {
    Grouping(std::shared_ptr<Expr> expression) : expression(std::move(expression)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::shared_ptr<Expr> expression;
};

struct Literal : Expr, std::enable_shared_from_this<Literal> {
    Literal(std::any value) : value(std::move(value)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::any value;
};

struct Unary : Expr, std::enable_shared_from_this<Unary> {
    Unary(Token op, std::shared_ptr<Expr> right) : op(std::move(op)), right(std::move(right)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const Token op;
    const std::shared_ptr<Expr> right;
};

// Visitor interface for statements
class StmtVisitor {
public:
    virtual ~StmtVisitor() = default;
    virtual std::any visit(std::shared_ptr<ExpressionStmt> stmt) = 0;
};

// Base class for all statement nodes
struct Stmt {
    virtual ~Stmt() = default;
    virtual std::any accept(StmtVisitor& visitor) = 0;
};

// --- Concrete Statement Nodes ---

struct ExpressionStmt : Stmt, std::enable_shared_from_this<ExpressionStmt> {
    ExpressionStmt(std::shared_ptr<Expr> expression) : expression(std::move(expression)) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::shared_ptr<Expr> expression;
};
