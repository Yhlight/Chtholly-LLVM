#ifndef CHTHOLLY_AST_HPP
#define CHTHOLLY_AST_HPP

#include "Token.hpp"
#include <any>
#include <memory>
#include <vector>

// Forward declarations for all AST node classes
struct Binary;
struct Unary;
struct Literal;
struct Grouping;
struct Variable;
struct Assign;
struct Call;

struct ExpressionStmt;
struct VarStmt;
struct BlockStmt;


// Visitor for Expressions
struct ExprVisitor {
    virtual std::any visitBinaryExpr(const std::shared_ptr<Binary>& expr) = 0;
    virtual std::any visitUnaryExpr(const std::shared_ptr<Unary>& expr) = 0;
    virtual std::any visitLiteralExpr(const std::shared_ptr<Literal>& expr) = 0;
    virtual std::any visitGroupingExpr(const std::shared_ptr<Grouping>& expr) = 0;
    virtual std::any visitVariableExpr(const std::shared_ptr<Variable>& expr) = 0;
    virtual std::any visitAssignExpr(const std::shared_ptr<Assign>& expr) = 0;
    virtual std::any visitCallExpr(const std::shared_ptr<Call>& expr) = 0;
    virtual ~ExprVisitor() = default;
};

// Base class for all expression nodes
struct Expr {
    virtual std::any accept(ExprVisitor& visitor) = 0;
};

// Concrete Expression Nodes
struct Binary : Expr, public std::enable_shared_from_this<Binary> {
    const std::shared_ptr<Expr> left;
    const Token op;
    const std::shared_ptr<Expr> right;

    Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitBinaryExpr(shared_from_this());
    }
};

struct Unary : Expr, public std::enable_shared_from_this<Unary> {
    const Token op;
    const std::shared_ptr<Expr> right;

    Unary(Token op, std::shared_ptr<Expr> right)
        : op(std::move(op)), right(std::move(right)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitUnaryExpr(shared_from_this());
    }
};

struct Literal : Expr, public std::enable_shared_from_this<Literal> {
    const std::any value;

    explicit Literal(std::any value) : value(std::move(value)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitLiteralExpr(shared_from_this());
    }
};

struct Grouping : Expr, public std::enable_shared_from_this<Grouping> {
    const std::shared_ptr<Expr> expression;

    explicit Grouping(std::shared_ptr<Expr> expression) : expression(std::move(expression)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitGroupingExpr(shared_from_this());
    }
};

struct Variable : Expr, public std::enable_shared_from_this<Variable> {
    const Token name;

    explicit Variable(Token name) : name(std::move(name)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitVariableExpr(shared_from_this());
    }
};

struct Assign : Expr, public std::enable_shared_from_this<Assign> {
    const Token name;
    const std::shared_ptr<Expr> value;

    Assign(Token name, std::shared_ptr<Expr> value)
        : name(std::move(name)), value(std::move(value)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitAssignExpr(shared_from_this());
    }
};

struct Call : Expr, public std::enable_shared_from_this<Call> {
    const std::shared_ptr<Expr> callee;
    const Token paren; // for error reporting
    const std::vector<std::shared_ptr<Expr>> arguments;

    Call(std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments)
        : callee(std::move(callee)), paren(std::move(paren)), arguments(std::move(arguments)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitCallExpr(shared_from_this());
    }
};


// Visitor for Statements
struct StmtVisitor {
    virtual std::any visitExpressionStmt(const std::shared_ptr<ExpressionStmt>& stmt) = 0;
    virtual std::any visitVarStmt(const std::shared_ptr<VarStmt>& stmt) = 0;
    virtual std::any visitBlockStmt(const std::shared_ptr<BlockStmt>& stmt) = 0;
    virtual ~StmtVisitor() = default;
};

// Base class for all statement nodes
struct Stmt {
    virtual std::any accept(StmtVisitor& visitor) = 0;
};

// Concrete Statement Nodes
struct ExpressionStmt : Stmt, public std::enable_shared_from_this<ExpressionStmt> {
    const std::shared_ptr<Expr> expression;

    explicit ExpressionStmt(std::shared_ptr<Expr> expression) : expression(std::move(expression)) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visitExpressionStmt(shared_from_this());
    }
};

struct VarStmt : Stmt, public std::enable_shared_from_this<VarStmt> {
    const Token name;
    const std::shared_ptr<Expr> initializer;

    VarStmt(Token name, std::shared_ptr<Expr> initializer)
        : name(std::move(name)), initializer(std::move(initializer)) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visitVarStmt(shared_from_this());
    }
};

struct BlockStmt : Stmt, public std::enable_shared_from_this<BlockStmt> {
    const std::vector<std::shared_ptr<Stmt>> statements;

    explicit BlockStmt(std::vector<std::shared_ptr<Stmt>> statements) : statements(std::move(statements)) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visitBlockStmt(shared_from_this());
    }
};


#endif // CHTHOLLY_AST_HPP
