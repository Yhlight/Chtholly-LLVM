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
struct Assign;
struct Variable;
struct CallExpr;
struct ExpressionStmt;
struct VarStmt;
struct BlockStmt;
struct IfStmt;
struct WhileStmt;
struct ForStmt;
struct FunctionStmt;

// Visitor for Expressions
struct ExprVisitor {
    virtual std::any visit(std::shared_ptr<Binary> expr) = 0;
    virtual std::any visit(std::shared_ptr<Grouping> expr) = 0;
    virtual std::any visit(std::shared_ptr<Literal> expr) = 0;
    virtual std::any visit(std::shared_ptr<Unary> expr) = 0;
    virtual std::any visit(std::shared_ptr<Assign> expr) = 0;
    virtual std::any visit(std::shared_ptr<Variable> expr) = 0;
    virtual std::any visit(std::shared_ptr<CallExpr> expr) = 0;
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
    virtual std::any visit(std::shared_ptr<BlockStmt> stmt) = 0;
    virtual std::any visit(std::shared_ptr<IfStmt> stmt) = 0;
    virtual std::any visit(std::shared_ptr<WhileStmt> stmt) = 0;
    virtual std::any visit(std::shared_ptr<ForStmt> stmt) = 0;
    virtual std::any visit(std::shared_ptr<FunctionStmt> stmt) = 0;
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

struct CallExpr : Expr, public std::enable_shared_from_this<CallExpr> {
    std::shared_ptr<Expr> callee;
    Token paren; // for error reporting
    std::vector<std::shared_ptr<Expr>> arguments;

    CallExpr(std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments)
        : callee(std::move(callee)), paren(std::move(paren)), arguments(std::move(arguments)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct Variable : Expr, public std::enable_shared_from_this<Variable> {
    Token name;

    Variable(Token name) : name(std::move(name)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct Assign : Expr, public std::enable_shared_from_this<Assign> {
    Token name;
    std::shared_ptr<Expr> value;

    Assign(Token name, std::shared_ptr<Expr> value)
        : name(std::move(name)), value(std::move(value)) {}

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

struct FunctionStmt : Stmt, public std::enable_shared_from_this<FunctionStmt> {
    Token name;
    std::vector<Token> params;
    std::vector<Token> param_types;
    std::optional<Token> return_type;
    std::shared_ptr<BlockStmt> body;

    FunctionStmt(Token name, std::vector<Token> params, std::vector<Token> param_types, std::optional<Token> return_type, std::shared_ptr<BlockStmt> body)
        : name(std::move(name)), params(std::move(params)), param_types(std::move(param_types)), return_type(std::move(return_type)), body(std::move(body)) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct ForStmt : Stmt, public std::enable_shared_from_this<ForStmt> {
    std::shared_ptr<Stmt> initializer;
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Expr> increment;
    std::shared_ptr<Stmt> body;

    ForStmt(std::shared_ptr<Stmt> initializer, std::shared_ptr<Expr> condition, std::shared_ptr<Expr> increment, std::shared_ptr<Stmt> body)
        : initializer(std::move(initializer)), condition(std::move(condition)), increment(std::move(increment)), body(std::move(body)) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct WhileStmt : Stmt, public std::enable_shared_from_this<WhileStmt> {
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> body;

    WhileStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body)
        : condition(std::move(condition)), body(std::move(body)) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct BlockStmt : Stmt, public std::enable_shared_from_this<BlockStmt> {
    std::vector<std::shared_ptr<Stmt>> statements;

    BlockStmt(std::vector<std::shared_ptr<Stmt>> statements)
        : statements(std::move(statements)) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct IfStmt : Stmt, public std::enable_shared_from_this<IfStmt> {
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> then_branch;
    std::shared_ptr<Stmt> else_branch;

    IfStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> then_branch, std::shared_ptr<Stmt> else_branch)
        : condition(std::move(condition)), then_branch(std::move(then_branch)), else_branch(std::move(else_branch)) {}

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
