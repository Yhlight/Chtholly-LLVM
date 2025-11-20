#pragma once

#include "Token.hpp"
#include <vector>
#include <memory>
#include <any>

// Forward declarations
struct BinaryExpr;
struct GroupingExpr;
struct LiteralExpr;
struct UnaryExpr;
struct VarExpr;
struct AssignExpr;
struct CallExpr;

// Visitor for expressions
template<typename R>
struct ExprVisitor {
    virtual R visit(const BinaryExpr& expr) = 0;
    virtual R visit(const GroupingExpr& expr) = 0;
    virtual R visit(const LiteralExpr& expr) = 0;
    virtual R visit(const UnaryExpr& expr) = 0;
    virtual R visit(const VarExpr& expr) = 0;
    virtual R visit(const AssignExpr& expr) = 0;
    virtual R visit(const CallExpr& expr) = 0;
};

// Base class for expressions
struct Expr {
    virtual ~Expr() = default;
    virtual std::any accept(ExprVisitor<std::any>& visitor) const = 0;
};

// Concrete expression classes
struct BinaryExpr : Expr {
    BinaryExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}

    std::any accept(ExprVisitor<std::any>& visitor) const override {
        return visitor.visit(*this);
    }

    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
};

struct GroupingExpr : Expr {
    GroupingExpr(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    std::any accept(ExprVisitor<std::any>& visitor) const override {
        return visitor.visit(*this);
    }

    std::unique_ptr<Expr> expression;
};

struct LiteralExpr : Expr {
    LiteralExpr(std::any value) : value(value) {}

    std::any accept(ExprVisitor<std::any>& visitor) const override {
        return visitor.visit(*this);
    }

    std::any value;
};

struct UnaryExpr : Expr {
    UnaryExpr(Token op, std::unique_ptr<Expr> right)
        : op(op), right(std::move(right)) {}

    std::any accept(ExprVisitor<std::any>& visitor) const override {
        return visitor.visit(*this);
    }

    Token op;
    std::unique_ptr<Expr> right;
};

struct VarExpr : Expr {
    VarExpr(Token name) : name(name) {}

    std::any accept(ExprVisitor<std::any>& visitor) const override {
        return visitor.visit(*this);
    }

    Token name;
};

struct AssignExpr : Expr {
    AssignExpr(Token name, std::unique_ptr<Expr> value)
        : name(name), value(std::move(value)) {}

    std::any accept(ExprVisitor<std::any>& visitor) const override {
        return visitor.visit(*this);
    }

    Token name;
    std::unique_ptr<Expr> value;
};

struct CallExpr : Expr {
    CallExpr(std::unique_ptr<Expr> callee, Token paren, std::vector<std::unique_ptr<Expr>> arguments)
        : callee(std::move(callee)), paren(paren), arguments(std::move(arguments)) {}

    std::any accept(ExprVisitor<std::any>& visitor) const override {
        return visitor.visit(*this);
    }

    std::unique_ptr<Expr> callee;
    Token paren;
    std::vector<std::unique_ptr<Expr>> arguments;
};

// Forward declarations
struct ExprStmt;
struct VarStmt;
struct BlockStmt;
struct IfStmt;
struct WhileStmt;
struct ReturnStmt;

// Visitor for statements
template<typename R>
struct StmtVisitor {
    virtual R visit(const ExprStmt& stmt) = 0;
    virtual R visit(const VarStmt& stmt) = 0;
    virtual R visit(const BlockStmt& stmt) = 0;
    virtual R visit(const IfStmt& stmt) = 0;
    virtual R visit(const WhileStmt& stmt) = 0;
    virtual R visit(const ReturnStmt& stmt) = 0;
};

// Base class for statements
struct Stmt {
    virtual ~Stmt() = default;
    virtual std::any accept(StmtVisitor<std::any>& visitor) const = 0;
};

// Concrete statement classes
struct ExprStmt : Stmt {
    ExprStmt(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    std::any accept(StmtVisitor<std::any>& visitor) const override {
        return visitor.visit(*this);
    }

    std::unique_ptr<Expr> expression;
};

struct VarStmt : Stmt {
    VarStmt(Token name, std::unique_ptr<Expr> initializer, bool isMutable)
        : name(name), initializer(std::move(initializer)), isMutable(isMutable) {}

    std::any accept(StmtVisitor<std::any>& visitor) const override {
        return visitor.visit(*this);
    }

    Token name;
    std::unique_ptr<Expr> initializer;
    bool isMutable;
};

struct BlockStmt : Stmt {
    BlockStmt(std::vector<std::unique_ptr<Stmt>> statements)
        : statements(std::move(statements)) {}

    std::any accept(StmtVisitor<std::any>& visitor) const override {
        return visitor.visit(*this);
    }

    std::vector<std::unique_ptr<Stmt>> statements;
};

struct IfStmt : Stmt {
    IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch)
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}

    std::any accept(StmtVisitor<std::any>& visitor) const override {
        return visitor.visit(*this);
    }

    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;
};

struct WhileStmt : Stmt {
    WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
        : condition(std::move(condition)), body(std::move(body)) {}

    std::any accept(StmtVisitor<std::any>& visitor) const override {
        return visitor.visit(*this);
    }

    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;
};

struct ReturnStmt : Stmt {
    ReturnStmt(Token keyword, std::unique_ptr<Expr> value)
        : keyword(keyword), value(std::move(value)) {}

    std::any accept(StmtVisitor<std::any>& visitor) const override {
        return visitor.visit(*this);
    }

    Token keyword;
    std::unique_ptr<Expr> value;
};
