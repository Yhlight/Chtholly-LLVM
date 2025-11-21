#ifndef CHTHOLLY_AST_HPP
#define CHTHOLLY_AST_HPP

#include "Token.hpp"
#include "Type.hpp"
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
struct Assign;
struct ArrayLiteral;
struct SubscriptExpr;
struct ScopeExpr;
struct LambdaExpr;
struct GetExpr;
struct SetExpr;
struct ThisExpr;
struct ExpressionStmt;
struct VarStmt;
struct BlockStmt;
struct FunctionStmt;
struct ReturnStmt;
struct IfStmt;
struct WhileStmt;
struct ForStmt;
struct SwitchStmt;
struct BreakStmt;
struct FallthroughStmt;
struct EnumStmt;
struct ClassStmt;
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
    virtual R visit(const std::shared_ptr<Assign>& expr) = 0;
    virtual R visit(const std::shared_ptr<ArrayLiteral>& expr) = 0;
    virtual R visit(const std::shared_ptr<SubscriptExpr>& expr) = 0;
    virtual R visit(const std::shared_ptr<ScopeExpr>& expr) = 0;
    virtual R visit(const std::shared_ptr<LambdaExpr>& expr) = 0;
    virtual R visit(const std::shared_ptr<GetExpr>& expr) = 0;
    virtual R visit(const std::shared_ptr<SetExpr>& expr) = 0;
    virtual R visit(const std::shared_ptr<ThisExpr>& expr) = 0;
    virtual ~ExprVisitor() = default;
};

template <typename R>
struct StmtVisitor {
    virtual R visit(const std::shared_ptr<ExpressionStmt>& stmt) = 0;
    virtual R visit(const std::shared_ptr<VarStmt>& stmt) = 0;
    virtual R visit(const std::shared_ptr<BlockStmt>& stmt) = 0;
    virtual R visit(const std::shared_ptr<FunctionStmt>& stmt) = 0;
    virtual R visit(const std::shared_ptr<ReturnStmt>& stmt) = 0;
    virtual R visit(const std::shared_ptr<IfStmt>& stmt) = 0;
    virtual R visit(const std::shared_ptr<WhileStmt>& stmt) = 0;
    virtual R visit(const std::shared_ptr<ForStmt>& stmt) = 0;
    virtual R visit(const std::shared_ptr<SwitchStmt>& stmt) = 0;
    virtual R visit(const std::shared_ptr<BreakStmt>& stmt) = 0;
    virtual R visit(const std::shared_ptr<FallthroughStmt>& stmt) = 0;
    virtual R visit(const std::shared_ptr<EnumStmt>& stmt) = 0;
    virtual R visit(const std::shared_ptr<ClassStmt>& stmt) = 0;
    virtual ~StmtVisitor() = default;
};

// Base classes
struct Expr {
    virtual std::any accept(ExprVisitor<std::any>& visitor) = 0;
};

struct Stmt {
    virtual std::any accept(StmtVisitor<std::any>& visitor) = 0;
};

struct Parameter {
    Token name;
    std::shared_ptr<Type> type;
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
    Call(std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments, std::vector<std::shared_ptr<Type>> type_arguments)
        : callee(std::move(callee)), paren(std::move(paren)), arguments(std::move(arguments)), type_arguments(std::move(type_arguments)) {}

    std::any accept(ExprVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::shared_ptr<Expr> callee;
    const Token paren;
    const std::vector<std::shared_ptr<Expr>> arguments;
    const std::vector<std::shared_ptr<Type>> type_arguments;
};

struct Assign : Expr, public std::enable_shared_from_this<Assign> {
    Assign(std::shared_ptr<Expr> target, std::shared_ptr<Expr> value)
        : target(std::move(target)), value(std::move(value)) {}

    std::any accept(ExprVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::shared_ptr<Expr> target;
    const std::shared_ptr<Expr> value;
};

struct ArrayLiteral : Expr, public std::enable_shared_from_this<ArrayLiteral> {
    ArrayLiteral(std::vector<std::shared_ptr<Expr>> elements)
        : elements(std::move(elements)) {}

    std::any accept(ExprVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::vector<std::shared_ptr<Expr>> elements;
};

struct SubscriptExpr : Expr, public std::enable_shared_from_this<SubscriptExpr> {
    SubscriptExpr(std::shared_ptr<Expr> name, std::shared_ptr<Expr> index)
        : name(std::move(name)), index(std::move(index)) {}

    std::any accept(ExprVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::shared_ptr<Expr> name;
    const std::shared_ptr<Expr> index;
};

struct ScopeExpr : Expr, public std::enable_shared_from_this<ScopeExpr> {
    ScopeExpr(std::shared_ptr<Expr> left, Token name)
        : left(std::move(left)), name(std::move(name)) {}

    std::any accept(ExprVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::shared_ptr<Expr> left;
    const Token name;
};

struct LambdaExpr : Expr, public std::enable_shared_from_this<LambdaExpr> {
    LambdaExpr(std::vector<Parameter> params, std::shared_ptr<Type> return_type, std::shared_ptr<BlockStmt> body)
        : params(std::move(params)), return_type(std::move(return_type)), body(std::move(body)) {}

    std::any accept(ExprVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::vector<Parameter> params;
    const std::shared_ptr<Type> return_type;
    const std::shared_ptr<BlockStmt> body;
};

enum class AccessModifier {
    PUBLIC,
    PRIVATE
};

struct GetExpr : Expr, public std::enable_shared_from_this<GetExpr> {
    GetExpr(std::shared_ptr<Expr> object, Token name)
        : object(std::move(object)), name(std::move(name)) {}

    std::any accept(ExprVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::shared_ptr<Expr> object;
    const Token name;
};

struct SetExpr : Expr, public std::enable_shared_from_this<SetExpr> {
    SetExpr(std::shared_ptr<Expr> object, Token name, std::shared_ptr<Expr> value)
        : object(std::move(object)), name(std::move(name)), value(std::move(value)) {}

    std::any accept(ExprVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::shared_ptr<Expr> object;
    const Token name;
    const std::shared_ptr<Expr> value;
};

struct ThisExpr : Expr, public std::enable_shared_from_this<ThisExpr> {
    ThisExpr(Token keyword) : keyword(std::move(keyword)) {}

    std::any accept(ExprVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const Token keyword;
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
    VarStmt(Token name, std::shared_ptr<Type> type, std::shared_ptr<Expr> initializer)
        : name(std::move(name)), type(std::move(type)), initializer(std::move(initializer)) {}

    std::any accept(StmtVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const Token name;
    const std::shared_ptr<Type> type;
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
    FunctionStmt(Token name, std::vector<Token> type_params, std::vector<Parameter> params, std::shared_ptr<Type> return_type, std::shared_ptr<BlockStmt> body)
        : name(std::move(name)), type_params(std::move(type_params)), params(std::move(params)), return_type(std::move(return_type)), body(std::move(body)) {}

    std::any accept(StmtVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const Token name;
    const std::vector<Token> type_params;
    const std::vector<Parameter> params;
    const std::shared_ptr<Type> return_type;
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

struct IfStmt : Stmt, public std::enable_shared_from_this<IfStmt> {
    IfStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch, std::shared_ptr<Stmt> elseBranch)
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}

    std::any accept(StmtVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::shared_ptr<Expr> condition;
    const std::shared_ptr<Stmt> thenBranch;
    const std::shared_ptr<Stmt> elseBranch;
};

struct WhileStmt : Stmt, public std::enable_shared_from_this<WhileStmt> {
    WhileStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body)
        : condition(std::move(condition)), body(std::move(body)) {}

    std::any accept(StmtVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::shared_ptr<Expr> condition;
    const std::shared_ptr<Stmt> body;
};

struct ForStmt : Stmt, public std::enable_shared_from_this<ForStmt> {
    ForStmt(std::shared_ptr<Stmt> initializer, std::shared_ptr<Expr> condition, std::shared_ptr<Expr> increment, std::shared_ptr<Stmt> body)
        : initializer(std::move(initializer)), condition(std::move(condition)), increment(std::move(increment)), body(std::move(body)) {}

    std::any accept(StmtVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::shared_ptr<Stmt> initializer;
    const std::shared_ptr<Expr> condition;
    const std::shared_ptr<Expr> increment;
    const std::shared_ptr<Stmt> body;
};

struct Case {
    Case(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body)
        : condition(std::move(condition)), body(std::move(body)) {}

    const std::shared_ptr<Expr> condition;
    const std::shared_ptr<Stmt> body;
};

struct SwitchStmt : Stmt, public std::enable_shared_from_this<SwitchStmt> {
    SwitchStmt(std::shared_ptr<Expr> expression, std::vector<Case> cases)
        : expression(std::move(expression)), cases(std::move(cases)) {}

    std::any accept(StmtVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const std::shared_ptr<Expr> expression;
    const std::vector<Case> cases;
};

struct BreakStmt : Stmt, public std::enable_shared_from_this<BreakStmt> {
    BreakStmt() {}

    std::any accept(StmtVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct FallthroughStmt : Stmt, public std::enable_shared_from_this<FallthroughStmt> {
    FallthroughStmt() {}

    std::any accept(StmtVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct EnumStmt : Stmt, public std::enable_shared_from_this<EnumStmt> {
    EnumStmt(Token name, std::vector<Token> members)
        : name(std::move(name)), members(std::move(members)) {}

    std::any accept(StmtVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const Token name;
    const std::vector<Token> members;
};

struct ClassStmt : Stmt, public std::enable_shared_from_this<ClassStmt> {
    struct ClassMember {
        std::shared_ptr<Stmt> declaration;
        AccessModifier access;
        bool is_static;
    };

    ClassStmt(Token name, std::vector<ClassMember> members)
        : name(std::move(name)), members(std::move(members)) {}

    std::any accept(StmtVisitor<std::any>& visitor) override {
        return visitor.visit(shared_from_this());
    }

    const Token name;
    const std::vector<ClassMember> members;
};

} // namespace chtholly

#endif // CHTHOLLY_AST_HPP
