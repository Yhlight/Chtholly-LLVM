#pragma once

#include "AST.hpp"
#include <string>
#include <vector>

class ASTPrinter : public ExprVisitor, public StmtVisitor {
public:
    std::string print(const std::vector<std::shared_ptr<Stmt>>& statements);
    std::string print(std::shared_ptr<Expr> expr);
    std::string print(std::shared_ptr<Stmt> stmt);

    std::any visit(std::shared_ptr<Assign> expr) override;
    std::any visit(std::shared_ptr<Binary> expr) override;
    std::any visit(std::shared_ptr<Grouping> expr) override;
    std::any visit(std::shared_ptr<Literal> expr) override;
    std::any visit(std::shared_ptr<Unary> expr) override;
    std::any visit(std::shared_ptr<Variable> expr) override;

    std::any visit(std::shared_ptr<ExpressionStmt> stmt) override;
    std::any visit(std::shared_ptr<VarStmt> stmt) override;

private:
    std::string parenthesize(const std::string& name, const std::vector<std::shared_ptr<Expr>>& exprs);
};
