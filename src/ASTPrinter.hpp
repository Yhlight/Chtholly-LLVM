#ifndef CHTHOLLY_AST_PRINTER_HPP
#define CHTHOLLY_AST_PRINTER_HPP

#include "AST.hpp"
#include <string>
#include <vector>
#include <memory>

class ASTPrinter : public ExprVisitor, public StmtVisitor {
public:
    std::string print(const std::vector<std::shared_ptr<Stmt>>& statements);

    std::any visitBinaryExpr(const std::shared_ptr<Binary>& expr) override;
    std::any visitUnaryExpr(const std::shared_ptr<Unary>& expr) override;
    std::any visitLiteralExpr(const std::shared_ptr<Literal>& expr) override;
    std::any visitGroupingExpr(const std::shared_ptr<Grouping>& expr) override;
    std::any visitVariableExpr(const std::shared_ptr<Variable>& expr) override;
    std::any visitAssignExpr(const std::shared_ptr<Assign>& expr) override;

    std::any visitExpressionStmt(const std::shared_ptr<ExpressionStmt>& stmt) override;
    std::any visitPrintStmt(const std::shared_ptr<PrintStmt>& stmt) override;
    std::any visitVarStmt(const std::shared_ptr<VarStmt>& stmt) override;
    std::any visitBlockStmt(const std::shared_ptr<BlockStmt>& stmt) override;

private:
    std::string parenthesize(const std::string& name, const std::vector<std::shared_ptr<Expr>>& exprs);
    std::string parenthesizeStmt(const std::string& name, const std::vector<std::shared_ptr<Stmt>>& stmts);
};

#endif // CHTHOLLY_AST_PRINTER_HPP
