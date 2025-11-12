#ifndef CHTHOLLY_EXPR_H
#define CHTHOLLY_EXPR_H

#include "Token.h"
#include <any>
#include <memory>
#include <vector>

namespace Chtholly {

class Assign;
class Literal;
class Variable;

class ExprVisitor {
public:
    virtual std::any visitAssignExpr(Assign& expr) = 0;
    virtual std::any visitLiteralExpr(Literal& expr) = 0;
    virtual std::any visitVariableExpr(Variable& expr) = 0;
};

class Expr {
public:
    virtual ~Expr() = default;
    virtual std::any accept(ExprVisitor& visitor) = 0;
};

class Assign : public Expr {
public:
    Assign(Token name, std::shared_ptr<Expr> value)
        : name(std::move(name)), value(std::move(value)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitAssignExpr(*this);
    }

    Token name;
    std::shared_ptr<Expr> value;
};

class Literal : public Expr {
public:
    Literal(std::any value) : value(std::move(value)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitLiteralExpr(*this);
    }

    std::any value;
};

class Variable : public Expr {
public:
    Variable(Token name) : name(std::move(name)) {}

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visitVariableExpr(*this);
    }

    Token name;
};

} // namespace Chtholly

#endif // CHTHOLLY_EXPR_H
