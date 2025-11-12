#ifndef CHTHOLLY_STMT_H
#define CHTHOLLY_STMT_H

#include "Expr.h"
#include "Token.h"
#include <any>
#include <memory>
#include <vector>

namespace Chtholly {

class Expression;
class Var;

class StmtVisitor {
public:
    virtual std::any visitExpressionStmt(Expression& stmt) = 0;
    virtual std::any visitVarStmt(Var& stmt) = 0;
};

class Stmt {
public:
    virtual ~Stmt() = default;
    virtual std::any accept(StmtVisitor& visitor) = 0;
};

class Expression : public Stmt {
public:
    Expression(std::shared_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visitExpressionStmt(*this);
    }

    std::shared_ptr<Expr> expression;
};

class Var : public Stmt {
public:
    Var(Token name, std::shared_ptr<Expr> initializer)
        : name(std::move(name)), initializer(std::move(initializer)) {}

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visitVarStmt(*this);
    }

    Token name;
    std::shared_ptr<Expr> initializer;
};

} // namespace Chtholly

#endif // CHTHOLLY_STMT_H
