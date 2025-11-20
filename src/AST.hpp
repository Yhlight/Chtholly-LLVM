#ifndef CHTHOLLY_AST_HPP
#define CHTHOLLY_AST_HPP

#include "Token.hpp"
#include <vector>
#include <memory>
#include <optional>

namespace chtholly {

// Forward declare all AST node types
struct Binary;
struct Grouping;
struct Literal;
struct Unary;
struct ExpressionStmt;
struct VarStmt;

// Visitor for Expressions
struct ExprVisitor {
    virtual std::any visit(std::shared_ptr<Binary> expr) = 0;
    virtual std::any visit(std::shared_ptr<Grouping> expr) = 0;
    virtual std::any visit(std::shared_ptr<Literal> expr) = 0;
    virtual std::any visit(std::shared_ptr<Unary> expr) = 0;
};

// Base class for all expressions
struct Expr {
    virtual ~Expr() = default;
    virtual std::any accept(ExprVisitor& visitor) = 0;
};

// Visitor for Statements
struct StmtVisitor {
    virtual std::any visit(std::shared_ptr<ExpressionStmt> stmt) = 0;
    virtual std::any visit(std::shared_ptr<VarStmt> stmt) = 0;
};

// Base class for all statements
struct Stmt {
    virtual ~Stmt() = default;
    virtual std::any accept(StmtVisitor& visitor) = 0;
};


// Concrete Expression Nodes
struct Binary : Expr, public std::enable_shared_from_this<Binary> {
    std::shared_ptr<Expr> left;
    Token op;
    std::shared_ptr<Expr> right;

    Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct Grouping : Expr, public std::enable_shared_from_this<Grouping> {
    std::shared_ptr<Expr> expression;

    Grouping(std::shared_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct Literal : Expr, public std::enable_shared_from_this<Literal> {
    std::any value;

    Literal(std::any value) : value(std::move(value)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct Unary : Expr, public std::enable_shared_from_this<Unary> {
    Token op;
    std::shared_ptr<Expr> right;

    Unary(Token op, std::shared_ptr<Expr> right)
        : op(std::move(op)), right(std::move(right)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit(shared_from_this());
    }
};


// Concrete Statement Nodes
struct ExpressionStmt : Stmt, public std::enable_shared_from_this<ExpressionStmt> {
    std::shared_ptr<Expr> expression;

    ExpressionStmt(std::shared_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct VarStmt : Stmt, public std::enable_shared_from_this<VarStmt> {
    Token name;
    std::optional<Token> type;
    std::shared_ptr<Expr> initializer;
    bool is_mutable;

    VarStmt(Token name, std::optional<Token> type, std::shared_ptr<Expr> initializer, bool is_mutable)
        : name(std::move(name)), type(std::move(type)), initializer(std::move(initializer)), is_mutable(is_mutable) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visit(shared_from_this());
    }
};


} // namespace chtholly

#endif //CHTHOLLY_AST_HPP
