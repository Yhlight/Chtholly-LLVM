#ifndef CHTHOLLY_AST_HPP
#define CHTHOLLY_AST_HPP

#include "Token.hpp"
#include <any>
#include <memory>
#include <optional>
#include <vector>

// Forward declarations for all AST node types and visitors
struct AssignExpr;
struct BinaryExpr;
struct CallExpr;
struct GroupingExpr;
struct LiteralExpr;
struct LogicalExpr;
struct UnaryExpr;
struct VariableExpr;

class ExprVisitor {
public:
    virtual std::any visitAssignExpr(std::shared_ptr<AssignExpr> expr) = 0;
    virtual std::any visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) = 0;
    virtual std::any visitCallExpr(std::shared_ptr<CallExpr> expr) = 0;
    virtual std::any visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) = 0;
    virtual std::any visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) = 0;
    virtual std::any visitLogicalExpr(std::shared_ptr<LogicalExpr> expr) = 0;
    virtual std::any visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) = 0;
    virtual std::any visitVariableExpr(std::shared_ptr<VariableExpr> expr) = 0;
    virtual ~ExprVisitor() = default;
};

class Expr {
public:
    virtual std::any accept(ExprVisitor& visitor) = 0;
};

struct BlockStmt;
struct ExpressionStmt;
struct VarStmt;

class StmtVisitor {
public:
    virtual std::any visitBlockStmt(std::shared_ptr<BlockStmt> stmt) = 0;
    virtual std::any visitExpressionStmt(std::shared_ptr<ExpressionStmt> stmt) = 0;
    virtual std::any visitVarStmt(std::shared_ptr<VarStmt> stmt) = 0;
    virtual ~StmtVisitor() = default;
};

class Stmt {
public:
    virtual std::any accept(StmtVisitor& visitor) = 0;
};

// Concrete Expression Nodes
struct AssignExpr : Expr, public std::enable_shared_from_this<AssignExpr> {
    Token name;
    std::shared_ptr<Expr> value;

    AssignExpr(Token name, std::shared_ptr<Expr> value)
        : name(name), value(std::move(value)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitAssignExpr(shared_from_this());
    }
};

struct BinaryExpr : Expr, public std::enable_shared_from_this<BinaryExpr> {
    std::shared_ptr<Expr> left;
    Token op;
    std::shared_ptr<Expr> right;

    BinaryExpr(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitBinaryExpr(shared_from_this());
    }
};

struct CallExpr : Expr, public std::enable_shared_from_this<CallExpr> {
    std::shared_ptr<Expr> callee;
    Token paren;
    std::vector<std::shared_ptr<Expr>> arguments;

    CallExpr(std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments)
        : callee(std::move(callee)), paren(paren), arguments(std::move(arguments)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitCallExpr(shared_from_this());
    }
};

struct GroupingExpr : Expr, public std::enable_shared_from_this<GroupingExpr> {
    std::shared_ptr<Expr> expression;

    GroupingExpr(std::shared_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitGroupingExpr(shared_from_this());
    }
};

struct LiteralExpr : Expr, public std::enable_shared_from_this<LiteralExpr> {
    std::any value;

    LiteralExpr(std::any value) : value(std::move(value)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitLiteralExpr(shared_from_this());
    }
};

struct LogicalExpr : Expr, public std::enable_shared_from_this<LogicalExpr> {
    std::shared_ptr<Expr> left;
    Token op;
    std::shared_ptr<Expr> right;

    LogicalExpr(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitLogicalExpr(shared_from_this());
    }
};

struct UnaryExpr : Expr, public std::enable_shared_from_this<UnaryExpr> {
    Token op;
    std::shared_ptr<Expr> right;

    UnaryExpr(Token op, std::shared_ptr<Expr> right)
        : op(op), right(std::move(right)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitUnaryExpr(shared_from_this());
    }
};

struct VariableExpr : Expr, public std::enable_shared_from_this<VariableExpr> {
    Token name;

    VariableExpr(Token name) : name(name) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitVariableExpr(shared_from_this());
    }
};


// Concrete Statement Nodes
struct BlockStmt : Stmt, public std::enable_shared_from_this<BlockStmt> {
    std::vector<std::shared_ptr<Stmt>> statements;

    BlockStmt(std::vector<std::shared_ptr<Stmt>> statements)
        : statements(std::move(statements)) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visitBlockStmt(shared_from_this());
    }
};

struct ExpressionStmt : Stmt, public std::enable_shared_from_this<ExpressionStmt> {
    std::shared_ptr<Expr> expression;

    ExpressionStmt(std::shared_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visitExpressionStmt(shared_from_this());
    }
};

struct VarStmt : Stmt, public std::enable_shared_from_this<VarStmt> {
    Token name;
    std::optional<Token> type;
    std::shared_ptr<Expr> initializer;
    bool isMutable;

    VarStmt(Token name, std::optional<Token> type, std::shared_ptr<Expr> initializer, bool isMutable)
        : name(name), type(std::move(type)), initializer(std::move(initializer)), isMutable(isMutable) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visitVarStmt(shared_from_this());
    }
};

#endif // CHTHOLLY_AST_HPP
