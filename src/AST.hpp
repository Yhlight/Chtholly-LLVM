#ifndef CHTHOLLY_AST_HPP
#define CHTHOLLY_AST_HPP

#include "Lexer.hpp"
#include <any>
#include <memory>
#include <vector>

namespace chtholly {

// Forward declarations for expressions
struct Expr;
struct Assign;
struct Binary;
struct CallExpr;
struct Grouping;
struct Literal;
struct Unary;
struct Variable;

// Visitor for expressions
struct ExprVisitor {
    virtual std::any visitAssignExpr(std::shared_ptr<Assign> expr) = 0;
    virtual std::any visitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
    virtual std::any visitCallExpr(std::shared_ptr<CallExpr> expr) = 0;
    virtual std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
    virtual std::any visitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
    virtual std::any visitUnaryExpr(std::shared_ptr<Unary> expr) = 0;
    virtual std::any visitVariableExpr(std::shared_ptr<Variable> expr) = 0;
};

// Base class for expressions
struct Expr {
    virtual std::any accept(ExprVisitor& visitor) = 0;
};

// Forward declarations for statements
struct Stmt;
struct BlockStmt;
struct ExpressionStmt;
struct ForStmt;
struct FunctionStmt;
struct IfStmt;
struct ReturnStmt;
struct VarStmt;
struct WhileStmt;

// Visitor for statements
struct StmtVisitor {
    virtual std::any visitBlockStmt(std::shared_ptr<BlockStmt> stmt) = 0;
    virtual std::any visitExpressionStmt(std::shared_ptr<ExpressionStmt> stmt) = 0;
    virtual std::any visitForStmt(std::shared_ptr<ForStmt> stmt) = 0;
    virtual std::any visitFunctionStmt(std::shared_ptr<FunctionStmt> stmt) = 0;
    virtual std::any visitIfStmt(std::shared_ptr<IfStmt> stmt) = 0;
    virtual std::any visitReturnStmt(std::shared_ptr<ReturnStmt> stmt) = 0;
    virtual std::any visitVarStmt(std::shared_ptr<VarStmt> stmt) = 0;
    virtual std::any visitWhileStmt(std::shared_ptr<WhileStmt> stmt) = 0;
};

// Base class for statements
struct Stmt {
    virtual std::any accept(StmtVisitor& visitor) = 0;
};

// Expression implementations
struct Assign : Expr, public std::enable_shared_from_this<Assign> {
    Assign(Token name, std::shared_ptr<Expr> value)
        : name(name), value(value) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitAssignExpr(shared_from_this());
    }

    const Token name;
    const std::shared_ptr<Expr> value;
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

struct CallExpr : Expr, public std::enable_shared_from_this<CallExpr> {
    CallExpr(std::shared_ptr<Expr> callee, Token paren, const std::vector<std::shared_ptr<Expr>>& arguments)
        : callee(callee), paren(paren), arguments(arguments) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitCallExpr(shared_from_this());
    }

    const std::shared_ptr<Expr> callee;
    const Token paren;
    const std::vector<std::shared_ptr<Expr>> arguments;
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

struct Variable : Expr, public std::enable_shared_from_this<Variable> {
    Variable(Token name) : name(name) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitVariableExpr(shared_from_this());
    }

    const Token name;
};

// Statement implementations
struct BlockStmt : Stmt, public std::enable_shared_from_this<BlockStmt> {
    BlockStmt(const std::vector<std::shared_ptr<Stmt>>& statements) : statements(statements) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visitBlockStmt(shared_from_this());
    }

    const std::vector<std::shared_ptr<Stmt>> statements;
};

struct ExpressionStmt : Stmt, public std::enable_shared_from_this<ExpressionStmt> {
    ExpressionStmt(std::shared_ptr<Expr> expression) : expression(expression) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visitExpressionStmt(shared_from_this());
    }

    const std::shared_ptr<Expr> expression;
};

struct ForStmt : Stmt, public std::enable_shared_from_this<ForStmt> {
    ForStmt(std::shared_ptr<Stmt> initializer, std::shared_ptr<Expr> condition,
            std::shared_ptr<Expr> increment, std::shared_ptr<Stmt> body)
        : initializer(initializer), condition(condition), increment(increment), body(body) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visitForStmt(shared_from_this());
    }

    const std::shared_ptr<Stmt> initializer;
    const std::shared_ptr<Expr> condition;
    const std::shared_ptr<Expr> increment;
    const std::shared_ptr<Stmt> body;
};

struct FunctionStmt : Stmt, public std::enable_shared_from_this<FunctionStmt> {
    FunctionStmt(Token name, const std::vector<Token>& params, const std::vector<std::shared_ptr<Stmt>>& body)
        : name(name), params(params), body(body) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visitFunctionStmt(shared_from_this());
    }

    const Token name;
    const std::vector<Token> params;
    const std::vector<std::shared_ptr<Stmt>> body;
};

struct IfStmt : Stmt, public std::enable_shared_from_this<IfStmt> {
    IfStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch,
           std::shared_ptr<Stmt> elseBranch)
        : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visitIfStmt(shared_from_this());
    }

    const std::shared_ptr<Expr> condition;
    const std::shared_ptr<Stmt> thenBranch;
    const std::shared_ptr<Stmt> elseBranch;
};

struct ReturnStmt : Stmt, public std::enable_shared_from_this<ReturnStmt> {
    ReturnStmt(Token keyword, std::shared_ptr<Expr> value)
        : keyword(keyword), value(value) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visitReturnStmt(shared_from_this());
    }

    const Token keyword;
    const std::shared_ptr<Expr> value;
};

struct VarStmt : Stmt, public std::enable_shared_from_this<VarStmt> {
    VarStmt(Token keyword, Token name, std::shared_ptr<Expr> initializer)
        : keyword(keyword), name(name), initializer(initializer) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visitVarStmt(shared_from_this());
    }

    const Token keyword;
    const Token name;
    const std::shared_ptr<Expr> initializer;
};

struct WhileStmt : Stmt, public std::enable_shared_from_this<WhileStmt> {
    WhileStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body)
        : condition(condition), body(body) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visitWhileStmt(shared_from_this());
    }

    const std::shared_ptr<Expr> condition;
    const std::shared_ptr<Stmt> body;
};

} // namespace chtholly

#endif // CHTHOLLY_AST_HPP
