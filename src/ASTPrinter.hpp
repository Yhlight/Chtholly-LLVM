#pragma once

#include "AST.hpp"
#include <string>
#include <vector>

class ASTPrinter : public ExprVisitor<std::any>, public StmtVisitor<std::any> {
public:
    std::string print(const std::vector<std::unique_ptr<Stmt>>& statements);

    std::any visit(const BinaryExpr& expr) override;
    std::any visit(const GroupingExpr& expr) override;
    std::any visit(const LiteralExpr& expr) override;
    std::any visit(const UnaryExpr& expr) override;
    std::any visit(const VarExpr& expr) override;
    std::any visit(const AssignExpr& expr) override;
    std::any visit(const CallExpr& expr) override;

    std::any visit(const ExprStmt& stmt) override;
    std::any visit(const VarStmt& stmt) override;
    std::any visit(const BlockStmt& stmt) override;
    std::any visit(const IfStmt& stmt) override;
    std::any visit(const WhileStmt& stmt) override;
    std::any visit(const ReturnStmt& stmt) override;

private:
    std::any parenthesize(const std::string& name, const std::vector<const Expr*>& exprs);
};
