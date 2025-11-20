#ifndef CHTHOLLY_ASTPRINTER_HPP
#define CHTHOLLY_ASTPRINTER_HPP

#include "AST.hpp"
#include <string>
#include <vector>

namespace chtholly {

class ASTPrinter : public ExprVisitor, public StmtVisitor {
public:
    std::string print(const std::vector<std::shared_ptr<Stmt>>& statements);

    std::any visitAssignExpr(std::shared_ptr<Assign> expr) override;
    std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override;
    std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override;
    std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override;
    std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override;
    std::any visitVariableExpr(std::shared_ptr<Variable> expr) override;

    std::any visitExpressionStmt(std::shared_ptr<ExpressionStmt> stmt) override;
    std::any visitVarStmt(std::shared_ptr<VarStmt> stmt) override;

private:
    std::string parenthesize(const std::string& name, const std::vector<std::shared_ptr<Expr>>& exprs);
    std::string parenthesize(const std::string& name, const std::vector<std::shared_ptr<Stmt>>& stmts);
};

} // namespace chtholly

#endif // CHTHOLLY_ASTPRINTER_HPP
