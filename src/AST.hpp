#ifndef CHTHOLLY_AST_HPP
#define CHTHOLLY_AST_HPP

#include "Token.hpp"
#include <any>
#include <memory>
#include <vector>
#include <utility>

namespace chtholly {

// Forward declarations
struct Binary;
struct Grouping;
struct Literal;
struct Unary;
struct Variable;
struct Call;
struct ExpressionStmt;
struct VarStmt;
struct BlockStmt;
struct FunctionStmt;
struct ReturnStmt;
struct Expr;
struct Stmt;

// Visitor interfaces
template <typename R>
struct ExprVisitor {
    virtual R visit(const std::shared_ptr<Binary>& expr) = 0;
    virtual R visit(const std::shared_ptr<Grouping>& expr) = 0;
    virtual R visit(const std::shared_ptr<Literal>& expr) = 0;
    virtual R visit(const std::shared_ptr<Unary>& expr) = 0;
    virtual R visit(const std::shared_ptr<Variable>& expr) = 0;
    virtual R visit(const std::shared_ptr<Call>& expr) = 0;
    virtual ~ExprVisitor() = default;
};

template <typename R>
struct StmtVisitor {
    virtual R visit(const std::shared_ptr<ExpressionStmt>& stmt) = 0;
    virtual R visit(const std::shared_ptr<VarStmt>& stmt) = 0;
    virtual R visit(const std::shared_ptr<BlockStmt>& stmt) = 0;
    virtual R visit(const std::shared_ptr<FunctionStmt>& stmt) = 0;
    virtual R visit(const std::shared_ptr<ReturnStmt>& stmt) = 0;
    virtual ~StmtVisitor() = default;
};

// Base classes
struct Expr {
    virtual std::any accept(ExprVisitor<std::any>& visitor) = 0;
};

struct Stmt {
    virtual std::any accept(StmtVisitor<std::any>& visitor) = 0;
};

// Concrete expression classes
struct Binary : Expr, public std::enable_shared_from_this<Binary> {
    Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

    std::any accept(ExprVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::shared_ptr<Expr> left;
    const Token op;
    const std::shared_ptr<Expr> right;
};

struct Grouping : Expr, public std::enable_shared_from_this<Grouping> {
    Grouping(std::shared_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    std::any accept(ExprVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::shared_ptr<Expr> expression;
};

struct Literal : Expr, public std::enable_shared_from_this<Literal> {
    Literal(std::any value) : value(std::move(value)) {}

    std::any accept(ExprVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::any value;
};

struct Unary : Expr, public std::enable_shared_from_this<Unary> {
    Unary(Token op, std::shared_ptr<Expr> right)
        : op(std::move(op)), right(std::move(right)) {}

    std::any accept(ExprVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const Token op;
    const std::shared_ptr<Expr> right;
};

struct Variable : Expr, public std::enable_shared_from_this<Variable> {
    Variable(Token name) : name(std::move(name)) {}

    std::any accept(ExprVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const Token name;
};

struct Call : Expr, public std::enable_shared_from_this<Call> {
    Call(std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments)
        : callee(std::move(callee)), paren(std::move(paren)), arguments(std::move(arguments)) {}

    std::any accept(ExprVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::shared_ptr<Expr> callee;
    const Token paren;
    const std::vector<std::shared_ptr<Expr>> arguments;
};

// Concrete statement classes
struct ExpressionStmt : Stmt, public std::enable_shared_from_this<ExpressionStmt> {
    ExpressionStmt(std::shared_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    std::any accept(StmtVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::shared_ptr<Expr> expression;
};

struct VarStmt : Stmt, public std::enable_shared_from_this<VarStmt> {
    VarStmt(Token name, std::shared_ptr<Expr> initializer)
        : name(std::move(name)), initializer(std::move(initializer)) {}

    std::any accept(StmtVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const Token name;
    const std::shared_ptr<Expr> initializer;
};

struct BlockStmt : Stmt, public std::enable_shared_from_this<BlockStmt> {
    BlockStmt(std::vector<std::shared_ptr<Stmt>> statements)
        : statements(std::move(statements)) {}

    std::any accept(StmtVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::vector<std::shared_ptr<Stmt>> statements;
};

struct FunctionStmt : Stmt, public std::enable_shared_from_this<FunctionStmt> {
    FunctionStmt(Token name, std::vector<Token> params, std::shared_ptr<BlockStmt> body)
        : name(std::move(name)), params(std::move(params)), body(std::move(body)) {}

    std::any accept(StmtVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const Token name;
    const std::vector<Token> params;
    const std::shared_ptr<BlockStmt> body;
};

struct ReturnStmt : Stmt, public std::enable_shared_from_this<ReturnStmt> {
    ReturnStmt(Token keyword, std::shared_ptr<Expr> value)
        : keyword(std::move(keyword)), value(std::move(value)) {}

    std::any accept(StmtVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const Token keyword;
    const std::shared_ptr<Expr> value;
};

} // namespace chtholly

#endif // CHTHOLLY_AST_HPP
